#ifndef H_NODE
#define H_NODE

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>

#include "texture2d.h"
#include "node_point.h"

class NodeEditor;

class Node {
public:
    std::vector<NodePoint*> inPoints, outPoints;
    
    Texture2D* texture;
    
    std::string title;
    
    NodeEditor *editor;
    glm::vec2 position;
    bool isSelected;
    
    Node();
    ~Node();
    void OnDraw();
    void OnEvent();
    
    bool CheckSelect();
    
    void UpdateTexture();
    
    virtual void OnInspector() {};
    virtual void ComputeTexture() {};
    
private:
    ImU32 backgroundColor, activeColor, pointColor;
    
    bool oldMouseDown, canDrag, _deselect;
};

#endif // H_NODE_POINT