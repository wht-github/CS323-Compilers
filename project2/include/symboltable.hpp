#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using std::unordered_map;
using std::vector;
typedef unordered_map<std::string, Type*> symboltable;
typedef vector<symboltable*> tablestack;
class SymbolTable {
   public:
    tablestack ts;
    void push() {
        ts.push_back(new symboltable());
#ifdef DEBUG
        std::cout << "size after push:" << ts.size() << std::endl;
#endif
    }
    symboltable* get_top() {
        return ts.back();
    }
    void pop() {
        delete ts.back();
        ts.pop_back();
#ifdef DEBUG
        std::cout << "size remain:" << ts.size() << std::endl;
#endif
    }
    void insert(string s, Type* t) {
#ifdef DEBUG
        std::cout << "Insert: " << s << std::endl;
#endif
        ts.back()->insert(make_pair(s, t));
    }
    optional<Type*> lookup(string s) {
#ifdef DEBUG
        std::cout << "Look up for: " << s << std::endl;
#endif
        tablestack::reverse_iterator it = ts.rbegin();
        for (; it != ts.rend(); ++it) {
            auto ptr = (*it)->find(s);
            if (ptr != (*it)->end()) {
                return ptr->second;
                // return (*it).at(s);
            }
        }
#ifdef DEBUG
        std::cout << "Not found: " << s << std::endl;
#endif
        return {};
    }
    optional<Type*> lookup_top(string s) {
        auto it = ts.back();
        auto ptr = it->find(s);
        if (ptr != it->end()) {
            // return ptr->second;
            return it->at(s);
        } else {
            return {};
        }
    }
};
extern SymbolTable stable;
#endif
