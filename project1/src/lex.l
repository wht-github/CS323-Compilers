%{
  #include "parser.tab.hpp"
  #include <iostream>
  int yylex();
    #define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycolno; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycolno + yyleng; \
    yycolno += yyleng;
  int yycolno = 1;
  #define lexlog(lexium) {\
              std::cout << lexium;\
              std::cout << " at "\
                        << yylloc.first_line \
                        << "," \
                        << yylloc.last_line \
                        << " " \
                        << yylloc.first_column \
                        << "," \
                        << yylloc.last_column \
                        << std::endl;\
          }

%}
%option noyywrap
INT  0|-?[1-9][0-9]{0,9}|-?0[xX][1-9a-fA-F][0-9a-fA-F]*
FLOAT -?[1-9][0-9]*\.[0-9]+
CHAR \'[a-zA-Z]\'|\'\\x[1-9a-fA-F][0-9a-fA-F]*\'
ID  [a-zA-Z][a-zA-Z0-9_]*|_[a-zA-Z0-9_]+
TYPE     int|float|char
STRUCT   "struct"
IF       "if"
ELSE     "else"
WHILE    "while"
RETURN   "return"
DOT      "."
SEMI     ";"
COMMA    ","
ASSIGN   "="
LT       "<"

LE       "<="
GT       ">"
GE       ">="
NE       "!="
EQ       "=="
PLUS     "+"
MINUS    "-"
MUL      "*"
DIV      "/"
AND      "&&"
OR       "||"
NOT      "!"
LP       "("
RP       ")"
LB       "["
RB       "]"
LC       "{"
RC       "}"
SPC     [ \t]

%%
{SEMI}  {
  #ifdef DEBUG
  lexlog("SEMI");
  #endif
  return SEMI;
  }
"\n"  {
  #ifdef DEBUG
  lexlog(R"(\n)");
  #endif
        yycolno = 1;
        yylineno += 1;
      }
{SPC}   { ;}
{DOT}   {
  #ifdef DEBUG
  lexlog("DOT");
  #endif
  return DOT;}
{LT}     {
    #ifdef DEBUG
  lexlog("LT");
  #endif
  return LT;}
{LE}    {
    #ifdef DEBUG
  lexlog("LE");
  #endif
  return LE;}
{GT}    {
    #ifdef DEBUG
  lexlog("GT");
  #endif
  return GT;}
{GE}  {
  #ifdef DEBUG
  lexlog("GE");
  #endif
  return GE;}
{NE}  {
  #ifdef DEBUG
  lexlog("NE");
  #endif
  return NE;}
{EQ}  {
  #ifdef DEBUG
  lexlog("EQ");
  #endif
  return EQ;}
{PLUS} {
  #ifdef DEBUG
  lexlog("PLUS");
  #endif
  return PLUS;}
{MINUS} {
  #ifdef DEBUG
  lexlog("MINUS");
  #endif
  return MINUS;}
{MUL} {
  #ifdef DEBUG
  lexlog("MUL");
  #endif
  return MUL;}
{DIV} {
  #ifdef DEBUG
  lexlog("DIV");
  #endif
  return DIV;}
{AND} {
  #ifdef DEBUG
  lexlog("AND");
  #endif
  return AND;}
{OR} {
  #ifdef DEBUG
  lexlog("OR");
  #endif
  return OR;}
{NOT} {
  #ifdef DEBUG
  lexlog("NOT");
  #endif
  return NOT;}
{LP} {
  #ifdef DEBUG
  lexlog("LP");
  #endif
  return LP;}
{RP} {
  #ifdef DEBUG
  lexlog("RP");
  #endif
  return RP;}
{LB} {
  #ifdef DEBUG
  lexlog("LB");
  #endif
  return LB;}
{RB} {
  #ifdef DEBUG
  lexlog("RB");
  #endif
  return RB;}
{LC} {
  #ifdef DEBUG
  lexlog("LC");
  #endif
  return LC;}
{RC} {
  #ifdef DEBUG
  lexlog("RC");
  #endif
  return RC;}
{ASSIGN} {
  #ifdef DEBUG
  lexlog("ASSIGN");
  #endif
  return ASSIGN;}
{IF} {
  #ifdef DEBUG
  lexlog("IF");
  #endif
  return IF;}
{ELSE} {
  #ifdef DEBUG
  lexlog("ELSE");
  #endif
  return ELSE;}
{WHILE} {
  #ifdef DEBUG
  lexlog("WHILE");
  #endif
  return WHILE;}
{STRUCT} {
  #ifdef DEBUG
  lexlog("STRUCT");
  #endif
  return STRUCT;}
{RETURN} {
  #ifdef DEBUG
  lexlog("RETURN");
  #endif
  return RETURN;}
{COMMA} {
  #ifdef DEBUG
  lexlog("COMMA");
  #endif
  return COMMA;}

{TYPE} {
  yylval.type_value = new std::string(yytext);
  // std::cout << yytext << std::endl;
  #ifdef DEBUG
  lexlog(*yylval.type_value);
  #endif
  return TYPE;
}
{INT} {
  yylval.int_value = new std::stoi(yytext, nullptr, 0); 
  #ifdef DEBUG
  lexlog(*yylval.int_value);
  #endif
  return INT;}

{ID} {
  yylval.id_value = new std::string(yytext);
  #ifdef DEBUG
  lexlog(*yylval.id_value);
  #endif
  return ID;}
{CHAR} {
  yylval.char_value = yytext[1];
  #ifdef DEBUG
  lexlog(yylval.char_value);
  #endif
  return CHAR;}
{FLOAT} {
  yylval.float_value = std::stoi(yytext, nullptr, 0); 
  #ifdef DEBUG
  lexlog(yylval.float_value);
  #endif
  return FLOAT;}

.                   { std::cerr << "Invalid token: " << yytext << std::endl; }
%%
