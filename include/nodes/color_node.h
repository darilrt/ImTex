#ifndef H_NODE_COLOR
#define H_NODE_COLOR

#include "node.h"

class ColorNode : public Node {
public:
    ColorNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
    ImVec4 color;
};

#endif // H_NODE_COLOR