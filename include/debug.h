#ifdef DEBUG_MODE
#ifndef H_DEBUG
#define H_DEBUG

#include <vector>
#include <functional>
#include <imgui.h>
#include <glm/glm.hpp>

class Debug {
public:
    std::vector<std::function<void(void)>> draws;

    static Debug* getInstance() {
        if (!Debug::instance) {
            Debug::instance = new Debug();
        }
        
        return Debug::instance;
    }
    
    void Add(std::function<void(void)> callback) {
        Debug* debug = getInstance();
        
        debug->draws.push_back(callback);
    }
    
private:
    static Debug* instance;
    
    Debug() {};
};

#endif // H_DEBUG
#endif // DEBUG_MODE