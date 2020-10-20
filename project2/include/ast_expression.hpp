#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
// Declaration that holds a list of declarations

class Exp : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Exp(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Exp (" << lineno << ")" << std::endl;
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
class Args : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Args(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Args (" << lineno << ")" << std::endl;
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