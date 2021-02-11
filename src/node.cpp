#include <imgui.h>

#include "node.h"
#include "node_editor.h"
#include "project.h"

Node::Node() {
    title = "Node Title";
    isSelected = false;
    canDrag = false;
    
    backgroundColor = IM_COL32(60, 60, 70, 255);
    activeColor = IM_COL32(255, 255, 255, 255);
    pointColor = IM_COL32(75, 75, 175, 255);
    
    texture = new Texture2D(Project::texture_size.x, Project::texture_size.y);
}

Node::~Node() {
    delete texture;
    
    for (NodePoint* point : inPoints) {
        delete point;
    }
    inPoints.clear();
    
    for (NodePoint* point : outPoints) {
        delete point;
    }
    outPoints.clear();
}

void Node::OnDraw() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    ImVec2 pos(position.x + editor->offset.x + editor->position.x, position.y + editor->offset.y + editor->position.y);
    ImVec2 end(pos.x + 116, pos.y + 130);
    
    ImGui::SetCursorScreenPos(pos);
    ImGui::SetItemAllowOverlap();
    ImGui::InvisibleButton("node", ImVec2(116, 130), ImGuiButtonFlags_MouseButtonLeft);
    const bool isHovered = ImGui::IsItemHovered();
    
    draw_list->AddRectFilled(pos, end, backgroundColor, 5.0f);
    
    if (isSelected) {
        draw_list->AddRect(pos, end, activeColor, 5.0f);
    }
    
    ImVec2 title_pos(pos.x + 8, pos.y + 5);
    ImVec2 image_pos(pos.x + 8, pos.y + 22);
    ImVec2 image_end(image_pos.x + 100, image_pos.y + 100);
    
    draw_list->AddText(title_pos, IM_COL32(255, 255, 255, 255), &title[0]);
    draw_list->AddImage((void*)(intptr_t) texture->ptr, image_pos, image_end);
    
    {
        const float step = 100.0f / inPoints.size();
        const float radius = 6.5f;
        int i = 0;
        
        for (NodePoint* point : inPoints) {
            ImVec2 pointPos(pos.x, pos.y + 22 + i * step + (step / 2.0f));
            point->position = glm::vec2(pointPos.x, pointPos.y);
            
            draw_list->AddCircleFilled(pointPos, radius, pointColor);
            
            if (isHovered) {
                float left = ImGui::GetFontSize() * point->name.size() / 2.0f + radius;
                draw_list->AddText(ImVec2(pointPos.x - radius - left, pointPos.y - ImGui::GetFontSize() / 2.0f), activeColor, point->name.c_str());
            }
            
            i++;
        }
        
        const float step2 = 100.0f / outPoints.size();
        i = 0;
        for (NodePoint* point : outPoints) {
            ImVec2 pointPos(pos.x + 116, pos.y + 22 + i * step2 + (step2 / 2.0f));
            point->position = glm::vec2(pointPos.x, pointPos.y);
            
            draw_list->AddCircleFilled(pointPos, radius, pointColor);
            
            if (isHovered) {
                draw_list->AddText(ImVec2(pointPos.x + radius + radius, pointPos.y - ImGui::GetFontSize() / 2.0f), activeColor, point->name.c_str());
            }
            
            i++;
        }
    }
}

bool Node::CheckSelect() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos(position.x + editor->offset.x + editor->position.x, position.y + editor->offset.y + editor->position.y);
    
    ImGui::SetCursorScreenPos(pos);
    ImGui::SetItemAllowOverlap();
    ImGui::InvisibleButton("node", ImVec2(116, 130), ImGuiButtonFlags_MouseButtonLeft);
    const bool isHovered = ImGui::IsItemHovered();
    const bool isActive = ImGui::IsItemActive();
    
    bool mouseDown = (!oldMouseDown && ImGui::IsMouseDown(ImGuiMouseButton_Left));
    bool mouseUp = (oldMouseDown && !ImGui::IsMouseDown(ImGuiMouseButton_Left));
    oldMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    
    if (mouseDown || mouseUp) {
        const uint32_t size = inPoints.size() > outPoints.size() ? inPoints.size() : outPoints.size();
        const float radius2 = 6.5f * 6.5f;
        
        for (int i=0; i < size; i++) {
            if (i < inPoints.size()) {
                float dx = io.MousePos.x - inPoints[i]->position.x;
                float dy = io.MousePos.y - inPoints[i]->position.y;
                
                if (dx * dx + dy * dy <= radius2) {
                    if (mouseDown && inPoints[i]->connection) {
                        Connection* conn = inPoints[i]->connection;
                        inPoints[i]->connection = NULL;
                        editor->selectedOutPoint = conn->out;
                        editor->RemoveConnection(conn);
                        delete conn;
                    }
                    else {
                        editor->selectedInPoint = inPoints[i];
                    }
                    return true;
                }
            }
            
            if (i < outPoints.size()) {
                float dx = io.MousePos.x - outPoints[i]->position.x;
                float dy = io.MousePos.y - outPoints[i]->position.y;
                
                if (dx * dx + dy * dy <= radius2) {
                    editor->selectedOutPoint = outPoints[i];
                    return true;
                }
            }
        }
    }
    
    bool ret = false;
    if (isHovered) {
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
        _deselect = false;
        
        if (mouseDown) {
            canDrag = true;
            
            if (io.KeyShift) {
                if (isSelected) editor->DeselectNode(this);
                else editor->SelectNode(this);
            }
            else {
                if (isSelected) {
                    _deselect = true;
                }
                else {
                    editor->ClearSelection();
                }
                
                editor->SelectNode(this);
            }
            
            ret = true;
        }
        
        if (mouseUp && !io.KeyShift && _deselect) {
            editor->ClearSelection();
            editor->SelectNode(this);
            
            _deselect = false;
            ret = true;
        }
        
        if (!editor->isDragging && ImGui::IsMouseDragging(ImGuiMouseButton_Left, -1.0f) && canDrag && !io.KeyShift) {
            editor->isDragging = true;
            _deselect = false;
            ret = true;
        }
    }
    
    if (mouseUp) {canDrag = false;}
    
    return ret;
}

void Node::UpdateTexture() {
    ComputeTexture();
    
    for (NodePoint* out : outPoints) {
        for (Connection* conn : out->connections) {
            conn->in->node->UpdateTexture();
        }
    }
}