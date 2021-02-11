#ifndef H_NODE_TRANSFORM
#define H_NODE_TRANSFORM

#include "node.h"

class TransformNode : public Node {
public:
    TransformNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
    ImVec2 offset, scale;
    float rotation;
};

#endif // H_NODE_TRANSFORM