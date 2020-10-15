#ifndef AST_STMT_HPP
#define AST_STMT_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
// Declaration that holds a list of declarations

class CompSt : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    CompSt(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "CompSt (" << lineno << ")" << std::endl;
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
class StmtList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    StmtList(int _lineno) : lineno(_lineno)
    {
    }
    StmtList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "StmtList (" << lineno << ")" << std::endl;
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

class Stmt : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Stmt(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Stmt (" << lineno << ")" << std::endl;
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