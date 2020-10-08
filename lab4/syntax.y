%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}

%token LC RC LB RB COLON COMMA
%token STRING NUMBER ENUMBER
%token TRUE FALSE VNULL
%%

Json:
      Value
    ;
Value:
      Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    | ENUMBER error { puts("Numbers cannot have leading zeroes, recovered"); }
    ;
Object:
      LC RC
    | LC Members RC
    | LC Members COMMA error{puts("Comma instead if closing brace, recovered");}
    | LC Members RC Values error {puts("Extra value after close, recovered");}
    ;
Members:
      Member
    | Member COMMA Members
    | Member COMMA error {puts("Extra comma, recovered");}
    ;
Member:
      STRING COLON Value
      |STRING COMMA Value error{puts("Comma instead of colon, recovered");}
      |STRING COLON  COLON Value error {puts("Double colon, recovered");}
      |STRING Value error {puts("Missing colon, recovered");}
    ;
Array:
      LB RB
    | LB Values RB
    | LB Values RC error { puts("unmatched right bracket, recovered"); }
    | LB Values RB RB error {puts("Extra close, recovered");}
    | LB Values RB COMMA error {puts("Comma after the close, recovered");}
    | LB Values error {puts("Unclosed array, recovered");}
    ;
Values:
      Value
    | Value COMMA Values
    | Value COMMA error{puts("extra comma, recovered");}
    | Value COLON Values error{puts("Colon instead of comma, recovered");}
    | STRING Value error {puts("Missing colon, recovered");}
    | COMMA Value error {puts("<-- missing value, recovered");}
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}
