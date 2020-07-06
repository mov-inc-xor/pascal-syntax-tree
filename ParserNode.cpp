#include "ParserNode.h"

ParserNode::NodeType ParserNode::nodeType() const {
    return tp;
}

std::string ParserNode::getLexem() {
    return lexem;
}

List<ParserNode> ParserNode::getList() {
    return lst;
}