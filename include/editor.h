#ifndef H_EDITOR
#define H_EDITOR

#include <iostream>
#include <SDL2/SDL.h>

class Editor {
public:
    bool open;
    std::string title;
    
    Editor();
    virtual void OnDraw() {};
    virtual void OnEvent(SDL_Event &event) {};
    
private:
};

#endif // H_EDITOR