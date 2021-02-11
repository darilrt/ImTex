#ifndef H_NODE_BLUR
#define H_NODE_BLUR

#include "node.h"

class BlurNode : public Node {
public:
    BlurNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_BLUR