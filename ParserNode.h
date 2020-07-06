#pragma once

#include <string>
#include <utility>

#include "List.h"

class ParserNode {
private:
    std::string lexem;
    List<ParserNode> lst;
public:
    enum NodeType {EMPTY, LEAF, LINK};

    NodeType tp = EMPTY;

    explicit ParserNode(std::string lexem) : lexem(std::move(lexem)), tp(LEAF) {}
    explicit ParserNode(const List<ParserNode>& lst) : lst(lst), tp(LINK) {}
    virtual ~ParserNode() = default;

    NodeType nodeType() const;
    std::string getLexem();
    List<ParserNode> getList();
};
