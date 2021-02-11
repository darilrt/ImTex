#ifndef H_NODE_MATERIAL
#define H_NODE_MATERIAL

#include "node.h"

class MaterialNode : public Node {
public:
    MaterialNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_MATERIAL