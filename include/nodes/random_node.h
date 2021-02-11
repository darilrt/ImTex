#ifndef H_NODE_RANDOM
#define H_NODE_RANDOM

#include "node.h"

class RandomNode : public Node {
public:
    RandomNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
    
};

#endif // H_NODE_RANDOM