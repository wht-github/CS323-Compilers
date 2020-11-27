#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
#include "ast_terminal.hpp"
// Declaration that holds a list of declarations
using std::find_if;
class Exp : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    Exp( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Exp (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (attr) {
            case Attr::INT: {
                auto ptr = new Type();
                ptr->primitive = Primitive::INT;
                ptr->category = Category::PRIMITIVE;
                ptr->initialized = true;
                ptr->name = {};
                return ptr;
            }
            case Attr::FLOAT: {
                auto ptr = new Type();
                ptr->primitive = Primitive::FLOAT;
                ptr->category = Category::PRIMITIVE;
                ptr->initialized = true;
                ptr->name = {};
                return ptr;
            }
            case Attr::CHAR: {
                auto ptr = new Type();
                ptr->primitive = Primitive::CHAR;
                ptr->category = Category::PRIMITIVE;
                ptr->initialized = true;
                ptr->name = {};
                return ptr;
            }
            case Attr::ID: {
                Type *ptr = stable.lookup(((ValId *)list[0])->val).value_or(nullptr);
                if (ptr == nullptr) {
                    //TODO: handle error
                    prterr(1, lineno, "variable is used without definition");
                    return nullptr;
                }
                ptr->lrv = LRV::L;
                return ptr;
            }
            case Attr::MEMBER: {
                auto exp_ptr = list[0]->visit();
                if (exp_ptr == nullptr) {
                    // error handle in list[0]->visit()
                    return nullptr;
                } else if (exp_ptr->cat_getter() != Category::STRUCT) {
                    //TODO: handle error
                    prterr(13, lineno, "accessing member of non-structure variable");
                    return nullptr;
                }
                auto str_ptr = (Struct *)exp_ptr;
                auto ptr = str_ptr->fields.find(((ValId *)list[2])->val);
                if (ptr == str_ptr->fields.end()) {
                    prterr(14, lineno, "accessing an undefined structure member");
                    return nullptr;
                }
                ptr->second->lrv = LRV::L;
                return ptr->second;
            }
            case Attr::ARRAYINDEXING: {
                auto exp1_ptr = list[0]->visit();
                auto exp2_ptr = list[2]->visit();
                if (exp1_ptr == nullptr || exp2_ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (exp1_ptr->cat_getter() != Category::ARRAY) {
                    //TODO: handle error
                    prterr(10, lineno, "applying indexing operator ([...]) on non-array type variables");
                    return nullptr;
                }
                if (!exp2_ptr->primitive.has_value() || exp2_ptr->primitive.value() != Primitive::INT) {
                    //TODO: handle error
                    prterr(12, lineno, "array indexing with non-integer type expression");
                    return nullptr;
                }
                //valid
                ((Array *)exp1_ptr)->type->lrv = LRV::L;
                return ((Array *)exp1_ptr)->type;
            };
            case Attr::FUNCCALL: {
                // std::cerr << "function" << std::endl;
                Type *ptr = stable.lookup(((ValId *)list[0])->val).value_or(nullptr);
                if (ptr == nullptr) {
                    // TODO: handle error, no val
                    prterr(2, lineno, "function is invoked without definition");
                    return nullptr;
                }
                if (ptr->cat_getter() != Category::FUNCTION) {
                    // TODO: handle error, not function
                    prterr(11, lineno, "applying function invocation operator (foo(...)) on non-function names");
                    return nullptr;
                }
                if (list.size() == 3) {
                    auto func_ptr = (Function *)ptr;
                    if (func_ptr->funcargs == nullptr) {
                        // ID LP RP
                        
                    } else {
                        // TODO: handle error, absent args
                        prterr(9, lineno, "the function’s arguments mismatch the declared parameters");
                    }
                    return func_ptr->return_type;

                } else if (list.size() == 4) {
                    auto func_ptr = (Function *)ptr;
                    if (type_equal(func_ptr->funcargs, list[2]->visit())) {
                        // Initial?
                        
                    } else {
                        prterr(9, lineno, "the function’s arguments mismatch the declared parameters");
                    }
                    return func_ptr->return_type;
                } else {
                    prterr(-1,lineno,"ExpFunC");
                    exit(1);
                }
            }
            case Attr::NOT: {
                auto ptr = list[1]->visit();
                if (ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (ptr->primitive.has_value() && ptr->primitive.value() == Primitive::INT) {
                    auto bool_type = new Type();
                    bool_type->category = Category::BOOLEAN;
                    //????????
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::AND: {
                auto ptr1 = list[0]->visit();
                auto ptr2 = list[2]->visit();
                if (ptr1 == nullptr || ptr2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((ptr1->primitive.has_value() && ptr1->primitive.value() == Primitive::INT) && (ptr2->primitive.has_value() && ptr2->primitive.value() == Primitive::INT)) {
                    auto bool_type = new Type();
                    bool_type->category = Category::BOOLEAN;
                    //????????
                    return ptr1;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::OR: {
                auto ptr1 = list[0]->visit();
                auto ptr2 = list[2]->visit();
                if (ptr1 == nullptr || ptr2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((ptr1->primitive.has_value() && ptr1->primitive.value() == Primitive::INT) && (ptr2->primitive.has_value() && ptr2->primitive.value() == Primitive::INT)) {
                    auto bool_type = new Type();
                    bool_type->category = Category::BOOLEAN;
                    //????????
                    return ptr1;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::UNARYMINUS: {
                auto ptr = list[1]->visit();
                if (ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (ptr->primitive.has_value() && (ptr->primitive.value() == Primitive::INT || ptr->primitive.value() == Primitive::FLOAT)) {
                    
                    return ptr;
                } else {
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::PARENTHESIS: {
                return list[1]->visit();
            }
            case Attr::PLUS: {
                auto arg1_ptr = list[0]->visit();
                auto arg2_ptr = list[2]->visit();
                if (arg1_ptr == nullptr || arg2_ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((arg1_ptr->primitive.has_value() && (arg1_ptr->primitive.value() == Primitive::INT || arg1_ptr->primitive.value() == Primitive::FLOAT)) && (arg2_ptr->primitive.has_value() && (arg2_ptr->primitive.value() == Primitive::INT || arg2_ptr->primitive.value() == Primitive::FLOAT))) {
                    if (type_equal(arg1_ptr, arg2_ptr)) {
                        return arg1_ptr;
                    } else {
                        prterr(7, lineno, "unmatching operands");
                        return nullptr;
                    }
                } else {
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::MINUS: {
                auto arg1_ptr = list[0]->visit();
                auto arg2_ptr = list[2]->visit();
                if (arg1_ptr == nullptr || arg2_ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((arg1_ptr->primitive.has_value() && (arg1_ptr->primitive.value() == Primitive::INT || arg1_ptr->primitive.value() == Primitive::FLOAT)) && (arg2_ptr->primitive.has_value() && (arg2_ptr->primitive.value() == Primitive::INT || arg2_ptr->primitive.value() == Primitive::FLOAT))) {
                    if (type_equal(arg1_ptr, arg2_ptr)) {
                        return arg1_ptr;
                    } else {
                        prterr(7, lineno, "unmatching operands");
                        return nullptr;
                    }
                } else {
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::MUL: {
                auto arg1_ptr = list[0]->visit();
                auto arg2_ptr = list[2]->visit();
                if (arg1_ptr == nullptr || arg2_ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((arg1_ptr->primitive.has_value() && (arg1_ptr->primitive.value() == Primitive::INT || arg1_ptr->primitive.value() == Primitive::FLOAT)) && (arg2_ptr->primitive.has_value() && (arg2_ptr->primitive.value() == Primitive::INT || arg2_ptr->primitive.value() == Primitive::FLOAT))) {
                    if (type_equal(arg1_ptr, arg2_ptr)) {
                        return arg1_ptr;
                    } else {
                        prterr(7, lineno, "unmatching operands");
                        return nullptr;
                    }
                } else {
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::DIV: {
                auto arg1_ptr = list[0]->visit();
                auto arg2_ptr = list[2]->visit();
                if (arg1_ptr == nullptr || arg2_ptr == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if ((arg1_ptr->primitive.has_value() && (arg1_ptr->primitive.value() == Primitive::INT || arg1_ptr->primitive.value() == Primitive::FLOAT)) && (arg2_ptr->primitive.has_value() && (arg2_ptr->primitive.value() == Primitive::INT || arg2_ptr->primitive.value() == Primitive::FLOAT))) {
                    if (type_equal(arg1_ptr, arg2_ptr)) {
                        return arg1_ptr;
                    } else {
                        prterr(7, lineno, "unmatching operands");
                        return nullptr;
                    }
                } else {
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::EQ: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::NE: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::GE: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::GT: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::LE: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::LT: {
                auto arg1 = list[0]->visit();
                auto arg2 = list[2]->visit();
                if (arg1 == nullptr || arg2 == nullptr) {
                    //TODO: handle error
                    return nullptr;
                }
                if (type_equal(arg1, arg2)) {
                    auto ptr = new Type();
                    ptr->primitive = Primitive::INT;
                    ptr->category = Category::PRIMITIVE;
                    ptr->initialized = true;
                    ptr->name = {};
                    return ptr;
                } else {
                    //TODO: handle error
                    prterr(7, lineno, "unmatching operands");
                    return nullptr;
                }
            }
            case Attr::ASSIGN: {
                auto ptr1 = list[0]->visit();
                auto ptr2 = list[2]->visit();
                if (ptr1 == nullptr || ptr2 == nullptr) {
                    //TODO: handle error
                    if (ptr1 != nullptr && ptr1->lrv == LRV::R) {
                        //TODO: handle error
                        prterr(6, lineno, "rvalue on the left side of assignment operator");
                        return nullptr;
                    }
                    if(ptr1!=nullptr) {
                        return ptr1;
                    }
                    return nullptr;
                }
                if (ptr1->lrv == LRV::R) {
                    prterr(6, lineno, "rvalue on the left side of assignment operator");
                    return nullptr;
                }
                if (type_equal(ptr1, ptr2)) {
                    //valid
                    return ptr1;
                } else {
                    //TODO: handle error
                    prterr(5, lineno, "unmatching types on both sides of assignment operator (=)");
                    return nullptr;
                }
            }
            default:
                break;
        }
        return nullptr;
    }
};
class Args : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    Args( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Args (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }

    virtual Type *visit() {
        switch (list.size()) {
            case 1: {
                Tuple *ptr = new Tuple();
                ptr->args.push_back(list[0]->visit());
                return ptr;
            }
            case 3: {
                Tuple *ptr = (Tuple *)(list[2]->visit());
                ptr->args.push_back(list[0]->visit());
                return ptr;
            }
            default: {
                prterr(-1,lineno,"Args");
                exit(1);
            }
        }
    }
};
#endif