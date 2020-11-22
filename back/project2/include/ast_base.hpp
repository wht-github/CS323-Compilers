#ifndef AST_BASE_HPP
#define AST_BASE_HPP
#include "ast.hpp"
#include "spltype.hpp"
#include "util.hpp"
#include <string>
#include <iostream>
#include <vector>
enum class Attr
{
    INVALID,
    ASSIGN,
    OR,
    AND,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    PLUS,
    MINUS,
    UNARYMINUS,
    MUL,
    DIV,
    NOT,
    FUNCCALL,
    ARRAY,
    INT,
    CHAR,
    FLOAT,
    ID,
    ARRAYINDEXING,
    MEMBER,
    PARENTHESIS,
    TYPE,
    GLOBALVAR,
    GLOBALFUNC,
    GLOBALSTRUCT
};

class Base
{
public:
    Attr attr = Attr::INVALID;
    const int indent = 2;
    virtual ~Base() {}

    virtual void print(int idt = 0) const = 0;
    virtual void push(const Base *_var) const = 0;
    virtual Attr get_attr() const { return attr; }
    virtual void set_attr(Attr attr) { this->attr = attr; };
    virtual Type *visit() 
    {
        logger("not implemented");
        return nullptr;
    };
    virtual Type *visit(bool scope)
    {
        logger("not implemented");
        return nullptr;
    };
};

#endif