#ifndef H_MATERIAL_PREVIEW_EDITOR
#define H_MATERIAL_PREVIEW_EDITOR

#include "node_editor.h"
#include "editor.h"
#include "mesh.h"
#include "shader.h"

class MaterialPreviewEditor : public Editor {
public:
    NodeEditor* nodeEditor;
    
    Mesh* meshSphere;
    Shader* pbrShader;
    Texture2D* render;
    uint32_t fbo, rbo;
    
    MaterialPreviewEditor(NodeEditor* nodeEditor);
    
    void PreRender();
    void OnDraw();
    void ProcessEvents();
};

#endif // H_MATERIAL_PREVIEW_EDITOR