#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <fstream>

#include "shader.h"
#include "project.h"

void print_shader_log(int shader) {
    if(glIsShader(shader)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* infoLog = new char[maxLength];
        
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if(infoLogLength > 0) {
            SDL_Log(infoLog);
        }
        
        delete[] infoLog;
    } else {
        std::string _log = "Name ";
        _log += std::to_string(shader);
        _log += " is not a shader";
        SDL_Log(_log.c_str());
    }
}

ComputeShader::ComputeShader() {}

ComputeShader::~ComputeShader() {}

void ComputeShader::Begin(Texture2D* renderTexture) {
    glUseProgram(program);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTexture->ptr);
    
    glBindImageTexture(0, renderTexture->ptr, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z) {
    glDispatchCompute(x, y, z);
}

void ComputeShader::End() {
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glUseProgram(0);
}

ComputeShader* ComputeShader::Load(const std::string name) {
    std::ifstream file("shaders/" + name + ".comp");
    
    std::string source, line;
    
    while (std::getline(file, line)) {
        source += "\n" + line;
    }
    
    // compile shader
    ComputeShader* cs = new ComputeShader();
    cs->program = glCreateProgram();
    
    uint32_t shader = glCreateShader(GL_COMPUTE_SHADER);
    const GLchar* source_c[] = {source.c_str()};
    glShaderSource(shader, 1, source_c, NULL);
    glCompileShader(shader);
    
    GLint rvalue = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &rvalue);

    if(rvalue != GL_TRUE) {
        SDL_Log("Unable to compile compute shader \"shaders/%s.comp\"\n", name.c_str());
        print_shader_log(shader);
    }
    
    glAttachShader(cs->program, shader);
    glLinkProgram(cs->program);

    char* log = new char[1000];
    int length = 0;
    glGetProgramiv(cs->program, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        glGetProgramInfoLog(cs->program, 1000, &length, log);
        SDL_Log("Error: Linker log:\n%s\n", log);
    }
    
    return cs;
}

void ComputeShader::SetTexture2D(uint32_t unit, Texture2D* texture) {
    glBindImageTexture(unit, texture ? texture->ptr : 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
}

void ComputeShader::SetInt(const char* name, int v1) {
    glUniform1i(location(name), v1);
}

void ComputeShader::SetInt2(const char* name, int v1, int v2) {
    glUniform2i(location(name), v1, v2);
}

void ComputeShader::SetInt3(const char* name, int v1, int v2, int v3) {
    glUniform3i(location(name), v1, v2, v3);
}

void ComputeShader::SetFloat(const char* name, float v1) {
    glUniform1f(location(name), v1);
}

void ComputeShader::SetFloat2(const char* name, float v1, float v2) {
    glUniform2f(location(name), v1, v2);
}

void ComputeShader::SetFloat3(const char* name, float v1, float v2, float v3) {
    glUniform3f(location(name), v1, v2, v3);
}

void ComputeShader::SetFloat4(const char* name, float v1, float v2, float v3, float v4) {
    glUniform4f(location(name), v1, v2, v3, v4);
}

Shader::Shader() {}

Shader::~Shader() {}

Shader* Shader::Load(const char* vert, const char* frag) {
    std::ifstream file("shaders/" + std::string((char*) vert));
    std::string source, line;
    while (std::getline(file, line)) {
        source += "\n" + line;
    }

    uint32_t shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* source_v[] = {source.c_str()};
    glShaderSource(shaderVertex, 1, source_v, NULL);
    glCompileShader(shaderVertex);
    
    GLint rvalue = GL_FALSE;
    glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &rvalue);

    if(rvalue != GL_TRUE) {
        SDL_Log("Unable to compile vertex shader \"shaders/%s\"\n", vert);
        print_shader_log(shaderVertex);
    }
    
    std::ifstream file2("shaders/" + std::string((char*) frag));
    source = "";
    while (std::getline(file2, line)) {
        source += "\n" + line;
    }

    uint32_t shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* source_f[] = {source.c_str()};
    glShaderSource(shaderFragment, 1, source_f, NULL);
    glCompileShader(shaderFragment);

    rvalue = GL_FALSE;
    glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &rvalue);

    if(rvalue != GL_TRUE) {
        SDL_Log("Unable to compile fragment shader \"shaders/%s\"\n", frag);
        print_shader_log(shaderFragment);
    }
    
    Shader* shader = new Shader();
    shader->program = glCreateProgram();
    
    glAttachShader(shader->program, shaderVertex);
    glAttachShader(shader->program, shaderFragment);
    glLinkProgram(shader->program);

    char* log = new char[1000];
    int length = 0;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        glGetProgramInfoLog(shader->program, 1000, &length, log);
        SDL_Log("Error: Linker log:\n%s\n", log);
    }
    
    return shader;
}

void Shader::Begin() {
    glUseProgram(program);
    samplerCount = 0;
}

void Shader::End() {
    glUseProgram(0);
}

void Shader::SetTexture2D(const char* location, Texture2D* texture) {
    glActiveTexture(GL_TEXTURE0 + samplerCount);
    glBindTexture(GL_TEXTURE_2D, texture->ptr);
    
    glUniform1i(Location(location), samplerCount);
    
    samplerCount++;
}

void Shader::SetFloat(const char* location, float v1) {
    glUniform1f(Location(location), v1);
}

void Shader::SetFloat2(const char* location, float v1, float v2) {
    glUniform2f(Location(location), v1, v2);
}

void Shader::SetFloat3(const char* location, float v1, float v2, float v3) {
    glUniform3f(Location(location), v1, v2, v3);
}