#ifndef AST_HPP
#define AST_HPP
#include "ast_base.hpp"
#include "ast_dec.hpp"
#include "ast_expression.hpp"
#include "ast_external.hpp"
#include "ast_program.hpp"
#include "ast_specifier.hpp"
#include "ast_stmt.hpp"
#include "ast_terminal.hpp"
#include "symboltable.hpp"

class ast_Top {
   public:
    void print() {
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i]->print();
        }
    }

    void push( Base *stmnt) {
        vec.push_back(stmnt);
    }
    void visit() {
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i]->visit();
        }
    }

   private:
    std::vector< Base *> vec;
};
ast_Top *parseAST();

void sematicscheck(ast_Top *&root);
#endif