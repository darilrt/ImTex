#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture2d.h"

Texture2D::Texture2D() {
}

Texture2D::Texture2D(int32_t _width, int32_t _height) {
    width = _width;
    height = _height;
    
    glGenTextures(1, &ptr);
    glBindTexture(GL_TEXTURE_2D, ptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &ptr);
}

Texture2D* Texture2D::LoadFromFile(const char* path) {
    Texture2D* texture = new Texture2D();
    
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(path, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return NULL;

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(image_data);

    texture->ptr = image_texture;
    texture->width = image_width;
    texture->height = image_height;
    
    return texture;
}

void Texture2D::Bind(int32_t location, uint32_t num) {
    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(GL_TEXTURE_2D, ptr);
    glUniform1i(location, num);
}