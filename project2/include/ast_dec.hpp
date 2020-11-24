#ifndef AST_DEC_HPP
#define AST_DEC_HPP

#include <iostream>
#include <string>
#include <vector>
#include"symboltable.hpp"
#include "ast.hpp"
#include "ast_terminal.hpp"
// Declaration that holds a list of declarations

class VarDec : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    VarDec(int _lineno) : lineno(_lineno) {
    }

    VarDec( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarDec (" << lineno << ")" << std::endl;
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
                auto ptr = new Type();
                ptr->varname = ((ValId *)list[0])->val;
                ptr->category = Category::UNCERTAIN;
                return ptr;
            }
            case 4: {
                auto ptr = new Array();
                ptr->size = ((ValInt *)list[2])->val;
                ptr->type = list[0]->visit();
                ptr->varname = ptr->type->varname.value();
                return ptr;
            }
            default:
                exit(1);
        }
    }
};
class FunDec : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    FunDec( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "FunDec (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        switch (list.size()) {
            case 3: {
                auto ptr = new Function();
                ptr->name = ((ValId *)list[0])->val;
                return ptr;
            }
            case 4: {
                auto ptr = new Function();
                ptr->name = ((ValId *)list[0])->val;

                NTuple * tmp = (NTuple *)(list[2]->visit());
                Tuple * tmpargs = new Tuple();
                for(size_t i=0; i< tmp->nargs.size(); i++) {
                    stable.insert(tmp->nargs[i].first,tmp->nargs[i].second);
                    tmpargs->args.push_back(tmp->nargs[i].second);
                }
                ptr->funcargs = tmpargs;
                return ptr;
            }
        }
    }
};

class VarList : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    VarList( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarList (" << lineno << ")" << std::endl;
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
                NTuple *ptr = new NTuple();
                auto tmp = list[0]->visit();
                ptr->nargs.push_back(std::make_pair(tmp->varname.value(),tmp));
                return ptr;
            }
            case 3: {
                NTuple *ptr = (NTuple *)(list[2]->visit());
                auto tmp = list[0]->visit();
                ptr->nargs.push_back(std::make_pair(tmp->varname.value(),tmp));
                return ptr;
            }
            default: {
                exit(1);
            }
        }
    }
};

class ParamDec : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    ParamDec( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ParamDec (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        auto spec = list[0]->visit();
        auto tmp = list[1]->visit();
        //type checking
        if (tmp->category == Category::UNCERTAIN) {
            spec->varname = tmp->varname.value();
            return spec;
        } else {
            auto ttmp = tmp;
            while (((Array *)ttmp)->type->category == Category::ARRAY) {
                ttmp = ((Array *)ttmp)->type;
            }
            ((Array *)ttmp)->type = spec;
            return tmp;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
class DefList : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    DefList(int _lineno) : lineno(_lineno) {
    }

    DefList( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DefList (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        if (lineno < 0) {
        } else {
            list[0]->visit();
            list[1]->visit();
        }
        return nullptr;
    }
};
class Def : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    Def( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Def (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node)  {
        list.push_back(_node);
    }
    virtual Type *visit() {
        auto spec = list[0]->visit();
        auto ptrs = (Tuple *)(list[1]->visit());
        for (size_t i = 0; i < ptrs->args.size(); ++i) {
            auto tmp = ptrs->args[i];
            bool flag = true;
            bool flag_insert = true;
            if (stable.lookup_top(tmp->varname.value()).has_value()) {
                // already defined
                prterr(3, lineno, "variable is redefined in the same scope");
                flag_insert = false;
            }
            //type checking
            if (tmp->category == Category::UNCERTAIN) {
                if (!type_equal(tmp->type_to_cal, spec)) {
                    flag = false;
                } else{
                    stable.insert(tmp->varname.value(), spec);
                    flag_insert=false;
                }
            } else {
                auto ttmp = tmp;
                while (((Array *)ttmp)->type->category == Category::ARRAY) {
                    ttmp = ((Array *)ttmp)->type;
                }
                ((Array *)ttmp)->type = spec;
                if (!type_equal(tmp, tmp->type_to_cal)) {
                    flag = false;
                }
            }
            //use spec type no matter correct or not
            if (flag) {
            } else {
                prterr(5, lineno, "unmatching types on both sides of assignment operator (=)");
            }
            if (flag_insert) {
                stable.insert(tmp->varname.value(), tmp);
            }
        }
        return nullptr;
    }
};

class DecList : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    DecList( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DecList (" << lineno << ")" << std::endl;
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

class Dec : public Base {
   private:
    mutable std::vector< Base *> list;
    int lineno;

   public:
    Dec( Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0)  {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Dec (" << lineno << ")" << std::endl;
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
                auto ptr = list[0]->visit();
                return ptr;
            }
            case 3: {
                auto ptr1 = list[0]->visit();
                auto ptr2 = list[2]->visit();

                ptr1->type_to_cal = ptr2;
                return ptr1;
            }
        }
    }
};

#endif