#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertex, std::vector<uint32_t> _index) {
    vertex = _vertex;
    index = _index;
    
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
  
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->_ebo);
    glDeleteBuffers(1, &this->_vbo);
    
    glDeleteVertexArrays(1, &this->_vao);
}

Mesh* Mesh::LoadOBJ(const char* path) {
    FILE* file = fopen(path, "r");
    
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        return NULL;
    }
    
    std::vector<glm::vec3> temp_vertex;
    std::vector<glm::vec3> temp_normal;
    std::vector<glm::vec2> temp_uv;
    
    std::vector<Vertex> vertex;
    std::vector<uint32_t> faces;
    
    while (1) {
        char lineHeader[128];
        
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertex.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uv.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normal.push_back(normal);
        }   
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            if (matches != 9) {
                printf("File can't be read: ( Try exporting with other options\n");
                break;
            }
            
            faces.push_back(vertex.size());
            faces.push_back(vertex.size() + 1);
            faces.push_back(vertex.size() + 2);
            vertex.push_back({temp_vertex[vertexIndex[0] - 1], temp_normal[normalIndex[0] - 1], temp_uv[uvIndex[0] - 1]});
            vertex.push_back({temp_vertex[vertexIndex[1] - 1], temp_normal[normalIndex[1] - 1], temp_uv[uvIndex[1] - 1]});
            vertex.push_back({temp_vertex[vertexIndex[2] - 1], temp_normal[normalIndex[2] - 1], temp_uv[uvIndex[2] - 1]});
        }
    }
    
    Mesh* mesh = new Mesh(vertex, faces);
    return mesh;
}

void Mesh::Draw() {
    // unsigned int diffuseNr = 1;
    // unsigned int specularNr = 1;
    // for(unsigned int i = 0; i < textures.size(); i++)
    // {
        // glActiveTexture(GL_TEXTURE0 + i);
        // string number;
        // string name = textures[i].type;
        // if(name == "texture_diffuse")
            // number = std::to_string(diffuseNr++);
        // else if(name == "texture_specular")
            // number = std::to_string(specularNr++);

        // shader.setFloat(("material." + name + number).c_str(), i);
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
    // }
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}  