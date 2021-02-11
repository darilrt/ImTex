#include "material_preview_editor.h"
#include "inspector_editor.h"
#include "node_editor.h"
#include "imtex.h"
#include "nodes.h"

ImTex::ImTex() : Window("ImTex", glm::vec2(1280, 720)) {
    AddEditor(nodeEditor = new NodeEditor());
    AddEditor(new InspectorEditor(nodeEditor));
    AddEditor(materialPreviewEditor = new MaterialPreviewEditor(nodeEditor));
}

ImTex::~ImTex() {
}

void ImTex::PreDraw() {
    materialPreviewEditor->PreRender();
}

void ImTex::OnDraw() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  { /* Do stuff */ }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Node Wditor")) { editors[0]->open = true; }
            if (ImGui::MenuItem("Inspector"))  { /* Do stuff */ }
            if (ImGui::MenuItem("Material Preview"))   { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    for (Editor* editor : editors) {
        if (editor->open) {
            ImGui::Begin(editor->title.c_str(), &editor->open);
            editor->OnDraw();
            ImGui::End();
        }
    }
}

void ImTex::OnEvent(SDL_Event &event) {
    
}

void ImTex::AddEditor(Editor* editor) {
    editors.push_back(editor);
}