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



class ast_Top {
public:
    void print() {
	for(size_t i = 0; i < vec.size(); ++i) {
	    vec[i]->print();
	}
    }
 
    void push(const Base *stmnt) {
        vec.push_back(stmnt);
    }
   
private:
    std::vector<const Base *> vec;
};
ast_Top *parseAST();

#endif