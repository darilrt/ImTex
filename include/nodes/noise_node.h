#ifndef H_NODE_NOISE
#define H_NODE_NOISE

#include "node.h"

class NoiseNode : public Node {
public:
    NoiseNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_NOISE