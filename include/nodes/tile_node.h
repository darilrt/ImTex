#ifndef H_NODE_TILE
#define H_NODE_TILE

#include "node.h"

class TileNode : public Node {
public:
    TileNode();
    
    void OnInspector();
    void ComputeTexture();
    
private:
};

#endif // H_NODE_TILE