#include "node_point.h"
#include "node_editor.h"

NodePoint::NodePoint(Node* parent, const std::string _name) {
    name = _name;
    node = parent;
    connection = NULL;
}
