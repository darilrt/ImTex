#ifndef H_MESH
#define H_MESH

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    std::vector<Vertex> vertex;
    std::vector<uint32_t> index;
    
    Mesh(std::vector<Vertex> vertex, std::vector<uint32_t> index);
    ~Mesh();
    
    void Draw();
    
    static Mesh* LoadOBJ(const char* path);

private:
    uint32_t _vbo, // Vertex Buffer
             _vao, // Vertex Array
             _ebo; // Element Buffer
};

#endif // H_MESH