#include <algorithm>
#include <cmath>
#include <list>

#include "node_editor.h"
#include "node_point.h"
#include "connection.h"

#include "nodes.h"
#include "project.h"

NodeEditor::NodeEditor() {
    open = true;
    title = "Node Editor";
    isDragging = false;
    
    backgroundColor = IM_COL32(25, 25, 30, 255);
    borderColor = IM_COL32(200, 200, 200, 40);
    gridColor = IM_COL32(150, 150, 150, 40);
    
    selectedInPoint = NULL;
    selectedOutPoint = NULL;
    
    Project::texture_size = glm::vec2(512, 512);
    
    materialNode = AddNode<MaterialNode>();
    
    materialNode->position = glm::vec2(0, 0);
}

void NodeEditor::OnDraw() {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    position = glm::vec2(pos.x, pos.y);
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
    canvas_size = glm::vec2(canvas_sz.x, canvas_sz.y);
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(pos.x + canvas_sz.x, pos.y + canvas_sz.y);
    
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(pos, canvas_p1, backgroundColor);
    draw_list->AddRect(pos, canvas_p1, borderColor);
    
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonMask_);
    const bool isHovered = ImGui::IsItemHovered();
    const bool isActive = ImGui::IsItemActive();
    
    if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)) {
        offset.x += io.MouseDelta.x;
        offset.y += io.MouseDelta.y;
        
        if (isDragging) {
            for (Node* node : selectedNodes) {
                node->position.x -= io.MouseDelta.x;
                node->position.y -= io.MouseDelta.y;
            }
        }
    }
    
    if (isHovered && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        ImGui::OpenPopupOnItemClick("context");
    
    if (ImGui::BeginPopup("context")) {
        static char find_node[128] = "";
        
        // if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            // ImGui::SetKeyboardFocusHere(0);
        
        // ImGui::InputText("", find_node, IM_ARRAYSIZE(find_node));
        
        // if (find_node[0] != '\0') {
            // ImGui::Text("No node found...");
        // }
        // else {
            if (ImGui::BeginMenu("Generator")) {
                if (ImGui::MenuItem("Shape", NULL, false, true)) {AddNode<ShapeNode>();}
                // if (ImGui::MenuItem("Random", NULL, false, true)) {AddNode<RandomNode>();}
                if (ImGui::MenuItem("Noise", NULL, false, true)) {AddNode<NoiseNode>();}
                if (ImGui::MenuItem("Color", NULL, false, true)) {AddNode<ColorNode>();}
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Operation")) {
                if (ImGui::MenuItem("Lerp", NULL, false, true)) {AddNode<LerpNode>();}
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Filter")) {
                if (ImGui::MenuItem("Transform", NULL, false, true)) {AddNode<TransformNode>();}
                if (ImGui::MenuItem("Ramp", NULL, false, true)) {AddNode<RampNode>();}
                if (ImGui::MenuItem("Normal", NULL, false, true)) {AddNode<NormalNode>();}
                if (ImGui::MenuItem("Blur", NULL, false, true)) {AddNode<BlurNode>();}
                if (ImGui::MenuItem("Tile", NULL, false, true)) {AddNode<TileNode>();}
                
                ImGui::EndMenu();
            }
        // }
        ImGui::EndPopup();
    }
    
    draw_list->PushClipRect(pos, canvas_p1, true);
    
    {
        const float GRID_STEP = 16.0f;
        for (float x = fmodf(offset.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(pos.x + x, pos.y), ImVec2(pos.x + x, canvas_p1.y), gridColor);
        
        for (float y = fmodf(offset.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(pos.x, pos.y + y), ImVec2(canvas_p1.x, pos.y + y), gridColor);
    }
    
    DrawConnections();
    DrawConnectionLine();
    DrawNodes();
    
    bool mouseDown = (!oldMouseDown && ImGui::IsMouseDown(ImGuiMouseButton_Left));
    oldMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    
    bool isBreaked = false;
    
    for (Node* node : nodes) {
        if (node->CheckSelect()) {
            isBreaked = true;
            break;
        }
    }
    // for (std::list<Node*>::const_reverse_iterator iter(nodes.rbegin()), iter_end(nodes.rend());
            // iter != iter_end;
            // ++iter) {
        // if ((*iter)->CheckSelect()) {
            // isBreaked = true;
            // break;
        // }
    // }
    
    if (isHovered && !isBreaked && mouseDown && !ImGui::GetIO().KeyShift) {
        ClearSelection();
    }
    
    if (isDragging) {
        ImVec2 mouseDrag = io.MouseDelta;
        
        for (Node* node : selectedNodes) {
            node->position.x += mouseDrag.x;
            node->position.y += mouseDrag.y;
        }
        
        if (isHovered && io.MouseDown[ImGuiMouseButton_Left]) {
            isDragging = false;
        }
    }
    
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        if (selectedInPoint && selectedOutPoint && selectedInPoint->node != selectedOutPoint->node) {
            if (selectedInPoint->connection != NULL) {
                Connection* conn = selectedInPoint->connection;
                selectedInPoint->connection = NULL;
                conn->out->connections.remove_if([&conn](Connection* c){
                    return c == conn;
                });
                RemoveConnection(conn);
                delete conn;
            }
            
            connections.push_back(new Connection(selectedInPoint, selectedOutPoint));
            selectedInPoint->node->UpdateTexture();
        }
        
        selectedInPoint = NULL;
        selectedOutPoint = NULL;
    }
    
    draw_list->PopClipRect();
    
    ProcessEvents();
}

void NodeEditor::ProcessEvents() {
    ImGuiIO& io = ImGui::GetIO();
    
    if (io.KeysDown[0xA]) { // G
        if (selectedNodes.size())
            isDragging = true;
    }
    if (io.KeysDown[0x4C] || io.KeysDown[0x1B]) { // Delete || X
        RemoveSelectedNodes();
    }
}

void NodeEditor::DrawNodes() {
    for (Node* node : nodes) {
        node->OnDraw();
    }
}

void NodeEditor::DrawConnections() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    for (Connection* conn : connections) {
        const float d = 100;
        draw_list->AddBezierCubic(
            ImVec2(conn->in->position.x, conn->in->position.y),
            ImVec2(conn->in->position.x - d, conn->in->position.y),
            ImVec2(conn->out->position.x + d, conn->out->position.y),
            ImVec2(conn->out->position.x, conn->out->position.y),
            IM_COL32(255, 255, 255, 255),
            2.0f
        );
    }
}

void NodeEditor::DrawConnectionLine() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    if (selectedInPoint != NULL) {
        const float d = 100;
        draw_list->AddBezierCubic(
            ImVec2(selectedInPoint->position.x, selectedInPoint->position.y),
            ImVec2(selectedInPoint->position.x - d, selectedInPoint->position.y),
            ImVec2(io.MousePos.x + d, io.MousePos.y),
            ImVec2(io.MousePos.x, io.MousePos.y),
            IM_COL32(255, 255, 255, 255),
            2.0f
        );
    }
    
    if (selectedOutPoint != NULL) {
        const float d = 100;
        draw_list->AddBezierCubic(
            ImVec2(selectedOutPoint->position.x, selectedOutPoint->position.y),
            ImVec2(selectedOutPoint->position.x + d, selectedOutPoint->position.y),
            ImVec2(io.MousePos.x - d, io.MousePos.y),
            ImVec2(io.MousePos.x, io.MousePos.y),
            IM_COL32(255, 255, 255, 255),
            2.0f
        );
    }
}

void NodeEditor::DeselectNode(Node* node) {
    if (node->isSelected) {
        selectedNodes.remove_if([&node](Node* n) {
            return n == node;
        });
        node->isSelected = false;
    }
}

void NodeEditor::SelectNode(Node* node) {
    if (!node->isSelected) {
        selectedNodes.push_back(node);
        node->isSelected = true;
    }
}

void NodeEditor::ClearSelection() {
    for (Node* _node : selectedNodes) {
        _node->isSelected = false;
    }
    selectedNodes.clear();
}

void NodeEditor::RemoveConnection(Connection* connection) {
    if (connection == NULL) {
        return;
    }
    
    connections.remove_if([&connection](Connection* c) {
        return c == connection;
    });
    
    connection->in->connection = NULL;
    connection->in->connections.remove_if([&connection](Connection* c) {
        return c == connection;
    });
    
    connection->out->connection = NULL;
    connection->out->connections.remove_if([&connection](Connection* c) {
        return c == connection;
    });
    
    delete connection;
}

void NodeEditor::RemoveNode(Node* node) {
    for (NodePoint* point : node->inPoints) {
        RemoveConnection(point->connection);
        for (Connection* c : point->connections) {
            RemoveConnection(c);
        }
    }
    
    for (NodePoint* point : node->outPoints) {
        RemoveConnection(point->connection);
        for (Connection* c : point->connections) {
            RemoveConnection(c);
        }
    }
    
    nodes.remove_if([&node](Node* n) {
        return n == node;
    });
    
    delete node;
}

void NodeEditor::RemoveSelectedNodes() {
    for (Node* node : selectedNodes) {
        RemoveNode(node);
    }
    selectedNodes.clear();
}