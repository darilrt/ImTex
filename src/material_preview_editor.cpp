#include "material_preview_editor.h"
#include "node_editor.h"
#include "node.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

MaterialPreviewEditor::MaterialPreviewEditor(NodeEditor* _nodeEditor) {
    open = true;
    title = "Material Preview";
    nodeEditor = _nodeEditor;
    
    pbrShader = Shader::Load("pbr.vert", "pbr.frag");
    meshSphere = Mesh::LoadOBJ("mesh/sphere.obj");
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // glGenRenderbuffers(1, &rbo);
    // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, 512, 512);  
    // glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
    // glReadBuffer(GL_COLOR_ATTACHMENT0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nodeEditor->materialNode->texture->ptr, 0);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        SDL_Log("Framebuffer not complete");
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MaterialPreviewEditor::PreRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, 512, 512);
    
    glClearColor(0.1, 0.1, 0.105, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 512.0f / 512.0f, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    static glm::vec3 rot = glm::vec3(0, 0, 0);
    
    glTranslatef(0, 0, -3);
    glRotatef(rot.x, 1, 0, 0);
    glRotatef(rot.y, 0, 1, 0);
    glRotatef(rot.z, 0, 0, 1);
    
    pbrShader->Begin();
    
    if (nodeEditor->materialNode->inPoints[0]->connection) {
        pbrShader->SetFloat("albedoValue", -1.0);
        pbrShader->SetTexture2D("albedoMap", nodeEditor->materialNode->inPoints[0]->connection->out->node->texture);
    }
    else {
        pbrShader->SetFloat("albedoValue", 1.0);
        pbrShader->SetFloat3("albedoColor", 1.0, 1.0, 1.0);
    }
    
    if (nodeEditor->materialNode->inPoints[1]->connection) {
        pbrShader->SetFloat("normalValue", -1.0);
        pbrShader->SetTexture2D("normalMap", nodeEditor->materialNode->inPoints[1]->connection->out->node->texture);
    }
    else {
        pbrShader->SetFloat("normalValue", 0.0);
    }
    
    if (nodeEditor->materialNode->inPoints[2]->connection) {
        pbrShader->SetFloat("metallicValue", -1.0);
        pbrShader->SetTexture2D("metallicMap", nodeEditor->materialNode->inPoints[2]->connection->out->node->texture);
    }
    else {
        pbrShader->SetFloat("metallicValue", 0.0);
    }
    
    if (nodeEditor->materialNode->inPoints[3]->connection) {
        pbrShader->SetFloat("roughnessValue", -1.0);
        pbrShader->SetTexture2D("roughnessMap", nodeEditor->materialNode->inPoints[3]->connection->out->node->texture);
    }
    else {
        pbrShader->SetFloat("roughnessValue", 0.0);
    }
    
    if (nodeEditor->materialNode->inPoints[4]->connection) {
        pbrShader->SetFloat("aoValue", -1.0);
        pbrShader->SetTexture2D("aoMap", nodeEditor->materialNode->inPoints[4]->connection->out->node->texture);
    }
    else {
        pbrShader->SetFloat("aoValue", 0.0);
    }
    
    meshSphere->Draw();
    
    pbrShader->End();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MaterialPreviewEditor::OnDraw() {
    ImGui::ShowMetricsWindow();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(pos.x + canvas_sz.x, pos.y + canvas_sz.y);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // ImGui::Image((void*)(intptr_t) nodeEditor->materialNode->texture->ptr, ImVec2(512, 512));
    draw_list->AddImage((void*)(intptr_t) nodeEditor->materialNode->texture->ptr, pos, canvas_p1);
    
    ProcessEvents();
}

void MaterialPreviewEditor::ProcessEvents() {
    // ImGuiIO& io = ImGui::GetIO();
}