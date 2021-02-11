#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "nodes/shape_node.h"
#include "shader.h"
#include "project.h"

static ComputeShader* computeShader;

ShapeNode::ShapeNode() {
    title = "Shape";
    
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("shape");
    }
    
    corners = 4;
    shapeStart = 0.32;
    gradientStart = 0.3;
    rotation = 0;
    offset = ImVec2(0, 0);
}

void ShapeNode::OnInspector() {
    bool change = false;
    
    change |= ImGui::DragFloat2("Offset", (float*) &offset, 0.5f);
    change |= ImGui::DragInt("Corners", &corners, 1, 0, 30);
    change |= ImGui::DragFloat("Rotation", &rotation, 0.25f, 0.0f, 360.0f);
    change |= ImGui::SliderFloat("Shape Start", &shapeStart, 0.0f, 1.0f);
    change |= ImGui::SliderFloat("Gradient Start", &gradientStart, 0.0f, 1.0f);
    
    if (change) {UpdateTexture();}
}

void ShapeNode::ComputeTexture() {
    computeShader->Begin(texture);
    
    computeShader->SetInt("corners", corners);
    
    computeShader->SetFloat2("offset", offset.x, offset.y);
    computeShader->SetFloat("shapeStart", shapeStart);
    computeShader->SetFloat("gradientStart", gradientStart);
    computeShader->SetFloat("rotation", rotation);
    
    computeShader->Dispatch(
        (Project::texture_size.x + 16 - 1) / 16,
        (Project::texture_size.y + 16 - 1) / 16,
        1
    );
    computeShader->End();
}