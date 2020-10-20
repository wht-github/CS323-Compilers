#ifndef AST_EXTERNAL_HPP
#define AST_EXTERNAL_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
// Declaration that holds a list of declarations

class ExtDefList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    ExtDefList( int _lineno) : lineno(_lineno)
    {
    }
    ExtDefList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDefList (" << lineno << ")" << std::endl;
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
class ExtDecList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    ExtDecList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDecList (" << lineno << ")" << std::endl;
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
class ExtDef : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    ExtDef(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ExtDef (" << lineno << ")" << std::endl;
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