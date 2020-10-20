#ifndef AST_SPECIFIER_HPP
#define AST_SPECIFIER_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
// Declaration that holds a list of declarations

class Specifier : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Specifier(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Specifier (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i)
        {
            list[i]->print(idt+indent);
        }
    }

    virtual void push(const Base *_node) const
    {
        list.push_back(_node);
    }
};
class StructSpecifier : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    StructSpecifier(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "StructSpecifier (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i)
        {
            list[i]->print(idt+indent);
        }
    }

    virtual void push(const Base *_node) const
    {
        list.push_back(_node);
    }
};

#endif