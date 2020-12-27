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
class Args : public Base {
   private:
    int lineno;

   public:
    std::vector<Base *> list;
    Args(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Args (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
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
                prterr(-1, lineno, "Args");
                exit(1);
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 1: {
                V t1 = icg.new_temp_variable();
                list[0]->translate(t1);
                return std::optional<retvar>{std::vector<V>(1, t1)};
            }
            case 3: {
                V t1 = icg.new_temp_variable();
                list[0]->translate(t1);
                std::vector<V> vec = std::get<std::vector<V> >(list[2]->translate().value());
                vec.push_back(t1);
                return std::optional<retvar>{vec};
            }
        }
    }
};
class Exp : public Base {
   private:
    // mutable
    int lineno;

   public:
    std::vector<Base *> list;
    Exp(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Exp (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
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
                    prterr(-1, lineno, "ExpFunC");
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
                    if (ptr1 != nullptr) {
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
    //cond tran
    virtual void translate(Label lb_true, Label lb_false) {
        switch (attr) {
            case Attr::NOT: {
                list[1]->translate(lb_false, lb_true);
                return;
            }
            case Attr::AND: {
                Label lb1 = icg.new_label();
                list[0]->translate(lb1, lb_false);
                Tac i = Tac(Operator::LABEL, lb1.m_labelName);
                icg.appendCode(i);
                list[2]->translate(lb_true, lb_false);
                return;
            }
            case Attr::OR: {
                Label lb1 = icg.new_label();
                list[0]->translate(lb_true, lb1);
                Tac i = Tac(Operator::LABEL, lb1.m_labelName);
                icg.appendCode(i);
                list[2]->translate(lb_true, lb_false);
                return;
            }

            case Attr::PARENTHESIS: {
                list[1]->translate(lb_true, lb_false);
                return;
            }

            case Attr::EQ:
            case Attr::NE:
            case Attr::GE:
            case Attr::GT:
            case Attr::LE:
            case Attr::LT: {
                std::string op = translate_token(attr);
                V t1 = icg.new_temp_variable();
                V t2 = icg.new_temp_variable();
                list[0]->translate(t1);
                list[2]->translate(t2);
                Tac i = Tac(op, t1, t2, lb_true);
                icg.appendCode(i);

                i = Tac(Operator::JUMP, lb_false);
                icg.appendCode(i);

                return;
            }

            default:
                break;
        }
    };
    // Exp tran
    virtual void translate(V &place) {
        switch (attr) {
            case Attr::INT: {
                int val = ((ValInt *)list[0])->val;
                Tac i = Tac(Operator::ASSI, val, place);
                icg.appendCode(i);
                return;
            };
            case Attr::ID: {
                std::string val = ((ValId *)list[0])->val;
                Tac i = Tac(Operator::ASSI, V(icg.get_v_count_from_dict(val)), place);
                icg.appendCode(i);
                return;
            };
            case Attr::UNARYMINUS: {
                V t = icg.new_temp_variable();
                list[1]->translate(t);
                Tac i = Tac(Operator::MINUS, 0, t, place);
                icg.appendCode(i);
                return;
            };
            case Attr::ASSIGN: {
                if (((Exp *)list[0])->list.size() == 1 && ((Exp *)list[0])->attr == Attr::ID) {
                    std::string val = ((ValId *)(((Exp *)list[0])->list[0]))->val;
                    V v = icg.new_variable(icg.m_variable_dict[val]);
                    list[2]->translate(v);
                    Tac i = Tac(Operator::ASSI, v, place);
                    icg.appendCode(i);
                    return;
                }
                return;
            }
            case Attr::PLUS:
            case Attr::MINUS:
            case Attr::MUL:
            case Attr::DIV: {
                V t1 = icg.new_temp_variable();
                V t2 = icg.new_temp_variable();
                list[0]->translate(t1);
                list[2]->translate(t2);
                Tac i = Tac(token_to_opType(attr), t1, t2, place);
                icg.appendCode(i);
                return;
            }
            case Attr::PARENTHESIS: {
                list[1]->translate(place);
                return;
            }
            case Attr::FUNCCALL: {
                if (list.size() == 3) {
                    if (((ValId *)list[0])->val == "read") {
                        Tac i = Tac(Operator::READ, place);
                        icg.appendCode(i);
                        return;
                    } else {
                        Tac i = Tac(Operator::CALL, ((ValId *)list[0])->val, place);
                        icg.appendCode(i);
                        return;
                    }
                } else {
                    if (((ValId *)list[0])->val == "write") {
                        V t1 = icg.new_temp_variable();
                        ((Args *)list[2])->list[0]->translate(t1);
                        Tac i = Tac(Operator::WRITE, t1);
                        icg.appendCode(i);
                        return;
                    } else {
                        std::vector<V> arg_list = std::get<std::vector<V> >(list[2]->translate().value());
                        for(int i = arg_list.size()-1; i >= 0; --i){
                            Tac ins = Tac(Operator::ARG, arg_list[i]);
                            icg.appendCode(ins);
                        }
                        Tac i = Tac(Operator::CALL, ((ValId *)list[0])->val, place);
                        icg.appendCode(i);
                        return;
                    }
                }
            }
            case Attr::NOT:
            case Attr::AND:
            case Attr::OR:
            case Attr::EQ:
            case Attr::NE:
            case Attr::GE:
            case Attr::GT:
            case Attr::LE:
            case Attr::LT:{
                Label lb1 = icg.new_label();
                Label lb2 = icg.new_label();
                Tac i = Tac(Operator::ASSI, 0, place);
                icg.appendCode(i);
                this->translate(lb1, lb2);

                i = Tac(Operator::LABEL, lb1.m_labelName);
                icg.appendCode(i);

                i = Tac(Operator::ASSI, 1, place);
                icg.appendCode(i);

                i = Tac(Operator::LABEL, lb2.m_labelName);
                icg.appendCode(i);
                return;
            }
        }
    }
};

#endif