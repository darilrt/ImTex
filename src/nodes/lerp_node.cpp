#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "nodes/lerp_node.h"
#include "shader.h"
#include "project.h"
#include "connection.h"

static ComputeShader* computeShader;

LerpNode::LerpNode() {
    title = "Lerp";
    
    inPoints.push_back(new NodePoint(this, "Texture"));
    inPoints.push_back(new NodePoint(this, "Texture"));
    inPoints.push_back(new NodePoint(this, "Factor"));
    
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("lerp");
    }
    
    factor = 0;
}

void LerpNode::OnInspector() {
    bool change = false;
    
    change |= ImGui::DragFloat("Factor", &factor, 0.01f, 0.0f, 1.0f);
    
    if (change) {UpdateTexture();}
}

void LerpNode::ComputeTexture() {
    computeShader->Begin(texture);
    
    computeShader->SetTexture2D(
        1, inPoints[0]->connection ? inPoints[0]->connection->out->node->texture : NULL
    );
    
    computeShader->SetTexture2D(
        2, inPoints[1]->connection ? inPoints[1]->connection->out->node->texture : NULL
    );
    
    computeShader->SetTexture2D(
        3, inPoints[2]->connection ? inPoints[2]->connection->out->node->texture : NULL
    );
    
    if (inPoints[2]->connection) {
        computeShader->SetFloat("factor", -1.0);
    }
    else {
        computeShader->SetFloat("factor", factor);
    }
    
    computeShader->Dispatch(
        (Project::texture_size.x + 16 - 1) / 16,
        (Project::texture_size.y + 16 - 1) / 16,
        1
    );
    computeShader->End();
}