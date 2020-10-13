%{
  #include"syntax.tab.h"

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
QUOTE_ESCAPE \'|\"
ASCII_ESCAPE \x{OCT_DIGIT}{HEX_DIGIT}|\n|\r|\t|\\|\0
UNICODE_ESCAPE  \u{({HEX_DIGIT}_*)1..6}

INT  ({DEC_LITERAL}|{BIN_LITERAL}|{OCT_LITERAL}|{HEX_LITERAL})  
FLOAT {DEC_LITERAL}.|{DEC_LITERAL}{FLOAT_EXPONENT}|{DEC_LITERAL}.{DEC_LITERAL}{FLOAT_EXPONENT}?|{DEC_LITERAL}(.{DEC_LITERAL})?{FLOAT_EXPONENT}?    
CHAR '(~['\\n\r\t]|{QUOTE_ESCAPE}|{ASCII_ESCAPE}|{UNICODE_ESCAPE})'

ID  [a-zA-Z][a-zA-Z0-9_]*|_[a-zA-Z0-9_]+

TYPE     int|float|char
STRUCT   struct
IF       if
ELSE     else
WHILE    while
RETURN   return
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
SPC     [ \t\n]
%option yylineno
%option noyywrap
%%
{SEMI}  {return SEMI;}
{SPC}   { ;}
{DOT}   {return DOT;}
{LT}     {return LT;}
{LE}    {return LE;}
{GT}    {return GT;}
{GE}  {return GE;}
{NE}  {return NE;}
{EQ}  {return EQ;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{MUL} {return MUL;}
{DIV} {return DIV;}
{AND} {return AND;}
{OR} {return OR;}
{NOT} {return NOT;}
{LP} {return LP;}
{RP} {return RP;}
{LB} {return LB;}
{RB} {return RB;}
{LC} {return LC;}
{RC} {return RC;}
{ASSIGN} {return ASSIGN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{ASSIGN} {return ASSIGN;}
{COMMA} {return COMMA;}

{TYPE} {
  yylval.type_value = new std::string(yytext);
}
{INT} {yylval.int_value = std::stoi(yytext, nullptr, 0); return INT;}
{FLOAT} {yylval.float_value = std::stoi(yytext, nullptr, 0); return FLOAT;}
{ID} {yylval.id_value = new std::string(yytext); return ID;}
{CHAR} {yy}
%%

