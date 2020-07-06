#pragma once

#include <istream>
#include <string>

#include "List.h"
#include "ParserNode.h"
#include "ParserExceptions.h"

/*
id - идентификатор (имя переменной)
num - число

<prog> ::= {CONST <consts_list> } {VAR <dec_list> } BEGIN <stmt_list> END.
<consts_list> ::= <const> { , <consts_list> }
<assign> ::= {VAR} id := <exp>
<exp> ::= <term> | <term> + <exp> | <term> - <exp>
<term> ::= <factor> | <factor> * <term> | <factor> / <term>
<factor> ::= id | num | (<exp>) | <func>
<bool_sign> ::= < | > | == | != | <= | >=
<fact_arg> ::= str | <exp>
<fact_args> ::= <fact_arg> {, <fact_arg> }
<bool_exp> ::= <fact_arg> <bool_sign> <fact_arg>
<func> ::= id(<fact_args>) | id(<bool_exp>)
<const> ::= id = num
<id_list> ::= id { , <id> }
<type> ::= REAL
<dec> ::= <id_list> : <type>
<dec_list> ::= <dec> { , <dec> }
<stmt> ::= <if> | <while> | <assign> | <func>
<stmt_list> ::= <stmt> { ; <stmt> }
<if> ::= IF <bool_exp> THEN <body> ELSE <body>
<while> ::= WHILE <bool_exp> DO <body>
<body> ::= <stmt> | BEGIN <stmt_list> END
*/

class Parser {
private:
    std::istream & in;

    void skipSpaces();
    char peek();
    char next();
    ParserNode readKeyword(const std::string& keyword);

    std::string parseAlNum();

public:
    explicit Parser(std::istream & in) : in(in) {}
    virtual ~Parser() = default;

    ParserNode parseId();
    ParserNode parseNum();
    ParserNode parseStr();
    ParserNode parseBoolSign();
    ParserNode parseType();

    List<ParserNode> parseExp();
    List<ParserNode> parseTerm();
    List<ParserNode> parseFactor();
    List<ParserNode> parseAssign();
    List<ParserNode> parseFactArg();
    List<ParserNode> parseFactArgs();
    List<ParserNode> parseBoolExp();
    List<ParserNode> parseFunc();
    List<ParserNode> parseConst();
    List<ParserNode> parseIdList();
    List<ParserNode> parseDec();
    List<ParserNode> parseDecList();
    List<ParserNode> parseStmt();
    List<ParserNode> parseStmtList();
    List<ParserNode> parseIf();
    List<ParserNode> parseWhile();
    List<ParserNode> parseBody();
    List<ParserNode> parseConstsList();
    List<ParserNode> parseProg();
};









