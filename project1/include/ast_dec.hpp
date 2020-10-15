#ifndef AST_DEC_HPP
#define AST_DEC_HPP

#include "ast.hpp"

#include <vector>
#include <string>
#include <iostream>
// Declaration that holds a list of declarations

class VarDec : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    VarDec(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarDec (" << lineno << ")" << std::endl;
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
class FunDec : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    FunDec(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "FunDec (" << lineno << ")" << std::endl;
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

class VarList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    VarList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "VarList (" << lineno << ")" << std::endl;
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

class ParamDec : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    ParamDec(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "ParamDec (" << lineno << ")" << std::endl;
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

////////////////////////////////////////////////////////////////////////////////
class DefList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    DefList(int _lineno) : lineno(_lineno)
    {

    }

    DefList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        if (lineno < 0)
            return;
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DefList (" << lineno << ")" << std::endl;
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
class Def : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Def(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Def (" << lineno << ")" << std::endl;
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

class DecList : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    DecList(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "DecList (" << lineno << ")" << std::endl;
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

class Dec : public Base
{
private:
    mutable std::vector<const Base *> list;
    int lineno;
public:
    Dec(const Base *_node, int _lineno) : lineno(_lineno)
    {
        list.push_back(_node);
    }

    virtual void print(int idt = 0) const
    {
        for (int i = 0; i < idt; i++)
            std::cout << " ";
        std::cout << "Dec (" << lineno << ")" << std::endl;
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