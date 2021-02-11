#ifndef H_NODE_LERP
#define H_NODE_LERP

#include "node.h"

class LerpNode : public Node {
public:
    LerpNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
    float factor;
};

#endif // H_NODE_LERP