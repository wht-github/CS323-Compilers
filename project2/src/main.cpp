#include "ast.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    stdin = fopen(argv[1], "r");
    ast_Top *ast = parseAST();
    
    ast->print();
    return 0;
}