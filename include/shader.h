#ifndef H_SHADER
#define H_SHADER

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <fstream>

#include "shader.h"
#include "project.h"

#include "texture2d.h"

class Shader {
public:
    uint32_t program;
    
    Shader();
    ~Shader();
    
    void Begin();
    
    void SetTexture2D(const char* location, Texture2D* texture);
    
    void SetFloat(const char* location, float v1);
    void SetFloat2(const char* location, float v1, float v2);
    void SetFloat3(const char* location, float v1, float v2, float v3);
    
    void End();
    
    static Shader* Load(const char* vert, const char* frag);
    
    inline int32_t Location(const char* name) {
        return glGetUniformLocation(program, name);
    }
        
private:
    uint32_t samplerCount;
};

class ComputeShader {
public:
    uint32_t program;
    
    ComputeShader();
    ~ComputeShader();
    
    void Begin(Texture2D* renderTexture);
    
    void SetTexture2D(uint32_t unit, Texture2D* renderTexture);
    
    void SetInt(const char* name, int v1);
    void SetInt2(const char* name, int v1, int v2);
    void SetInt3(const char* name, int v1, int v2, int v3);
    
    void SetFloat(const char* name, float v1);
    void SetFloat2(const char* name, float v1, float v2);
    void SetFloat3(const char* name, float v1, float v2, float v3);
    void SetFloat4(const char* name, float v1, float v2, float v3, float v4);
    
    void Dispatch(uint32_t x, uint32_t y, uint32_t z);
    void End();
    
    inline int32_t location(const char* name) {
        return glGetUniformLocation(program, name);
    }
    
    static ComputeShader* Load(const std::string name);
    
private:
    uint32_t count;
};

#endif // H_SHADER