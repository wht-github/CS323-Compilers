%code requires{
#include "ast.hpp"
#include<string>
extern ast_Top *g_root;
  int yylex();
  void yyerror(const char *);
}

%union {
  long int_value;
  float float_value;
  std::string * char_value;
  std::string * type_value;
  std::string * id_value;
  std::string * kw_value;
  char *ast_node;
}
%locations
/* %type <ast_node> Program
%type <ast_node> ExtDefList ExtDef ExtDecList
%type <ast_node> Specifier StructSpecifier
%type <ast_node> VarDec FunDec VarList ParamDec
%type <ast_node> CompSt StmtList Stmt
%type <ast_node> DefList Def DecList Dec
%type <ast_node> Exp Args */

%token <int_value> INT
%token <float_value> FLOAT
%token <char_value> CHAR
%token <type_value> TYPE
%token <id_value> ID

%token <kw_value> STRUCT
%token <kw_value> LC RC COMMA SEMI
%token <kw_value> RETURN WHILE IF

%nonassoc LOWER_ELSE
%nonassoc <kw_value> ELSE

%right <kw_value> ASSIGN
%left <kw_value> OR
%left <kw_value> AND
%left <kw_value> LT LE GT GE EQ NE
%left <kw_value> PLUS MINUS
%left <kw_value> MUL DIV
%right <kw_value> NOT
%left <kw_value> LP RP LB RB DOT

%%

/**
 * High-level definition: top-level syntax for a SPL program
 * - global variable declarations
 * - function definitions
 */
Program:
    ExtDefList {
    }
  ;
ExtDefList:
    ExtDef ExtDefList {
    }
  | %empty {
    }
  ;
ExtDef:
    Specifier ExtDecList SEMI {
    }
  | Specifier SEMI {
    }
  | Specifier FunDec CompSt {
    }
  ;
ExtDecList:
    VarDec {
    }
  | VarDec COMMA ExtDecList {
    }
  ;

/**
 * Specifier: SPL type system
 * - primitive types: int, float, char
 * - structure type
 */
Specifier:
    TYPE {
    }
  | StructSpecifier {
    }
  ;
StructSpecifier:
    STRUCT ID LC DefList RC {
    }
  | STRUCT ID LC DefList error {
    }
  | STRUCT ID {
    }
  ;

/**
 * Declarator: variable and function declaration
 * The array type is specified by the declarator
 */
VarDec:
    ID {
    }
  | VarDec LB INT RB {
    }
  ;
FunDec:
    ID LP VarList RP {
    }
  | ID LP VarList error {
    }
  | ID LP RP {
    }
  | ID LP error {
    }
  ;
VarList:
    ParamDec COMMA VarList {
    }
  | ParamDec {
    }
  ;
ParamDec:
    Specifier VarDec {
    }
  ;

/**
 * Statement: program structures like branchs and loops
 * - enclosed bny curly braces
 * - end with a semicolon
 */
CompSt:
    LC DefList StmtList RC {
    }
  | LC DefList StmtList error {
    }
  ;
StmtList:
    Stmt StmtList {
    }
  | %empty { }
  ;
Stmt:
    Exp SEMI {

    }
  | Exp error {
    }
  | CompSt {
    }
  | RETURN Exp SEMI {

    }
  | RETURN Exp error {
    }
  | IF LP Exp RP Stmt %prec LOWER_ELSE {

    }
  | IF LP Exp RP Stmt ELSE Stmt {

    }
  | WHILE LP Exp RP Stmt {

    }
  ;

/* Local definition: declaration and assignment of local variables */
DefList:
    Def DefList {

    }
  | %empty  {  }
  ;
Def:
    Specifier DecList SEMI {
    }
  | Specifier DecList error {
    }
  ;
DecList:
    Dec {
    }
  | Dec COMMA DecList {
    }
  ;
Dec:
    VarDec {
    }
  | VarDec ASSIGN Exp {
    }
  | VarDec ASSIGN error {
    }
  ;

/**
 * Expression
 * - single constant
 * - operations on variables: operators have precedence and associativity
 */
Exp:
    Exp ASSIGN Exp {
    }
  | Exp AND Exp {
    }
  | Exp OR Exp {
    }
  | Exp LT Exp {
    }
  | Exp LE Exp {
    }
  | Exp GT Exp {
    }
  | Exp GE Exp {
    }
  | Exp NE Exp {
    }
  | Exp EQ Exp {
    }
  | Exp PLUS Exp {
    }
  | Exp MINUS Exp {
    }
  | Exp MUL Exp {

    }
  | Exp DIV Exp {

    }
  | LP Exp RP {
 }
  | MINUS Exp {

    }
  | NOT Exp {

    }
  | ID LP Args RP {

    }
  | ID LP RP {

    }
  | Exp LB Exp RB {

    }
  | Exp DOT ID {

    }
  | ID {

    }
  | INT {

    }
  | FLOAT {

    }
  | CHAR {

    }
  ;
Args:
    Exp COMMA Args {

    }
  | Exp {

    }
  ;

%%

void yyerror(const char *s) {
  printf("Error type B at Line : ");
}

ast_Top *parseAST(){
    /* yyparse(); */
    while(1)yylex();
    ast_Top * res = nullptr;
    return res;
}