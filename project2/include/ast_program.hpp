#ifndef AST_PROGRAM_HPP
#define AST_PROGRAM_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
class AstProgram : public Base
{
private:
    mutable std::vector< Base *> list;
    int lineno;

public:
    AstProgram( Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Program (" << lineno << ")" << std::endl;
        for (size_t i = 0; i < list.size(); ++i)
        {
            list[i]->print(idt + indent);
        }
    }

    virtual void push( Base *_node) 
    {
        list.push_back(_node);
    }
    virtual Type *visit()
    {
        list[0]->visit();
        return nullptr;
    }
};

#endif