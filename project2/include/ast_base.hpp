#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include <string>
#include <iostream>
#include <vector>

class Base
{
public:
const int indent = 2;
    virtual ~Base() {}

    virtual void print(int idt = 0) const = 0;
    virtual void push(const Base *_var) const = 0;
};

#endif