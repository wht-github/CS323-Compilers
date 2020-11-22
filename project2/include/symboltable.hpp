#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using std::vector, std::unordered_map;
typedef unordered_map<std::string, Type*> symboltable;
typedef vector<symboltable> tablestack;
class SymbolTable {
   public:
    tablestack ts;
    void push() {
        ts.push_back(symboltable());
    }
    symboltable& get_top() {
        return ts.back();
    }
    void pop() {
        ts.pop_back();
    }
    void insert(string s, Type* t) {
        ts.back().insert(make_pair(s, t));
    }
    optional<Type*> lookup(string s) {
        tablestack::reverse_iterator it = ts.rbegin();
        for (; it != ts.rend(); ++it) {
            auto ptr = (*it).find(s);
            if (ptr != (*it).end()) {
                return ptr->second;
                // return (*it).at(s);
            }
        }
        return {};
    }
    optional<Type*> lookup_top(string s) {
        auto it = ts.back();
        auto ptr = it.find(s);
        if (ptr != it.end()) {
            // return ptr->second;
            return it.at(s);
        }else {
            return {};
        }
    }
};
static SymbolTable stable;
#endif
