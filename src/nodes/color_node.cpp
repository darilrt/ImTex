#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "nodes/color_node.h"
#include "shader.h"
#include "project.h"
#include "connection.h"

static ComputeShader* computeShader;

ColorNode::ColorNode() {
    title = "Color";
    
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("color");
    }
    
    color = ImVec4(1, 1, 1, 1);
}

void ColorNode::OnInspector() {
    bool change = false;
    
    change |= ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_NoOptions);
    
    if (change) {UpdateTexture();}
}

void ColorNode::ComputeTexture() {
    computeShader->Begin(texture);
    
    computeShader->SetFloat4("color", color.x, color.y, color.z, color.w);
    
    computeShader->Dispatch(
        (Project::texture_size.x + 16 - 1) / 16,
        (Project::texture_size.y + 16 - 1) / 16,
        1
    );
    computeShader->End();
}