#ifndef AST_STMT_HPP
#define AST_STMT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
// Declaration that holds a list of declarations

class CompSt : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    CompSt( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "CompSt (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        stable.push();
        list[1]->visit();
        auto tmp = list[2]->visit();
        stable.pop();
        if (((Tuple *)tmp)->args.size() == 0) {
            return nullptr;
        }
        return tmp;
    }
    virtual Type *visit(bool scope) {
        list[1]->visit();
        auto tmp = list[2]->visit();
        if (((Tuple *)tmp)->args.size() == 0) {
            return nullptr;
        }
        return tmp;
    }
};
class StmtList : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    StmtList(int _lineno) : lineno(_lineno) {
    }
    StmtList( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "StmtList (" << lineno << ")" << std::endl;
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
                return nullptr;
            }
            case 2: {
                auto tmp = list[0]->visit();
                auto ptr = (Tuple *)(list[1]->visit());
                if (ptr == nullptr) {
                    ptr = new Tuple();
                }
                if (tmp != nullptr)
                    ptr->args.push_back(tmp);
                return ptr;
            }
            default:
                exit(1);
        }
    }
};

class Stmt : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    Stmt( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Stmt (" << lineno << ")" << std::endl;
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
};

#endif