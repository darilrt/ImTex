#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "nodes/material_node.h"
#include "shader.h"
#include "project.h"
#include "connection.h"

MaterialNode::MaterialNode() {
    title = "Material";
    
    inPoints.push_back(new NodePoint(this, "Albedo"));
    inPoints.push_back(new NodePoint(this, "Normal Map"));
    inPoints.push_back(new NodePoint(this, "Metallic"));
    inPoints.push_back(new NodePoint(this, "Roughness"));
    inPoints.push_back(new NodePoint(this, "Ambient Occlusion"));
}

void MaterialNode::OnInspector() {
}

void MaterialNode::ComputeTexture() {
}