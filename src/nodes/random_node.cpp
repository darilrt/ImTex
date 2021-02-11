#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "shader.h"
#include "nodes/random_node.h"
#include "project.h"

static ComputeShader* computeShader;

RandomNode::RandomNode() {
    title = "Random";
    
    outPoints.push_back(new NodePoint(this, "Texture"));
    
    if (!computeShader) {
        computeShader = ComputeShader::Load("random");
    }
}

void RandomNode::OnInspector() {
    
}

void RandomNode::ComputeTexture() {
    // computeShader->Use();
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
    
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture->ptr);
    
    // glBindImageTexture(1, texture->ptr, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    
    // glDispatchCompute(
        // (Project::texture_size.x + 16 - 1) / 16,
        // (Project::texture_size.y + 16 - 1) / 16,
        // 1
    // );
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
}