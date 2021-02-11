#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "nodes/blur_node.h"
#include "shader.h"
#include "project.h"
#include "connection.h"

static ComputeShader* computeShader;

BlurNode::BlurNode() {
    title = "Blur";
    
    inPoints.push_back(new NodePoint(this, "Texture"));
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("<Compute shader>");
    }
    
    // init properties
    // rotation = 0;
    // offset = ImVec2(0, 0);
    // scale = ImVec2(Project::texture_size.x, Project::texture_size.y);
}

void BlurNode::OnInspector() {
    bool change = false;
    
    // change |= ImGui::DragFloat2("Offset", (float*) &offset, 0.5f);
    // change |= ImGui::DragFloat2("Scale", (float*) &scale, 0.5f);
    // change |= ImGui::DragFloat("Rotation", &rotation, 0.25f, 0.0f, 360.0f);
    
    if (change) {UpdateTexture();}
}

void BlurNode::ComputeTexture() {
    // computeShader->Use();
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
    
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture->ptr);
    
    // glBindImageTexture(0, texture->ptr, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    
    // glBindImageTexture(1, 
        // inPoints[0]->connection ? inPoints[0]->connection->out->node->texture->ptr : 0,
        // 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8
    // );
    
    // glUniform2f(computeShader->location("offset"), offset.x, offset.y);
    // glUniform2f(computeShader->location("scale"), scale.x, scale.y);
    // glUniform1f(computeShader->location("rotation"), rotation);
    
    // glDispatchCompute(
        // (Project::texture_size.x + 16 - 1) / 16,
        // (Project::texture_size.y + 16 - 1) / 16,
        // 1
    // );
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
}