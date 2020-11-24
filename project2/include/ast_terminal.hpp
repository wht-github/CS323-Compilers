#ifndef AST_TERMINAL_HPP
#define AST_TERMINAL_HPP

#include "ast.hpp"

#include <string>
#include <iostream>
class Terminal : public Base
{
    

public:
    std::string symbol;
    Terminal( std::string &_symbol) : symbol(_symbol) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << symbol << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for terminal" << std::endl;
        (void)_base;
    }
};
class ValInt : public Base
{
    

public:
    unsigned int val;
    ValInt(unsigned int _val) : val(_val) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "INT: " << val << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for int" << std::endl;
        (void)_base;
    }
};
class ValFloat : public Base
{



public:
    float val;
    ValFloat(float _val) : val(_val) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "FLOAT: " << val << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for float" << std::endl;
        (void)_base;
    }
};
class ValChar : public Base
{


public:
    std::string val;
    ValChar(std::string &_val) : val(_val) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "CHAR: " << val << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for char" << std::endl;
        (void)_base;
    }
};
class ValType : public Base
{
    

public:
    std::string val;
    ValType(std::string &_val) : val(_val) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "TYPE: " << val << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for type" << std::endl;
        (void)_base;
    }
};
class ValId : public Base
{
    

public:
    std::string val;
    ValId(std::string &_val) : val(_val) {}

    virtual void print(int idt = 0) 
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ID: " << val << std::endl;
    }

    virtual void push( Base *_base) 
    {
        std::cerr << "Can't call this function for identifier" << std::endl;
        (void)_base;
    }
};
#endif