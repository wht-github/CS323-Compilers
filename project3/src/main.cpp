#include <algorithm>  // std::for_each
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast.hpp"
#include "spltype.hpp"
#include "symboltable.hpp"
#include "tac.hpp"
using namespace std;
SymbolTable stable = SymbolTable();
ICG icg = ICG();
int main(int argc, char *argv[]) {
    stdin = fopen(argv[1], "r");
    ast_Top *ast = parseAST();

    // stable.push();

    // ast->print();

    // ast->visit();
    
    ast->translate();
    // cout << icg.m_genCodes.size() << endl;
    for (Tac &e : icg.m_genCodes) {
        cout << e.to_string() << endl;
    }
    // Type* ptr = new Function();
    // cout << (Category::FUNCTION == ptr->cat_getter())  << (true==false)<< endl;

    // Type * a = new Struct();
    // string x = "Fasdf";
    // stable.insert(x,a);
    // cout << (stable.lookup("Fasdf").value()->category != Category::STRUCT) << endl;
    // string c = "f";
    // string d = "f";
    // cout << (c==d) << endl;
    return 0;
}