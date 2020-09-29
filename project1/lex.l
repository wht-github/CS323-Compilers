%{
  #include <iostream>
  using namespace std;
  #define EXIT_OK 0
  #define EXIT_FAIL 1
  extern "C" int yylex();
%}
BIN_DIGIT   [0-1]
OCT_DIGIT   [0-7]
DEC_DIGIT   [0-9]
HEX_DIGIT   [0-9a-fA-F]
DEC_LITERAL {DEC_DIGIT}({DEC_DIGIT}|_)*
BIN_LITERAL 0b({BIN_DIGIT}|_)*{BIN_DIGIT}({BIN_DIGIT}|_)*
OCT_LITERAL 0o({OCT_DIGIT}|_)*{OCT_DIGIT}({OCT_DIGIT}|_)*
HEX_LITERAL 0x({HEX_DIGIT}|_)*{HEX_DIGIT}({HEX_DIGIT}|_)*
FLOAT_EXPONENT (e|E)(+|-)?({DEC_DIGIT}|_)*{DEC_DIGIT} ({DEC_DIGIT}|_)*
INT  ( {DEC_LITERAL} | {BIN_LITERAL} | {OCT_LITERAL} | {HEX_LITERAL} )      
/* integer in 32-bits (decimal or hexadecimal) */
FLOAT    /* floating point number (only dot-form) */
CHAR     /* single character (printable or hex-form) */
ID       /* identifier */
TYPE     int | float | char
STRUCT   struct
IF       if
ELSE     else
WHILE    while
RETURN   return
DOT      .
SEMI     ;
COMMA    ,
ASSIGN   =
LT       <
LE       <=
GT       >
GE       >=
NE       !=
EQ       ==
PLUS     +
MINUS    -
MUL      *
DIV      /
AND      &&
OR       ||
NOT      !
LP       (
RP       )
LB       [
RB       ]
LC       {
RC       }
%option yylineno
%option noyywrap
%%
[ \t]		;
[ \n]   { yylineno = yylineno + 1;}
[0-9]+\.[0-9]+  { cout << "Found a floating-point number:" << yytext << endl; }
[0-9]+          { cout << "Found an integer:" << yytext << endl; }
[a-zA-Z0-9]+    { cout << "Found a string: " << yytext << endl; }
%%
int main(int argc, char **argv){
    char *file_path;
    if(argc < 2){
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAIL;
    }
    else if(argc == 2){
        file_path = argv[1];
        if(!(yyin = fopen(file_path, "r"))){
            perror(argv[1]);
            return EXIT_FAIL;
        }
        yylex();
        return EXIT_OK;
    }
    else{
        fputs("Too much arguments!\n", stderr);
        return EXIT_FAIL;
    }
}
