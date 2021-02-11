#ifndef H_NODE_SHAPE
#define H_NODE_SHAPE

#include "node.h"

class ShapeNode : public Node {
public:
    ShapeNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
    ImVec2 offset;
    int32_t corners;
    float shapeStart, gradientStart, rotation;
};

#endif // H_NODE_SHAPE