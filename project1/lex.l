%{
  #include <iostream>
  using namespace std;
  extern "C" int yylex();
%}
alpha [a-zA-Z]
digit [0-9]
KEYWORD
%option yylineno
%option noyywrap
%%
[ \t]		;
[ \n]   { yylineno = yylineno + 1;}
[0-9]+\.[0-9]+  { cout << "Found a floating-point number:" << yytext << endl; }
[0-9]+          { cout << "Found an integer:" << yytext << endl; }
[a-zA-Z0-9]+    { cout << "Found a string: " << yytext << endl; }
%%
int main(int, char**) {
  // lex through the input:
  while (yylex());
}