#ifndef H_IMTEX
#define H_IMTEX

#include <vector>

#include "material_preview_editor.h"
#include "node_editor.h"
#include "window.h"

class ImTex : public Window {
public:
    NodeEditor* nodeEditor;
    MaterialPreviewEditor* materialPreviewEditor;
    
    ImTex();
    ~ImTex();
    
    void PreDraw();
    void OnDraw();
    void OnEvent(SDL_Event &event);
    
private:
    void AddEditor(Editor* editor);
    
    std::vector<Editor*> editors;
};

#endif // H_IMTEX