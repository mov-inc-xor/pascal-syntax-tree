#include <iostream>
#include <fstream>

#include "Parser.h"
#include "ParserExceptions.h"

void printTree(List<ParserNode> tree) {
    Node<ParserNode>* pn = tree.getHead();

    std::cout << "/";

    while (pn) {
        ParserNode::NodeType np = pn->getData().nodeType();

        std::cout << "->";

        switch (np) {
            case ParserNode::EMPTY:
                throw Exception("Ошибка печати синтаксического дерева, пустой ParserNode");
            case ParserNode::LEAF:
                std::cout << "\"" << pn->getData().getLexem() << "\"";
                break;
            case ParserNode::LINK:
                std::cout << "[";
                printTree(pn->getData().getList());
                std::cout << "]";
                break;
        }

        pn = pn->getNext();
    }
}

int main() {
    std::ifstream fin("/home/m5/CLionProjects/8/test.txt");

    if (!fin) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return -1;
    }

    Parser parser(fin);
    try {
        List<ParserNode> pn = parser.parseProg();
        printTree(pn);
    } catch (SyntaxError & se) {
        std::cerr << se.what() << std::endl;
    }

	fin.close();

	return 0;
}