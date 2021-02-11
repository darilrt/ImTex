#ifndef H_NODE_EDITOR
#define H_NODE_EDITOR

#include <vector>
#include <list>
#include <imgui.h>

#include "editor.h"
#include "node.h"
#include "nodes.h"
#include "connection.h"

class NodePoint;

class NodeEditor : public Editor {
public:
    MaterialNode* materialNode;
    
    NodePoint* selectedInPoint;
    NodePoint* selectedOutPoint;
    
    glm::vec2 position, offset, canvas_size;
    bool isDragging;
 
    std::list<Connection*> connections;
    std::list<Node*> nodes;
    std::list<Node*> selectedNodes;
    
    NodeEditor();
    void OnDraw();
    void ProcessEvents();
    
    void DrawConnectionLine();
    void DrawConnections();
    void DrawNodes();
    
    void RemoveConnection(Connection* connection);
    
    void RemoveNode(Node* node);
    void SelectNode(Node* node);
    void DeselectNode(Node* node);
    void ClearSelection();
    void RemoveSelectedNodes();
    
    template<typename T, typename ... A>
    T* AddNode(A ... params) {
        T* node = new T(params...);
        
        node->editor = this;
        
        ImVec2 p = ImGui::GetMousePos();
        node->position = {
            p.x - position.x - offset.x,
            p.y - position.y - offset.y
        };
        
        nodes.push_back(node);
        
        node->UpdateTexture();
        
        return node;
    }
    
private:
    bool oldMouseDown;
    ImU32 backgroundColor, borderColor, gridColor;
};

#endif // H_NODE_EDITOR