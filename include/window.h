#ifndef H_WINDOW
#define H_WINDOW

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <SDL2/SDL.h>

class Window {
public:
    bool is_open;
    
    Window(const char *title, glm::vec2 size);
    
    void Run();
    
    virtual void PreDraw() {};
    virtual void OnDraw() {};
    
private:
    SDL_WindowFlags window_flags;
    SDL_GLContext gl_context;
    SDL_Window *window;
    SDL_Event event;
};

#endif // H_WINDOW