%code requires{
#include "ast.hpp"
#include<string>
#include<iostream>
extern ast_Top *g_root; // A way of getting the AST out
  int yylex();
  void yyerror(const char *);
  
}

%union {
  unsigned int int_value;
  float float_value;
  std::string * char_value;
  std::string * type_value;
  std::string * id_value;
  std::string * kw_value;
  Base * ast_node;
}
%locations
%type <ast_node> Program
%type <ast_node> ExtDefList ExtDef ExtDecList
%type <ast_node> Specifier StructSpecifier
%type <ast_node> VarDec FunDec VarList ParamDec
%type <ast_node> CompSt StmtList Stmt
%type <ast_node> DefList Def DecList Dec
%type <ast_node> Exp Args

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
%nonassoc UNKNOWN_LEXIUM
%right <kw_value> ASSIGN
%left <kw_value> OR
%left <kw_value> AND
%left <kw_value> LT LE GT GE EQ NE
%left <kw_value> PLUS MINUS
%left <kw_value> MUL DIV
%right <kw_value> NOT
%left <kw_value> LP RP LB RB DOT

%start ROOT
                        
%%

ROOT:
    Program{g_root->push($1);}
    ;
/**
 * High-level definition: top-level syntax for a SPL program
 * - global variable declarations
 * - function definitions
 */
Program:
    ExtDefList {
      $$ = new AstProgram($1, @1.first_line);
    }
  ;
ExtDefList:
    ExtDef ExtDefList {
      $$ = new ExtDefList($1, @1.first_line);
      $$->push($2);
    }
  | %empty {
      $$ = new ExtDefList(-1);
    }
  ;
ExtDef:
    Specifier ExtDecList SEMI {
      $$ = new ExtDef($1, @1.first_line);
      $$->push($2);
      $$->push(new Terminal(*$3));
    }
  | Specifier SEMI {
    $$ = new ExtDef($1, @1.first_line);
    $$->push(new Terminal(*$2));
    }
  | Specifier FunDec CompSt {
    $$ = new ExtDef($1, @1.first_line);
    $$->push($2);
    $$->push($3);
    }
  | ExtDecList error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing specifier" << std::endl;
      $$ = new ExtDef($1, @1.first_line);
    }
  | Specifier ExtDecList error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing semicolon ';''" << std::endl;
      $$ = new ExtDef($1, @1.first_line);
      $$->push($2);
    }
  ;
ExtDecList:
    VarDec {
      $$ = new ExtDecList($1, @1.first_line);
    }
  | VarDec COMMA ExtDecList {
    $$ = new ExtDecList($1, @1.first_line);
    $$->push(new Terminal(*$2));
    $$->push($3);
    }
  ;

/**
 * Specifier: SPL type system
 * - primitive types: int, float, char
 * - structure type
 */
Specifier:
    TYPE {
      $$ = new Specifier(new ValType(*$1), @1.first_line);
    }
  | StructSpecifier {
      $$ = new Specifier($1, @1.first_line);
    }
  ;
StructSpecifier:
    STRUCT ID LC DefList RC {
      $$ = new StructSpecifier(new Terminal(*$1), @1.first_line);
      $$->push(new ValId(*$2));
      $$->push(new Terminal(*$3));
      $$->push($4);
      $$->push(new Terminal(*$5));
    }
  | STRUCT ID {
      $$ = new StructSpecifier(new Terminal(*$1), @1.first_line);
      $$->push(new ValId(*$2));
    }
  ;

/**
 * Declarator: variable and function declaration
 * The array type is specified by the declarator
 */
VarDec:
    ID {
      $$ = new VarDec(new ValId(*$1), @1.first_line);
    }
  | VarDec LB INT RB {
      $$ = new VarDec($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push(new ValInt($3));
      $$->push(new Terminal(*$4));
    }
  | UNKNOWN_LEXIUM error {
      $$ = new VarDec(@1.first_line);
    }
  ;
FunDec:
    ID LP VarList RP {
      $$ = new FunDec(new ValId(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
    }
  | ID LP RP {
      $$ = new FunDec(new ValId(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push(new Terminal(*$3));
    }
  | ID LP VarList error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing parenthesis ')'" << std::endl;
      $$ = new FunDec(new ValId(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | ID LP error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing parenthesis ')'" << std::endl;
      $$ = new FunDec(new ValId(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
    }
  ;
VarList:
    ParamDec COMMA VarList {
      $$ = new VarList($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | ParamDec {
      $$ = new VarList($1, @1.first_line);
    }
  ;
ParamDec:
    Specifier VarDec {
      $$ = new ParamDec($1, @1.first_line);
      $$->push($2);
    }
  | VarDec error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing specifier" << std::endl;
      $$ = new ParamDec($1, @1.first_line);
    }
  ;

/**
 * Statement: program structures like branchs and loops
 * - enclosed bny curly braces
 * - end with a semicolon
 */
CompSt:
    LC DefList StmtList RC {
      $$ = new CompSt(new Terminal(*$1), @1.first_line);
      $$->push($2);
      $$->push($3);
      $$->push(new Terminal(*$4));
    }
  | LC DefList StmtList error {
    std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing curly bracket '}'" << std::endl;
      $$ = new CompSt(new Terminal(*$1), @1.first_line);
      $$->push($2);
      $$->push($3);
    }
  ;
StmtList:
    Stmt StmtList {
      $$=new StmtList($1, @1.first_line);
      $$->push($2);
    }
  | %empty { $$=new StmtList(-1);}
  | Stmt Def StmtList error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing specifier" << std::endl;
      $$=new StmtList($1, @1.first_line);
      $$->push($2);
      $$->push($3);
  }
  ;
Stmt:
    Exp SEMI {
      $$=new Stmt($1, @1.first_line);
      $$->push(new Terminal(*$2));
    }
  | CompSt {
      $$=new Stmt($1, @1.first_line);
    }
  | RETURN Exp SEMI {
      $$=new Stmt(new Terminal(*$1), @1.first_line);
      $$->push($2);
      $$->push(new Terminal(*$3));
    }
  | IF LP Exp RP Stmt %prec LOWER_ELSE {
      $$=new Stmt(new Terminal(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
      $$->push($5);
    }
  | IF LP Exp RP Stmt ELSE Stmt {
      $$=new Stmt(new Terminal(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
      $$->push($5);
      $$->push(new Terminal(*$6));
      $$->push($7);
    }
  | WHILE LP Exp RP Stmt {
      $$=new Stmt(new Terminal(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
      $$->push($5);
    }
  | Exp error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing semicolon ';'" << std::endl;
      $$=new Stmt($1, @1.first_line);
    }
  | RETURN Exp error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing semicolon ';'" << std::endl;
      $$=new Stmt(new Terminal(*$1), @1.first_line);
    }
  | RETURN UNKNOWN_LEXIUM error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing semicolon ';'" << std::endl;
      $$=new Stmt(new Terminal(*$1), @1.first_line);
    }
  | RETURN UNKNOWN_LEXIUM SEMI error {
      $$=new Stmt(new Terminal(*$1), @1.first_line);
    }
  ;

/* Local definition: declaration and assignment of local variables */
DefList:
    Def DefList {
      $$ = new DefList($1, @1.first_line);
      $$->push($2);
    }
  | %empty  { 
    $$ = new DefList(-1);
   }
  ;
Def:
    Specifier DecList SEMI {
      $$ = new Def($1, @1.first_line);
      $$->push($2);
      $$->push(new Terminal(*$3));
    }
  | Specifier DecList error {
    std::cerr << "Error type B at Line "<< @$.first_line << ": Missing semicolon ';'" << std::endl;
      $$ = new Def($1, @1.first_line);
      $$->push($2);
    }
  ;
DecList:
    Dec {
      $$ = new DecList($1, @1.first_line);
    }
  | Dec COMMA DecList {
      $$ = new DecList($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Dec COMMA error {
      $$ = new DecList($1, @1.first_line);
    }
  ;
Dec:
    VarDec {
      $$ = new Dec($1, @1.first_line);
    }
  | VarDec ASSIGN Exp {
      $$ = new Dec($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | VarDec ASSIGN UNKNOWN_LEXIUM error {
      $$ = new Dec($1, @1.first_line);
    }
  | VarDec ASSIGN error {
      $$ = new Dec($1, @1.first_line);
    }
  ;

/**
 * Expression
 * - single constant
 * - operations on variables: operators have precedence and associativity
 */
Exp:
    Exp ASSIGN Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp AND Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp OR Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp LT Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp LE Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp GT Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp GE Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp NE Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp EQ Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp PLUS Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp MINUS Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp MUL Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp DIV Exp {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | LP Exp RP {
      $$=new Exp(new Terminal(*$1), @1.first_line);
      $$->push($2);
      $$->push(new Terminal(*$3));
    }
  | MINUS Exp {
      $$=new Exp(new Terminal(*$1), @1.first_line);
      $$->push($2);
    }
  | NOT Exp {
      $$=new Exp(new Terminal(*$1), @1.first_line);
      $$->push($2);
    }
  | ID LP Args RP {
      $$ = new Exp(new ValId(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
    }
  | ID LP RP {
      $$ = new Exp(new ValChar(*$1), @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push(new Terminal(*$3));
    }
  | Exp LB Exp RB {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
      $$->push(new Terminal(*$4));
    }
  | Exp DOT ID {
      $$ = new Exp($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push(new ValId(*$3));
    }
  | ID {
      $$ = new Exp(new ValId(*$1), @1.first_line);
    }
  | INT {
      $$ = new Exp(new ValInt($1), @1.first_line);
    }
  | FLOAT {
      $$ = new Exp(new ValFloat($1), @1.first_line);
    }
  | CHAR {
      $$ = new Exp(new ValChar(*$1), @1.first_line);
    }
  | Exp UNKNOWN_LEXIUM Exp error {}
  | ID LP Args error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing parenthesis ')'" << std::endl;

    }
  | ID LP error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing parenthesis ')'" << std::endl;

    }
  | Exp LB Exp error {
      std::cerr << "Error type B at Line "<< @$.first_line << ": Missing closing bracket ']'" << std::endl;

    }
  ;
Args:
    Exp COMMA Args {
      $$ = new Args($1, @1.first_line);
      $$->push(new Terminal(*$2));
      $$->push($3);
    }
  | Exp {
      $$ = new Args($1, @1.first_line);
    }
  | Exp COMMA error {
      $$ = new Args($1, @1.first_line);
      $$->push(new Terminal(*$2));
    }
  ;

%%
int s_error=0;
void yyerror(const char *s) {
  s_error = 1;
}
ast_Top *g_root; // Definition of variable (to match declaration earlier)

ast_Top *parseAST() {
    g_root = new ast_Top;
    
    yyparse();
    if (s_error == 1) {
      exit(1);
    }
    return g_root;
}