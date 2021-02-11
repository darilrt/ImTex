#ifndef H_NODE_RAMP
#define H_NODE_RAMP

#include "node.h"

class RampNode : public Node {
public:
    RampNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_RAMP