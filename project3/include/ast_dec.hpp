#ifndef AST_DEC_HPP
#define AST_DEC_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast.hpp"
#include "ast_terminal.hpp"
#include "symboltable.hpp"
// Declaration that holds a list of declarations

class VarDec : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    VarDec(int _lineno) : lineno(_lineno) {
    }

    VarDec(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarDec (" << lineno << ")" << std::endl;
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
                prterr(-1, lineno, "VarDec");
                exit(1);
        }
    }
    virtual void translate(V &place) {
        switch (list.size()) {
            case 1: {
                icg.register_variable(((ValId *)list[0])->val, place.m_count);
                return ;
            }
            case 4: {
                //array
                return ;
            }
            default:
                prterr(-1, lineno, "VarDec");
                exit(1);
        }
    }
};
class FunDec : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    FunDec(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "FunDec (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
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

                NTuple *tmp = (NTuple *)(list[2]->visit());
                Tuple *tmpargs = new Tuple();
                for (size_t i = 0; i < tmp->nargs.size(); i++) {
                    stable.insert(tmp->nargs[i].first, tmp->nargs[i].second);
                    tmpargs->args.push_back(tmp->nargs[i].second);
                }
                ptr->funcargs = tmpargs;
                return ptr;
            }
            default: {
                prterr(-1, lineno, "FunDec");
                exit(1);
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 3: {
                Tac i = Tac(Operator::FUNC, ((ValId *)list[0])->val);
                icg.appendCode(i);
                return {};
            }
            case 4: {
                Tac i = Tac(Operator::FUNC, ((ValId *)list[0])->val);
                icg.appendCode(i);
                std::vector<V> v1 = std::get<std::vector<V> >(list[2]->translate().value());
                std::vector<V>::iterator iter = v1.begin();
                for (; iter != v1.end(); ++iter) {
                    Tac i = Tac(Operator::PARAM, *iter);
                    icg.appendCode(i);
                }
                return {};
            }
        }
    }
};

class VarList : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    VarList(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarList (" << lineno << ")" << std::endl;
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
                NTuple *ptr = new NTuple();
                auto tmp = list[0]->visit();
                ptr->nargs.push_back(std::make_pair(tmp->varname.value(), tmp));
                return ptr;
            }
            case 3: {
                NTuple *ptr = (NTuple *)(list[2]->visit());
                auto tmp = list[0]->visit();
                ptr->nargs.push_back(std::make_pair(tmp->varname.value(), tmp));
                return ptr;
            }
            default: {
                prterr(-1, lineno, "VarList");
                exit(1);
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 1: {
                V v = std::get<V>(list[0]->translate().value());
                return std::optional<retvar>{std::vector<V>(1, v)};
            }
            case 3: {
                V v = std::get<V>(list[0]->translate().value());
                std::vector<V> v1 = std::get<std::vector<V> >(list[2]->translate().value());
                v1.push_back(v);
                return std::optional<retvar>{v1};
            }
        }
    }
};

class ParamDec : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    ParamDec(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ParamDec (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
        list.push_back(_node);
    }
    virtual Type *visit() {
        auto spec = list[0]->visit();
        auto tmp = list[1]->visit();
        //type checking
        if (tmp->cat_getter() == Category::UNCERTAIN) {
            spec->varname = tmp->varname.value();
            return spec;
        } else {
            auto ttmp = tmp;
            while (((Array *)ttmp)->type->cat_getter() == Category::ARRAY) {
                ttmp = ((Array *)ttmp)->type;
            }
            ((Array *)ttmp)->type = spec;
            return tmp;
        }
    }
    virtual std::optional<retvar> translate() {
        V v = icg.new_variable();
        list[0]->translate();
        list[1]->translate(v);
        return std::optional<retvar>{v};
    }
};

////////////////////////////////////////////////////////////////////////////////
class DefList : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    DefList(int _lineno) : lineno(_lineno) {
    }

    DefList(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DefList (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
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
    virtual std::optional<retvar> translate() {
        if (lineno < 0) {
        } else {
            list[0]->translate();
            list[1]->translate();
        }
        return {};
    }
};
class Def : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    Def(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Def (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i]->print(idt + indent);
        }
    }

    virtual void push(Base *_node) {
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
            if (tmp->cat_getter() == Category::UNCERTAIN) {
                if (tmp->type_to_cal.has_value() && !type_equal(tmp->type_to_cal.value(), spec)) {
                    flag = false;
                } else {
                    stable.insert(tmp->varname.value(), spec);
                    flag_insert = false;
                }
            } else {
                auto ttmp = tmp;
                while (((Array *)ttmp)->type->cat_getter() == Category::ARRAY) {
                    ttmp = ((Array *)ttmp)->type;
                }
                ((Array *)ttmp)->type = spec;
                if (tmp->type_to_cal.has_value() && !type_equal(tmp, tmp->type_to_cal.value())) {
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
    virtual std::optional<retvar> translate() {
        list[0]->translate();
        list[1]->translate();
        return {};
    }
};

class DecList : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    DecList(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DecList (" << lineno << ")" << std::endl;
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
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 1: {
                list[0]->translate();
                return {};
            }
            case 3: {
                list[0]->translate();
                list[2]->translate();
                return {};
            }
        }
    }
};

class Dec : public Base {
   private:
    mutable std::vector<Base *> list;
    int lineno;

   public:
    Dec(Base *_node, int _lineno) : lineno(_lineno) {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Dec (" << lineno << ")" << std::endl;
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
                auto ptr = list[0]->visit();
                return ptr;
            }
            case 3: {
                auto ptr1 = list[0]->visit();
                auto ptr2 = list[2]->visit();
                if (ptr2 != nullptr)
                    ptr1->type_to_cal = ptr2;
                return ptr1;
            }
        }
    }
    virtual std::optional<retvar> translate() {
        switch (list.size()) {
            case 1: {
                V v = icg.new_variable();
                list[0]->translate(v);
                return {};
            }
            case 3: {
                V v = icg.new_variable();
                list[0]->translate(v);
                list[2]->translate(v);
                return {};
            }
        }
    }
};

#endif