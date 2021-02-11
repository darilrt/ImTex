#include "inspector_editor.h"
#include "node_editor.h"
#include "node.h"

InspectorEditor::InspectorEditor(NodeEditor* _nodeEditor) {
    open = true;
    title = "Inspector";
    
    nodeEditor = _nodeEditor;
}

void InspectorEditor::OnDraw() {
    
    if (nodeEditor->selectedNodes.size()) {
        nodeEditor->selectedNodes.back()->OnInspector();
    }
    
    ProcessEvents();
}

void InspectorEditor::ProcessEvents() {
    // ImGuiIO& io = ImGui::GetIO();
}