#ifndef AST_EXTERNAL_HPP
#define AST_EXTERNAL_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
// Declaration that holds a list of declarations

class ExtDefList : public Base {
   private:
    mutable std::vector<const Base *> list;
    int lineno;

   public:
    ExtDefList(int _lineno) : lineno(_lineno) {
    }
    ExtDefList(const Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDefList (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(const Base *_node) const {
        list.push_back(_node);
    }
    virtual Type* visit(){
        switch(list.size()) {
            case 1:{
                return nullptr;
            }
            case 2:{
                list[0]->visit();
                return list[1]->visit();
            }
            default:exit(1);
        }
    }
};
class ExtDecList : public Base {
   private:
    mutable std::vector<const Base *> list;
    int lineno;

   public:
    ExtDecList(const Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDecList (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(const Base *_node) const {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (list.size()) {
            case 1: {
                auto ptr = new Tuple();
                ptr->args.push_back(list[0]->visit());
                return ptr;
            }
            case 3: {
                auto ptr2 = (Tuple *)(list[2]->visit());
                auto ptr1 = list[0]->visit();
                ptr2->args.push_back(ptr1);
                return ptr2;
            }
        }
    }
};
class ExtDef : public Base {
   private:
    mutable std::vector<const Base *> list;
    int lineno;

   public:
    ExtDef(const Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDef (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(const Base *_node) const {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (attr) {
            case Attr::GLOBALSTRUCT: {
                auto ptr = list[0]->visit();
                if (ptr->category == Category::PRIMITIVE) {
                    // return ptr;
                } else if (ptr->category == Category::STRUCT) {
                    if (!stable.lookup_top(ptr->name.value()).has_value()) {
                        stable.insert(ptr->name.value(), ptr);
                    } else {
                        prterr(15, lineno, "redefine the same structure type");
                    }
                } else {
                    prterr(-1, lineno, "?????");
                    exit(1);
                }
            }
            case Attr::GLOBALVAR: {
                auto spec = list[0]->visit();
                auto ptrs = (Tuple *)(list[1]->visit());
                for (int i = 0; i < ptrs->args.size(); ++i) {
                    auto tmp = ptrs->args[i];
                    bool flag_insert = true;
                    if (stable.lookup_top(tmp->varname.value()).has_value()) {
                        // already defined
                        prterr(3, lineno, "variable is redefined in the same scope");
                        flag_insert = false;
                    }
                    //type checking
                    if (tmp->category == Category::UNCERTAIN) {
                        if (flag_insert) {
                            stable.insert(tmp->varname.value(), spec);
                            flag_insert = false;
                        }
                    } else {
                        auto ttmp = tmp;
                        while (((Array *)ttmp)->type->category == Category::ARRAY) {
                            ttmp = ((Array *)ttmp)->type;
                        }

                        ((Array *)ttmp)->type = spec;
                    }
                    //use spec type no matter correct or not
                    if (flag_insert) {
                        stable.insert(tmp->varname.value(), tmp);
                    }
                }
                return nullptr;
            }
            case Attr::GLOBALFUNC: {
                auto spec = list[0]->visit();
                stable.push();
                auto fun = list[1]->visit();
                ((Function *)fun)->return_type = spec;
                Tuple *ret = (Tuple *)(list[2]->visit(false));
                stable.pop();
                stable.insert(fun->name.value(), fun);
                if (ret == nullptr) {
                    prterr(8, lineno, "the function’s return value type mismatches the declared type");
                    return fun;
                }
                if (!tuple_equal_to(spec, ret)) {
                    prterr(8, lineno, "the function’s return value type mismatches the declared type");
                    return fun;
                }
                return fun;
            }
            default:
                exit(1);
        }
    }
};
#endif