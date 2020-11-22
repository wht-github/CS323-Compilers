%{ 
    #include "lex.yy.c"
    #include "myjson.hpp"
    using std:cerr, std::endl, std::cin;
    void yyerror(const char*);

    JsonObject *root;
%}

%union {
  double number;
  string *s;
  int boolean;
  JsonObject *jsonObj;
  ObjectMember *objMem;
  ArrayValue *arrVal;
}

%token <number> NUMBER
%token <s> STRING
%token <boolean> TRUE FALSE
%token VNULL
%token LC RC LB RB COLON 
%token ERROR_NUMBER

%left LOWER_COMMA
%left COMMA

%type <jsonObj> Json Object Array
%type <objMem> Members Member
%type <arrVal> Values Value

%%

Json:
      %empty {
        root = NULL;
      }
    | Value {
        JsonObject *json = new JsonObject();
        json->category = Category::ARRAY;
        json->val = $1;
        $$ = json;
        root = $$;
      }
    | Value COMMA error {
        cerr << "Comma after the close, recovered" << endl;
      }
    ;
Value:
      Object {
        ArrayValue *val = new ArrayValue();
        val->value = $1;
        val->next = nullptr;
        $$ = val;
      }
    | Array {
        ArrayValue *val = new ArrayValue();
        val->value = $1;
        val->next = nullptr;
        $$ = val;
      }
    | STRING {
      }
    | NUMBER {
      }
    | ERROR_NUMBER error {
        puts("Numbers cannot have leading zeroes, recovered");
      }
    | TRUE {
        JsonObject *tru =new JsonObject();
        tru->category = Category::BOOLEAN;
        tru->val.boolean = $1;
        ArrayValue *val = new ArrayValue();
        val->values.push(tru);
        $$ = val;
      }
    | FALSE {
        JsonObject *fal = new JsonObject();
        fal->category = Category::BOOLEAN;
        fal->val.boolean = $1;
        struct ArrayValue *val = (struct ArrayValue *)malloc(sizeof(struct ArrayValue));
        val->value = fal;
        val->next = NULL;
        $$ = val;
      }
    | VNULL {
        JsonObject *nil = new JsonObject();
        nil->category = Category::NIL;
        ArrayValue *val = new ArrayValue();
        val->values.push(nil);
        $$ = val;
      }
    ;
Object:
      LC RC {
        struct JsonObject *obj =new JsonObject();
        obj->category = Category::OBJECT;
        obj->val.members = NULL;
        $$ = obj;
      }
    | LC Members RC {
        JsonObject *obj = new JsonObject();
        obj->category = Category::OBJECT;
        obj->val.members = $2;
        $$ = obj;
      }
    | LC Members RC Value error {
        puts("Extra value after close, recovered");
      }
    | LC Members COMMA error {
        puts("Comma instead of closing brace, recovered");
      }
    ;
Members:
      Member %prec LOWER_COMMA {
        $$ = $1;
      }
    | Member COMMA Members {
        $1->map.insert($3->map.begin(),$3->map.end());
        $$ = $1;
      }
    | Member COMMA error {
        puts("Extra comma, recovered");
      }
    ;
Member:
      STRING COLON Value {
        ObjectMember *mem = new ObjectMember();
        mem->map.insert(make_pair($1,$3));
        $$ = mem;
      }
    | STRING Value error {
        puts("Missing colon, recovered");
      }
    | STRING COMMA Value error {
        puts("Comma instead of colon, recovered");
      }
    | STRING COLON COLON Value error {
        puts("Double colon, recovered");
      }
    ;
Array:
      LB RB {
        JsonObject *arr = new JsonObject();
        arr->category = Category::ARRAY;
        arr->val = nullptr;
        $$ = arr;
      }
    | LB Values RB {
        JsonObject *arr = new JsonObject();
        arr->category = Category::ARRAY;
        arr->val = $2;
        $$ = arr;
      }
    | LB Values error {
        puts("Unclosed array, recovered");
      }
    | LB Values RC error {
        puts("Unmatched right bracket, recovered");
      }
    ;
Values:
      Value {
        $$ = $1;
      }
    | Value COMMA Values {
        $3->values.push($1);
        $$ = $3;
      }
    | Value COMMA error {
        puts("Extra comma, recovered");
      }
    | Value COMMA COMMA error {
        puts("Double extra comma, recovered");
      }
    | Value COLON Values error {
        puts("Colon instead of comma, recovered");
      }
    | COMMA Values error {
        puts("Missing value, recovered");
      }
    ;
%%


void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    } else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();

}