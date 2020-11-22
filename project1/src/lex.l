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
%option yylineno
SINGLE_LINE_COMMENT "//".*

INT  0|-?[1-9][0-9]{0,9}|-?0[xX][1-9a-fA-F][0-9a-fA-F]*
FLOAT -?[0-9]*\.[0-9]+
CHAR \'[\x00-\x7F]\'
ID  [a-zA-Z][a-zA-Z0-9_]*|_[a-zA-Z0-9_]*
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
SPC     [ \t\r]
ERRID    [0-9][a-zA-Z0-9_]*
ERRCHAR  \'\\x[0-9a-zA-Z]+\' 
%x C_COMMENT
%%


"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { yycolno = 1;}
<C_COMMENT>.    { }
{SINGLE_LINE_COMMENT} {yycolno = 1;}
{SEMI}  {
  #ifdef DEBUG
  lexlog("SEMI");
  #endif
  yylval.kw_value = new std::string("SEMI");
  return SEMI;
  }
"\n"  {
        yycolno = 1;
      }
{SPC}   { ;}
{DOT}   {
  #ifdef DEBUG
  lexlog("DOT");
  #endif
  yylval.kw_value = new std::string("DOT");
  return DOT;}
{LT}     {
    #ifdef DEBUG
  lexlog("LT");
  #endif
  yylval.kw_value = new std::string("LT");
  return LT;}
{LE}    {
    #ifdef DEBUG
  lexlog("LE");
  #endif
  yylval.kw_value = new std::string("LE");
  return LE;}
{GT}    {
    #ifdef DEBUG
  lexlog("GT");
  #endif
  yylval.kw_value = new std::string("GT");
  return GT;}
{GE}  {
  #ifdef DEBUG
  lexlog("GE");
  #endif
  yylval.kw_value = new std::string("GE");
  return GE;}
{NE}  {
  #ifdef DEBUG
  lexlog("NE");
  #endif
  yylval.kw_value = new std::string("NE");
  return NE;}
{EQ}  {
  #ifdef DEBUG
  lexlog("EQ");
  #endif
  yylval.kw_value = new std::string("EQ");
  return EQ;}
{PLUS} {
  #ifdef DEBUG
  lexlog("PLUS");
  #endif
  yylval.kw_value = new std::string("PLUS");
  return PLUS;}
{MINUS} {
  #ifdef DEBUG
  lexlog("MINUS");
  #endif
  yylval.kw_value = new std::string("MINUS");
  return MINUS;}
{MUL} {
  #ifdef DEBUG
  lexlog("MUL");
  #endif
  yylval.kw_value = new std::string("MUL");
  return MUL;}
{DIV} {
  #ifdef DEBUG
  lexlog("DIV");
  #endif
  yylval.kw_value = new std::string("DIV");
  return DIV;}
{AND} {
  #ifdef DEBUG
  lexlog("AND");
  #endif
  yylval.kw_value = new std::string("AND");
  return AND;}
{OR} {
  #ifdef DEBUG
  lexlog("OR");
  #endif
  yylval.kw_value = new std::string("OR");
  return OR;}
{NOT} {
  #ifdef DEBUG
  lexlog("NOT");
  #endif
  yylval.kw_value = new std::string("NOT");
  return NOT;}
{LP} {
  #ifdef DEBUG
  lexlog("LP");
  #endif
  yylval.kw_value = new std::string("LP");
  return LP;}
{RP} {
  #ifdef DEBUG
  lexlog("RP");
  #endif
  yylval.kw_value = new std::string("RP");
  return RP;}
{LB} {
  #ifdef DEBUG
  lexlog("LB");
  #endif
  yylval.kw_value = new std::string("LB");
  return LB;}
{RB} {
  #ifdef DEBUG
  lexlog("RB");
  #endif
  yylval.kw_value = new std::string("RB");
  return RB;}
{LC} {
  #ifdef DEBUG
  lexlog("LC");
  #endif
  yylval.kw_value = new std::string("LC");
  return LC;}
{RC} {
  #ifdef DEBUG
  lexlog("RC");
  #endif
  yylval.kw_value = new std::string("RC");
  return RC;}
{ASSIGN} {
  #ifdef DEBUG
  lexlog("ASSIGN");
  #endif
  yylval.kw_value = new std::string("ASSIGN");
  return ASSIGN;}
{IF} {
  #ifdef DEBUG
  lexlog("IF");
  #endif
  yylval.kw_value = new std::string("IF");
  return IF;}
{ELSE} {
  #ifdef DEBUG
  lexlog("ELSE");
  #endif
  yylval.kw_value = new std::string("ELSE");
  return ELSE;}
{WHILE} {
  #ifdef DEBUG
  lexlog("WHILE");
  #endif
  yylval.kw_value = new std::string("WHILE");
  return WHILE;}
{STRUCT} {
  #ifdef DEBUG
  lexlog("STRUCT");
  #endif
  yylval.kw_value = new std::string("STRUCT");
  return STRUCT;}
{RETURN} {
  #ifdef DEBUG
  lexlog("RETURN");
  #endif
  yylval.kw_value = new std::string("RETURN");
  return RETURN;}
{COMMA} {
  #ifdef DEBUG
  lexlog("COMMA");
  #endif
  yylval.kw_value = new std::string("COMMA");
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
  yylval.int_value = std::strtol(yytext, nullptr, 0); 
  #ifdef DEBUG
  lexlog(yylval.int_value);
  #endif
  return INT;}

{ID} {
  yylval.id_value = new std::string(yytext);
  #ifdef DEBUG
  lexlog(*yylval.id_value);
  #endif
  return ID;}
{CHAR} {
  if (yytext[1] == '\\' && yytext[2] == 'x') {
    yytext[1] = '0';
    long  c = std::strtol(yytext+1, nullptr, 0);
    yytext[1] = '\\';
    if (c == 0 || c > 255) {
      std::cerr << "Error type A at Line " << yylineno <<": unknown lexeme " << yytext << std::endl;
      return UNKNOWN_LEXIUM;
    }
  }
  yylval.char_value = new std::string(yytext);
  #ifdef DEBUG
  lexlog(*yylval.char_value);
  #endif
  return CHAR;
  out: ;}
{FLOAT} {
  yylval.float_value = ::atof(yytext); 
  #ifdef DEBUG
  lexlog(yylval.float_value);
  #endif
  return FLOAT;}
{ERRID}|{ERRCHAR} {
    std::cerr << "Error type A at Line " << yylineno <<": unknown lexeme " << yytext << std::endl;
    return UNKNOWN_LEXIUM;
  }
.  {
    std::cerr << "Error type A at Line " << yylineno <<": unknown lexeme " << yytext << std::endl;
    return UNKNOWN_LEXIUM;
  }
%%
