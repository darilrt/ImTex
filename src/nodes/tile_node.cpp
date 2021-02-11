#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "nodes/tile_node.h"
#include "shader.h"
#include "project.h"
#include "connection.h"

static ComputeShader* computeShader;

TileNode::TileNode() {
    title = "Tile";
    
    inPoints.push_back(new NodePoint(this, "Texture"));
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("tile");
    }
}

void TileNode::OnInspector() {
    bool change = false;
    
    // change |= ImGui::DragFloat2("Offset", (float*) &offset, 0.5f);
    // change |= ImGui::DragFloat2("Scale", (float*) &scale, 0.5f);
    // change |= ImGui::DragFloat("Rotation", &rotation, 0.25f, 0.0f, 360.0f);
    
    if (change) {UpdateTexture();}
}

void TileNode::ComputeTexture() {
    computeShader->Begin(texture);
    
    // computeShader->SetTexture2D(
        // 1, inPoints[0]->connection ? inPoints[0]->connection->out->node->texture : NULL
    // );
    
    // computeShader->SetFloat2("offset", offset.x, offset.y);
    // computeShader->SetFloat2("scale", scale.x, scale.y);
    // computeShader->SetFloat("rotation", rotation);
    
    computeShader->Dispatch(
        (Project::texture_size.x + 16 - 1) / 16,
        (Project::texture_size.y + 16 - 1) / 16,
        1
    );
    computeShader->End();
}
