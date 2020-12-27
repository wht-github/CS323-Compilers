#ifndef AST_STMT_HPP
#define AST_STMT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
// Declaration that holds a list of declarations

class CompSt : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    CompSt(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "CompSt (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
        list.push_back(_node);
    }
    virtual Type *visit() {
        stable.push();
        list[1]->visit();
        auto tmp = list[2]->visit();

        stable.pop();
        if (tmp == nullptr) return nullptr;
        if (((Tuple *)tmp)->args.size() == 0) {
            return nullptr;
        }
        return tmp;
    }
    virtual Type *visit(bool scope) {
        list[1]->visit();
        auto tmp = (Tuple *)(list[2]->visit());
        if (tmp == nullptr || tmp->args.size() == 0) {
            return nullptr;
        }
        return tmp;
    }
    virtual std::optional<retvar> translate() {
        list[1]->translate();
        list[2]->translate();
        return {};
    }
};
class StmtList : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    StmtList(int _lineno) : lineno(_lineno) {
    }
    StmtList(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "StmtList (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (list.size()) {
            case 0: {
                return nullptr;
            }
            case 2: {
                auto tmp = list[0]->visit();
                auto ptr = (Tuple *)(list[1]->visit());
                if (tmp == nullptr) {
                    return ptr;
                }
                if (ptr == nullptr) {
                    ptr = new Tuple();
                    ptr->args.push_back(tmp);
                    return ptr;
                }
                ptr->args.push_back(tmp);
                return ptr;
            }
            default:
                prterr(list.size(), lineno, "StmtList");
                exit(1);
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 0: {
                return {};
            }
            case 2: {
                list[0]->translate();
                list[1]->translate();
                return  {};
            }
            default:
                prterr(list.size(), lineno, "StmtList");
                exit(1);
        }
    }
};

class Stmt : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    Stmt(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Stmt (" << lineno << ")" << std::endl;
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
                return list[0]->visit();
            }
            case 2: {
                list[0]->visit();
                return nullptr;
            }
            case 3: {
                return list[1]->visit();
            }
            case 5: {
                list[2]->visit();
                return list[4]->visit();
            }
            case 7: {
                list[2]->visit();
                auto t1 = list[4]->visit();
                auto t2 = list[6]->visit();
                auto ptr = new Tuple();
                if (t1 != nullptr)
                    ptr->args.push_back(t1);
                if (t2 != nullptr)
                    ptr->args.push_back(t2);
                if (ptr->args.size() == 0)
                    return nullptr;
                return ptr;
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 1: {
                list[0]->translate();
                return {};
            }
            case 2: {
                V t = icg.new_temp_variable();
                list[0]->translate(t);
                return {};
            }
            case 3: {
                V t = icg.new_temp_variable();
                list[1]->translate(t);
                Tac i = Tac(Operator::RET, t);
                icg.appendCode(i);
                return {};
            }
            case 5: {
                Terminal *ptr = (Terminal *)(list[0]);
                if(ptr->symbol == "WHILE"){
                    Label lb1 = icg.new_label();
                    Label lb2 = icg.new_label();
                    Label lb3 = icg.new_label();
                    Tac i = Tac(Operator::LABEL, lb1.m_labelName);
                    icg.appendCode(i);

                    list[2]->translate(lb2,lb3);

                    i = Tac(Operator::LABEL, lb2.m_labelName);
                    icg.appendCode(i);

                    list[4]->translate();
                    i = Tac(Operator::JUMP, lb1);
                    icg.appendCode(i);

                    i = Tac(Operator::LABEL, lb3.m_labelName);
                    icg.appendCode(i);
                    return {};
                } else if (ptr->symbol == "IF"){
                    Label lb1 = icg.new_label();
                    Label lb2 = icg.new_label();
                    list[2]->translate(lb1, lb2);

                    Tac i = Tac(Operator::LABEL, lb1.m_labelName);
                    icg.appendCode(i);

                    list[4]->translate();

                    i = Tac(Operator::LABEL, lb2.m_labelName);
                    icg.appendCode(i);
                    return {};
                }
                std::cout<<"error" <<std::endl;
                exit(1);
            }
            case 7: {
                Label lb1 = icg.new_label();
                Label lb2 = icg.new_label();
                Label lb3 = icg.new_label();

                list[2]->translate(lb1, lb2);

                Tac i = Tac(Operator::LABEL, lb1.m_labelName);
                icg.appendCode(i);

                list[4]->translate();

                i = Tac(Operator::JUMP, lb3);
                icg.appendCode(i);

                i = Tac(Operator::LABEL, lb2.m_labelName);
                icg.appendCode(i);

                list[6]->translate();

                i = Tac(Operator::LABEL, lb3.m_labelName);
                icg.appendCode(i);

                return {};
            }
        }
    }
};

#endif