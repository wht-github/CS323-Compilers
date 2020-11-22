#include "ast.hpp"
#include "spltype.hpp"
#include "symboltable.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
int main(int argc, char *argv[]) {
    stdin = fopen(argv[1], "r");
    ast_Top *ast = parseAST();
    stable = SymbolTable();
    stable.push();
    ast->visit();
    
    
    // Type * a = new Struct();
    // string x = "Fasdf";
    // stable.insert(x,a);
    // cout << (stable.lookup("Fasdf").value()->category != Category::STRUCT) << endl;
    // string c = "f";
    // string d = "f";
    // cout << (c==d) << endl;
    return 0;

}