#ifndef AST_SPECIFIER_HPP
#define AST_SPECIFIER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
#include "ast_terminal.hpp"
// Declaration that holds a list of declarations

class Specifier : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    Specifier(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Specifier (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (attr) {
            case Attr::TYPE: {
                auto ptr = new Type();
                ptr->category = Category::PRIMITIVE;
                std::string val = ((ValType *)list[0])->val;
                if (val == "int") {
                    ptr->primitive = Primitive::INT;
                } else if (val == "float") {
                    ptr->primitive = Primitive::FLOAT;
                } else if (val == "char") {
                    ptr->primitive = Primitive::CHAR;
                } else {
                    prterr(-1, lineno, "Unknown Primitives");
                    exit(-1);
                }
                return ptr;
            }
            default: {
                return list[0]->visit();
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (attr) {
            case Attr::TYPE: {
                // std::cout << "Only implement int in Specifier" << std::endl;
                return {};
            }
            default: {
                return list[0]->translate();
            }
        }
    }
};
class StructSpecifier : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    StructSpecifier(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "StructSpecifier (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (list.size()) {
            case 2: {
                Type *ptr = stable.lookup(((ValId *)list[1])->val).value_or(nullptr);
                if (ptr == nullptr) {
                    prterr(1, lineno, "variable is used without definition");
                    return nullptr;
                } else {
                    return ptr;
                }
            }
            case 5: {
                Struct *ptr = new Struct();
                ptr->name = ((ValId *)list[1])->val;
                stable.push();
                list[3]->visit();
                ptr->fields.insert(stable.get_top()->begin(), stable.get_top()->end());
                stable.pop();
                return ptr;
            }
        }
    }
    virtual std::optional<retvar> translate() {
        std::cout << "Not implemented TAC for StructSpecifier" << std::endl;
        exit(1);
    }
};

#endif