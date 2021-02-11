#ifndef H_TEXTURE2D
#define H_TEXTURE2D

#include <glm/glm.hpp>

class Texture2D {
public:
    int32_t width, height;
    uint32_t ptr;
    
    Texture2D();
    Texture2D(int32_t width, int32_t height);
    ~Texture2D();
    
    void Bind(int32_t location, uint32_t num);
    
    static Texture2D* LoadFromFile(const char* path);
    
private:
};

#endif // H_TEXTURE2D