#ifndef H_NODE_POINT
#define H_NODE_POINT

#include <iostream>
#include <list>
#include <glm/glm.hpp>

class Node;
class Connection;

class NodePoint {
public:
    Connection* connection;
    std::list<Connection*> connections;
    Node* node;
    
    glm::vec2 position;
    
    std::string name;
    NodePoint(Node* parent, const std::string name);
    
private:
};

#endif // H_NODE_POINT