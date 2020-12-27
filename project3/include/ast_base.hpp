#ifndef AST_BASE_HPP
#define AST_BASE_HPP
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "ast.hpp"
#include "spltype.hpp"
#include "tac.hpp"
#include "util.hpp"
enum class Attr {
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
static std::string translate_token(Attr token) {
    if (token == Attr::LT)
        return "<";
    else if (token == Attr::LE)
        return "<=";
    else if (token == Attr::GT)
        return ">";
    else if (token == Attr::GE)
        return ">=";
    else if (token == Attr::NE)
        return "!=";
    else if (token == Attr::EQ)
        return "==";
    else if (token == Attr::PLUS)
        return "+";
    else if (token == Attr::MUL)
        return "*";
    else if (token == Attr::DIV)
        return "/";
    else if (token == Attr::MINUS)
        return "-";
};
static Operator token_to_opType(Attr token) {
    if (token == Attr::LT)
        return Operator::J_LT;
    else if (token == Attr::LE)
        return Operator::J_LE;
    else if (token == Attr::GT)
        return Operator::J_GT;
    else if (token == Attr::GE)
        return Operator::J_GE;
    else if (token == Attr::NE)
        return Operator::J_NE;
    else if (token == Attr::EQ)
        return Operator::J_EQ;
    else if (token == Attr::PLUS)
        return Operator::PLUS;
    else if (token == Attr::MUL)
        return Operator::MUL;
    else if (token == Attr::DIV)
        return Operator::DIV;
    else if (token == Attr::MINUS)
        return Operator::MINUS;
};
typedef std::variant<V, std::vector<V> > retvar;
class Base {
   public:
    Attr attr = Attr::INVALID;
    int indent = 2;
    virtual ~Base() {}

    virtual void print(int idt = 0) = 0;
    virtual void push(Base *_var) = 0;
    virtual Attr get_attr() { return attr; }
    virtual void set_attr(Attr attr) { this->attr = attr; };
    virtual Type *visit() {
        logger("not implemented");
        return nullptr;
    };
    virtual Type *visit(bool scope) {
        logger("not implemented scope visit");
        return nullptr;
    };
    virtual void translate(V &place){logger("not implemented1");throw(false);};
    virtual std::optional<retvar> translate() { logger("not implemented2");throw(false);return {}; };
    virtual void translate(Label lb_true, Label lb_false){logger("not implemented3");throw(false);};
};

#endif