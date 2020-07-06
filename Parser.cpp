#include "Parser.h"

#include <iostream>

std::string FUNCS[] = {"write", "writeln", "readln", "sin", "assert"};

void Parser::skipSpaces() {
    char c = in.peek();
    while (isspace(c)) {
        in.get(c);
        c = in.peek();
    }
}

char Parser::peek() {
    return in.peek();
}

char Parser::next() {
    char c;
    if (in.get(c)) {
        return c;
    } else {
        throw SyntaxError("Неожиданный конец файла");
    }
}

ParserNode Parser::readKeyword(const std::string& keyword) {
    std::string kw;
    char c = next();
    for (char kc : keyword) {
        kw += c;
        c = next();
    }
    if (keyword != kw) {
        throw SyntaxError("Ожидается ключевое слово " + keyword + ", получено " + kw);
    }
    skipSpaces();
    return ParserNode(keyword);
}

std::string Parser::parseAlNum() {
    std::string str;
    char c = peek();
    while (isalnum(c)) {
        str += c;
        next();
        c = peek();
    }
    skipSpaces();
    return str;
}

ParserNode Parser::parseId() {
    return ParserNode(parseAlNum());
}

ParserNode Parser::parseStr() {
    char begin = peek();
    if (begin != '\'') {
        throw SyntaxError("Ошибка чтения строки, ожидается символ \"'\"");
    }
    next();

    std::string str;
    char c = peek();
    while (c != '\'') {
        str += c;
        next();
        c = peek();
    }
    next();
    skipSpaces();
    return ParserNode(str);
}

ParserNode Parser::parseNum() {
    std::string num;
    char c = peek();

    while (isdigit(c) || c == '.') {
        num += c;
        next();
        c = peek();
    }
    skipSpaces();
    return ParserNode(num);
}

List<ParserNode> Parser::parseExp() {
    List<ParserNode> term = parseTerm();

    char c = peek();
    if (c == '+' || c == '-') {
        next();
        skipSpaces();
        List<ParserNode> exp;
        exp.append(ParserNode(term));
        exp.append(ParserNode(std::string(1, c)));
        exp.append(ParserNode(parseExp()));
        return exp;
    }

    return term;
}

List<ParserNode> Parser::parseTerm() {
    List<ParserNode> factor = parseFactor();

    char c = peek();
    if (c == '*' || c == '/') {
        next();
        skipSpaces();
        List<ParserNode> term;
        term.append(ParserNode(factor));
        term.append(ParserNode(std::string(1, c)));
        term.append(ParserNode(parseTerm()));

        return term;
    }

    return factor;
}

List<ParserNode> Parser::parseFactor() {
    List<ParserNode> factor;

    char c = peek();

    if (isalpha(c)) {
        factor.append(parseId());
        c = peek();
        if (c == '(') {
            factor.append(ParserNode(parseFunc()));
        }
    } else if (isdigit(c)) {
        factor.append(parseNum());
    } else if (c == '(') {
        next();
        skipSpaces();
        factor.append(ParserNode(")"));
        factor.append(ParserNode(parseExp()));
        c = next();
        skipSpaces();
        if (c != ')') {
            throw SyntaxError("Не найдена закрывающая скобка в <factor>");
        }
        factor.append(ParserNode("("));
    } else {
        throw SyntaxError("Ошибка парсинга <factor>, неожидаемый символ \"" + std::string(1, c) + "\"");
    }

    return factor;
}

List<ParserNode> Parser::parseAssign() {
    List<ParserNode> assign;

    std::string firstLexem = parseAlNum();
    assign.append(ParserNode(firstLexem));

    if (firstLexem == "var") {
        assign.append(parseId());
    }

    assign.append(readKeyword(":="));
    assign.append(ParserNode(parseExp()));

    return assign;
}

ParserNode Parser::parseBoolSign() {
    char c = peek();

    if (c == '=' || c == '!') {
        next();
        skipSpaces();
        c = next();
        skipSpaces();
        if (c != '=') {
            throw SyntaxError("Ошибка парсинга логического знака " + std::string(1, c) + "=");
        }
        return ParserNode(std::string(1, c) + "=");
    } else if (c == '<' || c == '>') {
        next();
        skipSpaces();
        c = peek();
        if (c == '=') {
            next();
            skipSpaces();
            return ParserNode(std::string(1, c) + "=");
        }
        return ParserNode(std::string(1, c));
    } else {
        throw SyntaxError("Неожиданный символ при парсинге логического знака " + std::string(1, c));
    }
}

List<ParserNode> Parser::parseFactArg() {
    List<ParserNode> factArg;

    if (peek() == '\'') {
        factArg.append(parseStr());
    } else {
        factArg.append(ParserNode(parseExp()));
    }

    return factArg;
}

List<ParserNode> Parser::parseFactArgs() {
    List<ParserNode> factArgs;

    factArgs.append(ParserNode(parseFactArg()));

    char c = peek();
    while (c == ',') {
        next();
        skipSpaces();
        factArgs.append(ParserNode(parseFactArg()));
        c = peek();
    }

    return factArgs;
}

List<ParserNode> Parser::parseBoolExp() {
    List<ParserNode> boolExp;

    boolExp.append(ParserNode(parseFactArg()));
    boolExp.append(parseBoolSign());
    boolExp.append(ParserNode(parseFactArg()));

    return boolExp;
}

List<ParserNode> Parser::parseFunc() {
    List<ParserNode> func;

    func.append(parseId());

    char begin, end;

    begin = next();
    skipSpaces();

    ParserNode factArgs = ParserNode(parseFactArgs());

    end = next();
    skipSpaces();

    if (begin != '(' or end != ')') {
        throw SyntaxError("Ожидаются фактические аргументы функции в скобках");
    }

    func.append(ParserNode(")"));
    func.append(factArgs);
    func.append(ParserNode("("));

    return func;
}

List<ParserNode> Parser::parseConst() {
    List<ParserNode> cst;

    cst.append(parseId());

    char c = next();
    skipSpaces();

    if (c != '=') {
        throw SyntaxError("Пропущен знак \"=\" при парсинге констант");
    }

    cst.append(parseNum());

    return cst;
}

List<ParserNode> Parser::parseIdList() {
    List<ParserNode> idList;

    idList.append(parseId());

    char c = peek();
    while (c == ',') {
        next();
        skipSpaces();
        idList.append(parseId());
        c = peek();
    }

    return idList;
}

ParserNode Parser::parseType() {
    char c = peek();

    if (c == 'r') {
        return readKeyword("real");
    } else {
        throw SyntaxError("Неизвестный тип переменных");
    }
}

List<ParserNode> Parser::parseDec() {
    List<ParserNode> dec;

    dec.append(ParserNode(parseIdList()));

    char c = next();
    skipSpaces();
    if (c != ':') {
        throw SyntaxError("Ошибка в определении типа переменных, ожидается символ \":\"");
    }

    dec.append(parseType());

    return dec;
}

List<ParserNode> Parser::parseDecList() {
    List<ParserNode> decList;

    decList.append(ParserNode(parseDec()));

    char c = peek();
    while (c == ',') {
        next();
        skipSpaces();
        decList.append(ParserNode(parseDec()));
        c = peek();
    }

    return decList;
}

List<ParserNode> Parser::parseStmt() {
    List<ParserNode> stmt;

    std::string lexem = parseAlNum();

    if (lexem == "if") {
        stmt.append(ParserNode(parseIf()));
        return stmt;
    } else if (lexem == "while") {
        stmt.append(ParserNode(parseWhile()));
        return stmt;
    }

    bool fn = false;
    for (const std::string& f : FUNCS) {
        if (lexem == f) {
            fn = true;
            break;
        }
    }

    if (fn) {
        List<ParserNode> func;

        func.append(ParserNode(lexem));

        char begin, end;

        begin = next();
        skipSpaces();

        List<ParserNode> innerStmt;
        ParserNode factArg = ParserNode(parseFactArg());
        innerStmt.append(factArg);

        char c = peek();
        if (c == ',') {
            while (c == ',') {
                next();
                skipSpaces();
                innerStmt.append(ParserNode(parseFactArg()));
                c = peek();
            }
        } else if (c == '<' || c == '>' || c == '!' || c == '='){
            innerStmt.append(parseBoolSign());
            innerStmt.append(ParserNode(parseFactArg()));
        }

        end = next();
        skipSpaces();

        if (begin != '(' or end != ')') {
            throw SyntaxError("Ожидаются фактические аргументы функции в скобках");
        }

        func.append(ParserNode(")"));
        func.append(ParserNode(innerStmt));
        func.append(ParserNode("("));

        stmt.append(ParserNode(func));
    } else {
        List<ParserNode> assign;

        assign.append(ParserNode(lexem));

        if (lexem == "var") {
            assign.append(parseId());
        }

        assign.append(readKeyword(":="));
        assign.append(ParserNode(parseExp()));

        stmt.append(ParserNode(assign));
    }

    return stmt;
}

List<ParserNode> Parser::parseStmtList() {
    List<ParserNode> stmtList;

    stmtList.append(ParserNode(parseStmt()));

    char c = peek();
    while (c == ';') {
        next();
        skipSpaces();
        stmtList.append(ParserNode(parseStmt()));
        c = peek();
    }

    return stmtList;
}

List<ParserNode> Parser::parseIf() {
    List<ParserNode> ifStmt;

    ifStmt.append(ParserNode("if"));
    ifStmt.append(ParserNode(parseBoolExp()));
    ifStmt.append(readKeyword("then"));
    ifStmt.append(ParserNode(parseBody()));
    ifStmt.append(readKeyword("else"));
    ifStmt.append(ParserNode(parseBody()));

    return ifStmt;
}

List<ParserNode> Parser::parseWhile() {
    List<ParserNode> whileStmt;

    whileStmt.append(ParserNode("while"));
    whileStmt.append(ParserNode(parseBoolExp()));
    whileStmt.append(readKeyword("do"));
    whileStmt.append(ParserNode(parseBody()));

    return whileStmt;
}

List<ParserNode> Parser::parseBody() {
    List<ParserNode> body;

    char c = peek();
    if (c == 'b') {
        body.append(readKeyword("begin"));
        body.append(ParserNode(parseStmtList()));
        body.append(readKeyword("end"));
    } else {
        body.append(ParserNode(parseStmt()));
    }

    return body;
}

List<ParserNode> Parser::parseConstsList() {
    List<ParserNode> constsList;

    constsList.append(ParserNode(parseConst()));

    char c = peek();
    while (c == ',') {
        next();
        skipSpaces();
        constsList.append(parseId());
        c = peek();
    }

    return constsList;
}

List<ParserNode> Parser::parseProg() {
    List<ParserNode> prog;

    char c = peek();
    while (c == 'c' || c == 'v') {
        if (c == 'c') {
            prog.append(readKeyword("const"));
            prog.append(ParserNode(parseConstsList()));
            next();
            skipSpaces();
        } else {
            prog.append(readKeyword("var"));
            prog.append(ParserNode(parseDecList()));
        }
        c = peek();
    }

    prog.append(readKeyword("begin"));
    prog.append(ParserNode(parseStmtList()));
    prog.append(readKeyword("end."));

    return prog;
}


