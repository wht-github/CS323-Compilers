%{
    #include"lex.yy.c"

    int result=1;
    void yyerror(const char *s){result = 0;}
%}
%token LP RP LB RB LC RC
%%

EXP:
    LP EXP RP EXP
    | LB EXP RB EXP
    | LC EXP RC EXP
    | %empty {}
    ;
%%

int validParentheses(char *expr){
    yy_scan_string(expr);
    yyparse();
    return result;
}
