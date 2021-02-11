#ifndef H_NODE_NORMAL
#define H_NODE_NORMAL

#include "node.h"

class NormalNode : public Node {
public:
    NormalNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_NORMAL