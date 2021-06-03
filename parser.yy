%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define api.parser.class {parser}
%define api.token.constructor
%define api.namespace {yy}
%define api.value.type variant
%define parse.assert
%code requires
{
#include <string>
#include <stdio.h>
class parser_driver;
int yylex(void);
void yyerror(const char *);
extern int yylineno;
}
%param { parser_driver& driver }
%locations
%define parse.trace
%define parse.error verbose
%code
{
#include "driver.h"
#include "flags.h"
#include "semantic.cpp"
#include <iostream>
#include <vector>
#include <stack>

Scope scope = Scope();
int tempCount = 0;
ExpressionSolver es;
stack<ParamSolver> params;

stack<ExpressionSolver> pes;
Quad quads = Quad();
VarTable::symbolRow * var = NULL;
}
%define api.token.prefix {TOK_}


%token PUNTO COMA SCOL COL LCUR RCUR LBRA RBRA LPAR RPAR
%token EQ GE LE GT LT NE ASSIGN SUM RES MUL DIV MOD AND OR 
%token PROGRAMA CLASE VARIABLES MAIN METODOS HEREDA ATRIBUTOS FUNCION RETORNAR LEER ESCRIBIR
%token VOID INT_T FLOAT_T CHAR_T
%token SI SINO MIENTRAS PARA EN HACER ENTONCES

%token <std::string> ID
%token <std::string> STRING
%token <char> CHAR
%token <float> INT
%token <float> FLOAT
%token FIN 0 "eof"

%type program
%type empty
%type classes
%type variables
%type functions
%type main
%type methods
%type inheritance
%type attributes
%type var_type
%type return_type
%type parameters
%type statutes
%type assignment
%type expression
%type call_void
%type function_return
%type read
%type write
%type exp
%type term
%type factor
%type var_cte
%type decision_statement
%type repetition_statement
%type conditional
%type nonconditional


/* New */
%type declare_var
%type declare_bridge1
%type declare_bridge2
%type declare_bridge3
%type declare_bridge4
%type dimensions
%type dimensionsdecl
%type single_express
%type mult_express
%type primitive_type

/*Repeated*/
%type matrix
%type matrixdecl

/*Variables*/
%type var_bridge1
%type var_bridge2

/*Attributes*/
%type attr_bridge1
%type attr_bridge2

/*Call_Var*/
%type call_options
%type call_path

/*Parameters*/
%type par_array
%type par_cont

/*Functions*/
/*Write*/
%type write_expression_opt

/*Auxiliaries*/
%type dec_else
%type relop
%type express_loop
%type exp_loop
%type term_loop
%type sign
%type call
%type expression_loop

%printer { yyoutput << $$; } <*>;

%%
%start script;

script : program
;

empty : 
;

program : PROGRAMA ID {
                quads.prepareMainQuad();
        } COL classes {
                scope.freeClassScope();
                classFlag = 0;
                globalFlag = 1;
        } variables {
                globalFlag = 0;
                onClass = 0;
        } functions main 
;

/* DECLARE_VAR */
declare_var : ID {setID($1,0,0);} declare_bridge1 
declare_bridge1 : COMA ID {setID($2,0,0);} declare_bridge1 
                | declare_bridge2
declare_bridge2 : declare_bridge3
                | COL var_type
declare_bridge3 : dimensionsdecl declare_bridge4
declare_bridge4 : COL primitive_type 
                | COMA ID {setID($2,0,0);} declare_bridge4 
                | COMA ID {setID($2,0,0);} declare_bridge3 
;
/*END DECLARE_VAR */

/* DIMENSIONS */
dimensionsdecl : LBRA INT {currentIds.back().dim1 = $2;} matrixdecl RBRA
matrixdecl : COMA INT {currentIds.back().dim2 = $2;}
        | empty
;

dimensions : LBRA {
        int id = 0;
        if(paramsFlag){
                id = pes.top().popOprnd();
        }else{
                if(expressionFlag){
                        id = es.popOprnd();
                }else{
                        if(onClass && !classFlag)
                                classAssign = 1;
                        id = var->dir;
                }
        }
        if(onClass)
                params.push(ParamSolver(id,1));
        else{
                if(classFlag)
                        params.push(ParamSolver(id,1));
                else
                        params.push(ParamSolver(id,0));
        }
        if(onClass && !classFlag){
                scope.freeClassScope();
                onClass = 0;
        }
        paramsFlag = 1;
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
} expression {
        if(paramsFlag){
                quads.importSolver(pes.top());
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParamArray(single);
                        int dim = params.top().getDimension(paramNum);
                        quads.insertQuad(205,single,-1,dim);
                        if(params.top().getDimension(2) > 0){
                                int temp = params.top().generateArrayQuad();
                                int dimConstant = scope.addConstantINTVarDir(params.top().getDimension(2));
                                quads.insertQuad(2,single,dimConstant,temp);
                                matrixDir.push(temp);
                        }else{
                                int pointer = params.top().generatePointerQuad();
                                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                                quads.insertQuad(0,arrayDirConstant,single,pointer);
                        }
                }else{
                        int paramNum = params.top().insertParamArray(quads.getLastQuad().result);
                        int dim = params.top().getDimension(paramNum);
                        quads.insertQuad(205,quads.getLastQuad().result,-1,dim);
                        if(params.top().getDimension(2) > 0){
                                int temp = params.top().generateArrayQuad();
                                int dimConstant = scope.addConstantINTVarDir(params.top().getDimension(2));
                                quads.insertQuad(2,quads.getLastQuad().result,dimConstant,temp);
                                matrixDir.push(temp);
                        }else{
                                int pointer = params.top().generatePointerQuad();
                                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                                quads.insertQuad(0,arrayDirConstant,quads.getLastQuad().result,pointer);
                        }
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
} matrix RBRA {
        
        params.top().checkParams();
        if(params.size()<2){
                paramsFlag = 0; 
        }
        int pointer = quads.getLastQuad().result;
        
        if(params.size()>1){
                if(params.top().isClass() && !classFlag){
                        int temp = scope.insertVarOnScope("_"+to_string(tempCount++),params.top().getArrayType());
                        quads.insertQuad(5,quads.getLastQuad().result,-1,temp);
                        quads.insertQuad(207,-1,-1,-1);
                        pointer = temp;
                }
                pes.top().insertOprndDir(pointer);
        }else{
                if(expressionFlag){  
                        if(params.top().isClass() && !classFlag){
                                int temp = scope.insertVarOnScope("_"+to_string(tempCount++),params.top().getArrayType());
                                quads.insertQuad(5,quads.getLastQuad().result,-1,temp);
                                quads.insertQuad(207,-1,-1,-1);
                                pointer = temp;
                        }
                        es.insertOprndDir(pointer);
                }else{
                        var = scope.getGlobalVariable(pointer);
                }
        }

        params.pop();
}
matrix : COMA {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
} expression {
        if(paramsFlag){
                int result = matrixDir.top();
                quads.importSolver(pes.top()); 
                int single = pes.top().singleVar();
                int arrayDirConstant = scope.addConstantINTVarDir(params.top().getArrayDir());
                if(single){
                        int paramNum = params.top().insertParamArray(single);
                        int dim = params.top().getDimension(paramNum);
                        int temp = params.top().generateArrayQuad();
                        int pointer = params.top().generatePointerQuad();
                        quads.insertQuad(205,single,-1,dim);
                        quads.insertQuad(0,result,single,temp);
                        quads.insertQuad(0,arrayDirConstant,temp,pointer);
                }else{
                        int paramNum = params.top().insertParamArray(quads.getLastQuad().result);
                        int dim = params.top().getDimension(paramNum);
                        int temp = params.top().generateArrayQuad();
                        int pointer = params.top().generatePointerQuad();
                        quads.insertQuad(205,quads.getLastQuad().result,-1,dim);
                        quads.insertQuad(0,result,quads.getLastQuad().result,temp);
                        quads.insertQuad(0,arrayDirConstant,temp,pointer);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
}
        | empty
;
/*END DIMENSIONS */

/* EXPRESSES */
single_express : LPAR{
        if(paramsFlag){
                pes.top().installPar();
        }else{
                if(expressionFlag){
                        es.installPar();
                }
        }
        } expression RPAR {
                if(paramsFlag){
                        pes.top().closePar();
                }else{
                        if(expressionFlag){
                                es.closePar();
                        }
                }
        }
;

mult_express : LPAR {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
} expression {
        if(paramsFlag){
                quads.importSolver(pes.top()); 
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParam(single);
                        quads.insertQuad(203,single,-1,paramNum);
                }else{
                        int paramNum = params.top().insertParam(quads.getLastQuad().result);
                        quads.insertQuad(203,quads.getLastQuad().result,-1,paramNum);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
} expression_loop RPAR
                | LPAR empty RPAR
;
expression_loop : COMA {
        if(paramsFlag){
                pes.push(ExpressionSolver());
        }
} expression {
        if(paramsFlag){
                quads.importSolver(pes.top());
                int single = pes.top().singleVar();
                if(single){
                        int paramNum = params.top().insertParam(single);
                        quads.insertQuad(203,single,-1,paramNum);
                }else{
                        int paramNum = params.top().insertParam(quads.getLastQuad().result);
                        quads.insertQuad(203,quads.getLastQuad().result,-1,paramNum);
                }
                if(!classFlag){
                        int inst = pes.top().returnClassInstances();
                        while(inst > 0){
                                inst--;
                                quads.insertQuad(207,-1,-1,-1);
                        }
                }
                pes.pop();
        }
} expression_loop 
            | empty
;

classes : CLASE ID {
        scope.resetClassConts();
        classFlag = 1;
        setClassName($2);
        scope.globalInsertClass($2,10,quads.getQuadLastDir());
        scope.setClassScope($2);
} inheritance LCUR attributes methods RCUR SCOL classes 
        | empty
;

functions : return_type FUNCION ID {
        scope.resetLocalConts();
        if(classFlag){
                scope.insertClassFunction($3,currentType,quads.getQuadLastDir());
                scope.setClassFunctionScope($3);
                if(currentType != 20){
                        string varName = "_" + getClassName() + $3;
                        scope.insertGlobalVariable(varName,currentType);
                }
        }else{
                scope.globalInsertFunction($3,currentType,quads.getQuadLastDir());
                scope.setFunctionScope($3);
                if(currentType != 20){ 
                        scope.insertGlobalVariable($3,currentType);
                }
        }
        resetCurrentType();
} LPAR parameters RPAR variables LCUR statutes RCUR SCOL {
        quads.insertQuad(299,-1,-1,-1);
} functions
        | empty
;

main : MAIN {
        scope.resetLocalConts();
        quads.setMainQuad();
        scope.setFunctionScope("global");
} LPAR RPAR LCUR statutes RCUR {quads.insertQuad(999,-1,-1,-1);}
;

methods : METODOS functions
        | empty
;

inheritance : HEREDA ID
        | empty
;

/*VARIABLES*/

variables : VARIABLES var_bridge1
        | empty
;
var_bridge1 : declare_var SCOL 
{       
        
        if(globalFlag){
                for(auto var : currentIds){
                        scope.insertGlobalVariable(var.id, var.dim1,var.dim2,currentType);
                }
        }else{
                for(auto var : currentIds){
                        scope.insertVarOnScope(var.id, var.dim1,var.dim2,currentType);
                } 
        }
        
        resetID();
        resetCurrentType();
} 
var_bridge2 
;
var_bridge2 : var_bridge1
        | empty
;

/*FIN VARIABLES*/

/*ATTRIBUTES*/

attributes : ATRIBUTOS attr_bridge1
        | empty
attr_bridge1 : declare_var SCOL {
        if(classFlag){
                for(auto var : currentIds){
                        scope.insertVarOnClassScope(var.id, var.dim1,var.dim2,currentType);
                }
        }
        resetID();
        resetCurrentType();
} attr_bridge2
attr_bridge2 : attr_bridge1
        | empty
;

/*FIN ATTRIBUTES*/

primitive_type : INT_T {setCurrentType(0);}
                | FLOAT_T {setCurrentType(1);}
                | CHAR_T {setCurrentType(2);}
;

var_type : primitive_type | ID {setCurrentType(scope.getClassType($1));}
;

return_type : primitive_type | VOID {setCurrentType(20);}
;

call_path : ID {
        VarTable::symbolRow * variable = NULL;
        if(!onClass)
                variable = scope.getGlobalVariable($1);
        else
                variable = scope.getClassVariable($1);
        if(variable && variable->type >= 100){
                scope.setClassScopeID(variable->type);
                quads.insertQuad(206,variable->type,variable->dir,-1);
                onClass = 1;
        }else{
                if((onClass ? scope.getClassFunction($1) : scope.globalGetFunction($1))){
                        if(onClass){
                                params.push(ParamSolver($1,1));
                                onClass = 0;
                        }else{
                                params.push(ParamSolver($1,0));
                        }
                        paramsFlag = 1;
                        quads.insertQuad(202,params.top().getFunctionQuadDir(),-1,-1);    
                }else{
                        if(paramsFlag){
                                pes.top().insertOprnd($1);
                                if(onClass){
                                        int dir = pes.top().popOprnd();
                                        if(variable->dimension1 > 0){
                                                pes.top().insertOprnd($1);
                                        }else{
                                                int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++),var->type);
                                                quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                quads.insertQuad(5,dir,-1,dir2);
                                                pes.top().insertOprndDir(dir2);
                                                pes.top().insertClassInstance();
                                                onClass = 0;
                                                scope.freeClassScope();
                                        }
                                }
                        }else{
                                if(expressionFlag){
                                        es.insertOprnd($1);
                                        if(onClass){
                                                int dir = es.popOprnd();
                                                if(variable->dimension1 > 0){
                                                        es.insertOprnd($1);
                                                }else{
                                                        int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++)+var->name,var->type);
                                                        quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                        quads.insertQuad(5,dir,-1,dir2);
                                                        es.insertOprndDir(dir2);
                                                        onClass = 0;
                                                        es.insertClassInstance();
                                                        scope.freeClassScope();
                                                }
                                        }
                                }else{
                                        var = scope.getVarOnScope($1);
                                        if(var == NULL && scope.classScope)
                                                var = scope.getClassVariable($1);
                                        if(var == NULL)
                                                var = scope.getGlobalVariable($1);
                                        if(onClass){
                                                int dir2 = scope.insertVarOnScope("_"+to_string(tempCount++),var->type);
                                                quads.updateQuadResult(quads.getQuadLastDir()-1,dir2);
                                                classAssign = 1;
                                                if(var->dimension1 <= 0)
                                                        quads.insertQuad(5,var->dir,-1,dir2);
                                        }
                                }
                        }
                }
        }
} call_options

call_options : PUNTO {if(paramsFlag && params.size()<1)throw CompilerError("Error: No parameters inserted");} call_path 
        | dimensions call_options 
        | mult_express {
                params.top().checkParams();
                if(params.size()<2){
                        paramsFlag = 0; 
                } 
                quads.insertQuad(204,params.top().getFunctionGlobalDir(),-1,-1);
                if(params.top().getFunctionGlobalDir() != -1){
                        int dir = params.top().generateReturnQuad();
                        quads.insertQuad(5,params.top().getFunctionGlobalDir(),-1,dir);
                        if(params.size()>1){
                                pes.top().insertOprndDir(dir);
                        }else{
                                if(expressionFlag){
                                        es.insertOprndDir(dir);
                                }
                        }
                }
                if(params.top().isClass()){
                        quads.insertQuad(207,-1,-1,-1);
                }
                params.pop();
                scope.freeClassScope();
        }
        | empty
;



parameters : ID {setID($1,0,0);} par_array COL var_type 
{
        scope.insertParameterOnScope($1,currentType);
        resetCurrentType();
        resetID();
} par_cont 
        | empty

par_cont : COMA parameters
        | empty

par_array : dimensions
        | empty
;


statutes : call_void statutes
        | assignment statutes
        | function_return statutes
        | read statutes
        | write statutes
        | decision_statement statutes
        | repetition_statement statutes
        | empty
;

assignment : call_path ASSIGN 
{
        es = ExpressionSolver();
        expressionFlag = 1;
        es.insertOprnd(var->name);
        es.insertOptr(5);
        if(onClass){
                setClassName(scope.freeClassScope());
                onClass = 0;
        }
}
expression SCOL 
{
        es.checkStacks();
        if(!classFlag && classAssign){
                scope.setClassScope(getClassName());
                quads.importSolver(es);
                scope.freeClassScope();
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }else{
                quads.importSolver(es);
        }
        if(classAssign){
                quads.insertQuad(207,-1,-1,-1);
        }
        classAssign = 0;
        onClass = 0;
        expressionFlag = 0;
}
;


call_void : call_path SCOL
;

function_return : RETORNAR{
        es = ExpressionSolver();
        expressionFlag = 1;
} expression SCOL {
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        if(single){
                es.checkVarType(single);
                int var = scope.getScopeDir();
                quads.insertQuad(298,single,-1,var);
        }else{
                es.checkVarType(quads.getLastQuad().result);
                int var = scope.getScopeDir();
                quads.insertQuad(298,quads.getLastQuad().result,-1,var);
        }
        if(!classFlag){
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }
}
;

read : LEER LPAR call_path RPAR SCOL { 
        quads.insertQuad(103,-1,-1,var->dir);
        if(onClass){
                quads.insertQuad(207,-1,-1,-1);
                onClass = 0;
        }
}
;

write : ESCRIBIR LPAR { es = ExpressionSolver(); expressionFlag = 1;} write_expression_opt RPAR SCOL
write_expression_opt : STRING {
                        expressionFlag = 0;
                        int text = scope.addConstantSTRINGVarDir($1);
                        quads.insertQuad(102,text,-1,-1);
                        int newLine = scope.addConstantSTRINGVarDir("\n");
                        quads.insertQuad(102,newLine,-1,-1);
                        }
                | STRING {
                        expressionFlag = 0;
                        int text = scope.addConstantSTRINGVarDir($1);
                        quads.insertQuad(102,text,-1,-1);
                        expressionFlag = 1;
                } COMA write_expression_opt
                | expression {
                        quads.importSolver(es); 
                        expressionFlag = 0;
                        int single = es.singleVar();
                        if(single){
                                quads.insertQuad(102,single,-1,-1);
                        }else{
                                quads.insertQuad(102,quads.getLastQuad().result,-1,-1);
                        }
                        if(!classFlag){
                                int inst = es.returnClassInstances();
                                while(inst > 0){
                                        inst--;
                                        quads.insertQuad(207,-1,-1,-1);
                                }
                        }
                        int newLine = scope.addConstantSTRINGVarDir("\n");
                        quads.insertQuad(102,newLine,-1,-1);
                }
                | expression COMA {
                        quads.importSolver(es); 
                        expressionFlag = 0;
                        int single = es.singleVar();
                        if(single){
                                quads.insertQuad(102,single,-1,-1);
                        }else{
                                quads.insertQuad(102,quads.getLastQuad().result,-1,-1);
                        }
                        if(!classFlag){
                                int inst = es.returnClassInstances();
                                while(inst > 0){
                                        inst--;
                                        quads.insertQuad(207,-1,-1,-1);
                                }
                        }
                        es = ExpressionSolver(); 
                        expressionFlag = 1;
                } write_expression_opt
;

decision_statement : SI LPAR {paramsFlag = 0; es = ExpressionSolver(); expressionFlag = 1;} expression RPAR {
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        setJDir(quads.getQuadLastDir());
        if(single){
                quads.insertQuad(201,single,-1,-1);
        }else{
                quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
        }
}ENTONCES LCUR statutes RCUR dec_else
;
dec_else : SINO { 
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir()+1); 
                setJDir(quads.getQuadLastDir());
                quads.insertQuad(200,-1,-1,-1);
        } LCUR statutes RCUR {
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir());
        }
        | empty {
                int dir = getJDir();
                quads.updateQuadResult(dir,quads.getQuadLastDir()); 
        }
;

repetition_statement : conditional | nonconditional
;

expression : exp relop exp {es.checkStacks();} express_loop 
        | exp 
relop : EQ {if(paramsFlag){pes.top().insertOptr(6);}else{ if(expressionFlag){es.insertOptr(6);}}}
      | NE {if(paramsFlag){pes.top().insertOptr(7);}else{ if(expressionFlag){es.insertOptr(7);}}}
      | GE {if(paramsFlag){pes.top().insertOptr(10);}else{ if(expressionFlag){es.insertOptr(10);}}}
      | LE {if(paramsFlag){pes.top().insertOptr(11);}else{ if(expressionFlag){es.insertOptr(11);}}}
      | GT {if(paramsFlag){pes.top().insertOptr(8);}else{ if(expressionFlag){es.insertOptr(8);}}}
      | LT {if(paramsFlag){pes.top().insertOptr(9);}else{ if(expressionFlag){es.insertOptr(9);}}}
express_loop : AND {if(paramsFlag){pes.top().insertOptr(12);}else{ if(expressionFlag){es.insertOptr(12);}}} expression {if(paramsFlag){pes.top().checkStacks();}else{ if(expressionFlag){es.checkStacks();}}}
              | OR {if(paramsFlag){pes.top().insertOptr(13);}else{ if(expressionFlag){es.insertOptr(13);}}} expression {if(paramsFlag){pes.top().checkStacks();}else{ if(expressionFlag){es.checkStacks();}}}
              | empty
;

exp : term {
        if(paramsFlag){
                if(pes.top().operators.size() > 0){
                        if(pes.top().operators.top() == 0 || pes.top().operators.top() == 1){
                                pes.top().checkStacks();
                        }
                }
        }else{ 
                if(expressionFlag){
                        if(es.operators.size() > 0){
                                if(es.operators.top() == 0 || es.operators.top() == 1){
                                        es.checkStacks();
                                }
                        }
                }
        }
        } exp_loop 
exp_loop : SUM {if(paramsFlag){pes.top().insertOptr(0);}else{ if(expressionFlag){es.insertOptr(0);}}} exp
         | RES {if(paramsFlag){pes.top().insertOptr(1);}else{ if(expressionFlag){es.insertOptr(1);}}} exp
         | empty
;

term : factor {
        if(paramsFlag){
                if(pes.top().operators.size() > 0){
                        if(pes.top().operators.top() == 2 || pes.top().operators.top() == 3 || pes.top().operators.top() == 4){
                                pes.top().checkStacks();
                        }
                }
        }else{
                if(expressionFlag){
                        if(es.operators.size() > 0){
                                if(es.operators.top() == 2 || es.operators.top() == 3 || es.operators.top() == 4){
                                        es.checkStacks();
                                }
                        }
                }
        }
        } term_loop
term_loop : MUL {if(paramsFlag){pes.top().insertOptr(2);}else{if(expressionFlag){es.insertOptr(2);}}} term 
          | DIV {if(paramsFlag){pes.top().insertOptr(3);}else{if(expressionFlag){es.insertOptr(3);}}} term
          | MOD {if(paramsFlag){pes.top().insertOptr(4);}else{if(expressionFlag){es.insertOptr(4);}}} term
          | empty
;

factor : single_express
        | sign call 
sign : SUM
     | RES
     | empty
call : var_cte
     | call_path
;

var_cte : INT {if(paramsFlag){pes.top().insertOprnd((int)$1);}else{ if(expressionFlag){es.insertOprnd((int)$1);}}}
        | FLOAT {if(paramsFlag){pes.top().insertOprnd((float)$1);}else{ if(expressionFlag){es.insertOprnd((float)$1);}}}
        | CHAR  {if(paramsFlag){pes.top().insertOprnd((char)$1);}else{ if(expressionFlag){es.insertOprnd((char)$1);}}}
;

conditional : MIENTRAS LPAR {paramsFlag = 0; es = ExpressionSolver(); expressionFlag = 1;} expression RPAR {
        setJDir(quads.getQuadLastDir());
        quads.importSolver(es); 
        expressionFlag = 0;
        int single = es.singleVar();
        setJDir(quads.getQuadLastDir());
        if(!classFlag){
                int inst = es.returnClassInstances();
                while(inst > 0){
                        inst--;
                        quads.insertQuad(207,-1,-1,-1);
                }
        }
        if(single){
                quads.insertQuad(201,single,-1,-1);
        }else{
                quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
        }
        
}HACER LCUR statutes RCUR {
        int dir = getJDir();
        int exp = getJDir();
        quads.insertQuad(200,-1,-1,exp);
        quads.updateQuadResult(dir,quads.getQuadLastDir());
}
;

nonconditional : PARA call_path {
        if(onClass){
                quads.insertQuad(207,-1,-1,-1);
                onClass = 0;
        }
} ASSIGN {
        es = ExpressionSolver(); 
        paramsFlag = 0;
        expressionFlag = 1;
        setforID(var->name);
        es.insertOprnd(var->name);
        es.insertOptr(5);
} exp {
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0;
        string assign = var->name;
        es = ExpressionSolver(); 
        expressionFlag = 1;
        es.insertOprnd(assign);
} EN { es.insertOptr(11);} exp {
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0; 
        int lastDir = quads.getQuadLastDir();
        setJDir(lastDir);
        quads.insertQuad(201,quads.getLastQuad().result,-1,-1);
}HACER LCUR statutes RCUR {
        string fid = getforID();
        es = ExpressionSolver(); 
        expressionFlag = 1;
        es.insertOprnd(fid);
        es.insertOptr(5);
        es.insertOprnd(fid);
        es.insertOptr(0);
        es.insertOprnd(1);
        es.checkStacks();
        es.checkStacks();
        quads.importSolver(es); 
        expressionFlag = 0;

        int dir = getJDir();
        quads.insertQuad(200,-1,-1,dir-1);
        quads.updateQuadResult(dir,quads.getQuadLastDir());
}
;

%%
void yy::parser::error(const location_type& lugar, const std::string& lexema)
{

  throw CompilerError("Line: "+ std::to_string(yylineno) + " :: " + lexema);
}