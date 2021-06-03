%{
    #include <cerrno>
    #include <stdio.h>
    #include <string>
    #include "driver.h"
    #include "parser.tab.hh"
    #undef yywrap
    #define yywrap() 1
    static location loc;
%}

%option noyywrap nounput batch debug noinput
%option outfile="Scanner.cpp"
%option yylineno

COMENTARIOMULT "/*"(.|\n)*"*/"
COMENTARIOSINGLE "//".*([\r\n])
LETRA [a-zA-Z]
DIGITO [0-9]
PROGRAMA "Program"
CLASE "Class"
VARIABLES "variables"
ATRIBUTOS "attributes"
MAIN "main"
METODOS "methods"
HEREDA "Inherits"
FUNCION "function"
RETORNAR "return"
LEER "read"
ESCRIBIR "write"
COMILLAS ["]
COMILLA [']
GBAJO [_]
ID {LETRA}({LETRA}|{DIGITO}|{GBAJO})*
COMA ","
SCOL ";"
COL  ":"
PUNTO  "."
VOID "void"
INT_T "int"
FLOAT_T "float"
CHAR_T "char"
LCUR "{"
RCUR "}"
LBRA "["
RBRA "]"
LPAR  "("
RPAR  ")"
ASSIGN "="
EQ  "=="
GT  ">"
LT  "<"
GE  ">="
LE  "<="
NE  "!="
SUM  "+"
RES  "-"
MUL  "*"
DIV  "/"
MOD  "%"
AND "&"
OR "|"
INT {DIGITO}+
FLOAT {DIGITO}*{PUNTO}{DIGITO}+
DELIM [" "\n\t]
CHAR {COMILLA}({LETRA}|{DIGITO}|{DELIM})?{COMILLA}
STRING {COMILLAS}(\\.|[^"\\])*{COMILLAS}
SI "if"
SINO "else"
ENTONCES "then"
MIENTRAS "while"
PARA "for"
EN "in"
HACER "do"

%%
{COMENTARIOSINGLE} {}
{COMENTARIOMULT} {}
{PROGRAMA}  { return parser::make_PROGRAMA(loc);}
{CLASE}     { return parser::make_CLASE(loc);}
{VARIABLES} { return parser::make_VARIABLES(loc);}
{MAIN}      { return parser::make_MAIN(loc);}
{METODOS}   { return parser::make_METODOS(loc);}
{HEREDA}    { return parser::make_HEREDA(loc);}
{ATRIBUTOS} { return parser::make_ATRIBUTOS(loc);}
{FUNCION}   { return parser::make_FUNCION(loc);}
{COMA}      { return parser::make_COMA(loc);}
{PUNTO}     { return parser::make_PUNTO(loc);}
{SCOL}      { return parser::make_SCOL(loc);}
{COL}       { return parser::make_COL(loc);}
{LCUR}      { return parser::make_LCUR(loc);}
{RCUR}      { return parser::make_RCUR(loc);}
{LBRA}      { return parser::make_LBRA(loc);}
{RBRA}      { return parser::make_RBRA(loc);}
{LPAR}      { return parser::make_LPAR(loc);}
{RPAR}      { return parser::make_RPAR(loc);}
{EQ}        { return parser::make_EQ(loc);}
{GE}        { return parser::make_GE(loc);}  
{LE}        { return parser::make_LE(loc);}
{GT}        { return parser::make_GT(loc);}  
{LT}        { return parser::make_LT(loc);}
{NE}        { return parser::make_NE(loc);}
{ASSIGN}    { return parser::make_ASSIGN(loc);}
{SUM}       { return parser::make_SUM(loc);}
{RES}       { return parser::make_RES(loc);}
{MUL}       { return parser::make_MUL(loc);}
{DIV}       { return parser::make_DIV(loc);}
{MOD}       { return parser::make_MOD(loc);}
{AND}       { return parser::make_AND(loc);}
{OR}        { return parser::make_OR(loc);}
{INT}       { return parser::make_INT(strtol (yytext, NULL, 10),loc);} 
{FLOAT}     { return parser::make_FLOAT(std::stof(yytext),loc);} 
{CHAR}      { return parser::make_CHAR((char)yytext[1],loc);}
{STRING}    { return parser::make_STRING(yytext,loc);}
{DELIM}     {}
{RETORNAR}  {return parser::make_RETORNAR(loc);}
{LEER}      {return parser::make_LEER(loc);}
{ESCRIBIR}  {return parser::make_ESCRIBIR(loc);}
{VOID}      {return parser::make_VOID(loc);}
{INT_T}     {return parser::make_INT_T(loc);}
{FLOAT_T}   {return parser::make_FLOAT_T(loc);}
{CHAR_T}    {return parser::make_CHAR_T(loc);}
{SI}        {return parser::make_SI(loc);}
{SINO}      {return parser::make_SINO(loc);}
{ENTONCES}  {return parser::make_ENTONCES(loc);}
{MIENTRAS}  {return parser::make_MIENTRAS(loc);}
{PARA}      {return parser::make_PARA(loc);}
{EN}        {return parser::make_EN(loc);}
{HACER}     {return parser::make_HACER(loc);}
{ID}        {return parser::make_ID(yytext,loc);}
<<EOF>>     {return parser::make_FIN(loc);}
%%

void parser_driver::scannerInit(){
  yy_flex_debug = false;
  if(file.empty() || file == "-"){
    printf("File not found !");
    exit(1);
  }else{
    yyin = fopen (file.c_str (), "r");
  }
}

void parser_driver:: scannerEnd(){
  fclose(yyin);
}