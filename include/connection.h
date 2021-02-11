#ifndef H_CONNECTION
#define H_CONNECTION

#include "node_point.h"

class Connection {
public:
    NodePoint *in, *out;
    
    Connection(NodePoint* _in, NodePoint* _out) {
        in = _in;
        out = _out;
        
        in->connection = this;
        out->connections.push_back(this);
    }
};

#endif // H_CONNECTION