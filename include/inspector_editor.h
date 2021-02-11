#ifndef H_INSPECTOR_EDITOR
#define H_INSPECTOR_EDITOR

#include "editor.h"

class NodeEditor;

class InspectorEditor : public Editor {
public:
    NodeEditor* nodeEditor;
    
    InspectorEditor(NodeEditor* nodeEditor);
    
    void OnDraw();
    void ProcessEvents();
};

#endif // H_INSPECTOR_EDITOR