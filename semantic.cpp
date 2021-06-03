#include <stdio.h>
#include <string>
#include "driver.h"
#include <stack>
#include <vector>
#include <iostream>

using namespace std;

//Declaración anticipada de clases
class FunctionTable;
class VarTable;
class Scope;
class ExpressionSolver;
class Quad;

// Variables declaradas como externas para poder ser inicializada en el PARSER
// y poder ser compartidas entre la semantica y lo sintactico
extern Scope scope;
extern Quad quads;
extern int tempCount;

// Cubo semantico declarado
static SemanticConsideration sc = SemanticConsideration();

//Tabla de variables heredada de SymbolTable que genera un mapa de variables
//Cada renglón de variable puede guardar diferentes variables y su respectivo valor
class VarTable : public SymbolTable{
  public:
    struct symbolRow{
      string name;
      int type; //0->Int, 1->float, 2->char, 3->error, 4->Quad
      int dir;
      int dimension1 = 0;
      int dimension2 = 0;
    };

    // Inserta un symbolRow dentro de la lista de la clase 
    void insert(struct symbolRow * row){
      if(lookup(row)){
        throw CompilerError("Error: " + (*row).name + " it's already declared!");
      }
      VarTable::umap[(*row).name] = row;
      VarTable::umapDir[(*row).dir] = row;
    }

    // Busca un symbolRow dentro de la lista de la clase 
    int lookup(symbolRow * row){
      symbolRow * exist = VarTable::umap[(*row).name];
      if(exist != NULL){
        return 1;
      }
      return 0;
    }
    // Regresa un symbolRow de la lista de la clase según el nombre del renglón
    symbolRow * find(string row){
      symbolRow * exist = VarTable::umap[row];
      if(exist != NULL){
        return exist;
      }
      return NULL;
    }

    // Regresa un symbolRow de la lista de la clase según el número del renglón
    symbolRow * find(int row){
      symbolRow * exist = VarTable::umapDir[row];
      if(exist != NULL){
        return exist;
      }
      return NULL;
    }
    
    // Modifica los atributos de un symbolRow (tipo y dirección) dentro de la lista de la clase
    void modify(struct symbolRow * row){
      struct symbolRow * exist = VarTable::umap[(*row).name];
      if(exist != NULL){
        (*exist).type = (*row).type;
        (*exist).dir = (*row).dir;
      }
    }
    
    // Regresa el hashmap de symbolRows de las variables referenciadas por indices
    unordered_map<int, symbolRow*> getList(){
      return umapDir;
    }

  protected:
    // Hashmap de symbolRows indexado por una dirección de tipo string (Nombre de la variable)
    unordered_map<string, symbolRow*> umap;
    // Hashmap de symbolRows indexado por una dirección de tipo entero (Dirección de la variable)
    unordered_map<int, symbolRow*> umapDir;
};

// Tabla de funciones heredada de SymbolTable que genera un mapa de funciones
// Cada renglón de función puede guardar una tabla de clase o una tabla de variables
class FunctionTable : public SymbolTable{
  public:
    
    //Estructura encargada de almacenar información 
    struct symbolRow{
      string name;
      int return_type;//0->int 1->float 2->char 20->void 100+-> class
      VarTable * varTable;
      FunctionTable * classFunctions;
      vector<VarTable::symbolRow> parameters;
      int functionId = -1;
      int ints = 0;
      int floats = 0;
      int chars = 0;
      //int objects = 0;
      int quadDir = -1;
    };
  
    // Inserta un symbolRow dentro de la lista de la función
    void insert(struct symbolRow * row){
      if(lookup(row)){
        throw CompilerError("Error: " + row->name + " it's already declared!");
      }
      
      umap[(*row).name] = row;
    }

    // Busca un symbolRow dentro de la lista de la función 
    int lookup(symbolRow * row){
      symbolRow * exist = FunctionTable::umap[(*row).name];
      if(exist != NULL){
        return 1;
      }
      return 0;
    }
    
    // Regresa un symbolRow de la lista de la función según el nombre del renglón
    symbolRow * find(string row){
      symbolRow * exist = umap[row];
      if(exist != NULL){
        return exist;
      }
      return NULL;
    }

    // Modifica los atributos de un symbolRow (tipo y dirección) dentro de la lista de la funciones
    void modify(struct symbolRow * row){
      struct symbolRow * exist = FunctionTable::umap[(*row).name];
      if(exist != NULL){
        (*exist).return_type = (*row).return_type;
        (*exist).varTable = (*row).varTable;
        (*exist).classFunctions = (*row).classFunctions;
      }
    }
  protected:
    unordered_map<string, symbolRow*> umap;
};

//Clase central donde se maneja toda la semantica
//
class Scope{
  public:
    
    //Listas Hash de variables constantes (int, float, char y string)
    unordered_map<int,int> cte_ints;
    unordered_map<float,int> cte_floats;
    unordered_map<char,int> cte_chars;
    unordered_map<string,int> cte_strings;
    
    //Listas de apuntadores y direcciones de clases
    unordered_map<int,int> pointers;
    unordered_map<int,int> classDirs;
    
    //FunctionTable global encargado de guardar variables globales
    FunctionTable globalTable;

    //FunctionTable variable que se asigna cuando se entra a una función
    FunctionTable * currentTable;
    
    //Apuntadores symbolRow para el scope de la función y de la clase
    FunctionTable::symbolRow * functionScope = NULL;
    FunctionTable::symbolRow * classScope = NULL;

    vector<FunctionTable::symbolRow *> functions;
    vector<FunctionTable::symbolRow *> classes;

    //Contadores para el manejo de psuedo direcciones
    //1000 - 10,000 = Global ; 10,001 - 20,000 = Local ; 20,001 - 31,000 = Constantes
    //40,001 - 45000 Variables clases ; 40,001 - 50,000 Objetos ; 50,001 - 55,000, Apuntadores ;
    
    int globalIntCont    =  1000;
    int globalFloatCont  =  6001;
    int globalCharCont   =  9001;
    int localIntCont     = 10001;
    int localFloatCont   = 16001;
    int localCharCont    = 19001;
    int constanIntCont   = 20001; 
    int constanFloatCont = 26001; 
    int constanCharCont  = 29001; 
    int constanStringCont = 30001;

    int classIntCont = 31001;
    int classFloatCont = 36001;
    int classCharCont = 39001;

    int globalObjectCont = 40001;
    int localObjectCont = 45001;

    int pointersCont = 50001;
    
    Scope(){
      currentTable = &globalTable;
      globalInsertFunction("global",20,0);
    };

    // Reestablece las direcciones de los contadores locales
    void resetLocalConts(){
      localIntCont     = 10001;
      localFloatCont   = 16001;
      localCharCont    = 19001;
      localObjectCont = 45001;
    }

    // Reestablece las direcciones de los contadores locales
    void resetClassConts(){
      classIntCont     = 31001;
      classFloatCont   = 36001;
      classCharCont    = 39001;
    }

    // Incrementa la direcciones de las variables globales
    int addGlobalVarDir(int type){
      switch (type){
      case 0:
        return globalIntCont++;
      case 1:
        return globalFloatCont++;
      case 2:
        return globalCharCont++;
      default:
        if(type>=100){
          return globalObjectCont++;
        }
        return -1;
      }
    }

    // Incrementa las direcciones de las variables locales
    int addLocalVarDir(int type){
      switch (type){
      case 0:
        functionScope->ints++;
        return localIntCont++;
      case 1:
        functionScope->floats++;
        return localFloatCont++;
      case 2:
        functionScope->chars++;
        return localCharCont++;
      default:
        if(type>=100){
          return globalObjectCont++;
        }
        return -1;
      }
    }
    
    // Incrementa las direcciones de las clases
    int addClassVarDir(int type){
      switch (type){
      case 0:
        classScope->ints++;
        return classIntCont++;
      case 1:
        classScope->floats++;
        return classFloatCont++;
      case 2:
        classScope->chars++;
        return classCharCont++;
      default:
        return -1;
      }
    }

    // Incrementa las direcciones de los apuntadores
    int addPointerBaseVar(int type){
      if(pointersCont + 1 > 60000)
        throw CompilerError("Error: Memory leak, variables count exceeded");
      pointers[pointersCont] = type;
      return pointersCont++;
    }

    // Verifica si esta la constante entera, si no está la agrega a la lista
    int addConstantINTVarDir(int value){
      if(constanIntCont + 1 > 26000)
        throw CompilerError("Error: Memory leak, variables count exceeded");
      int num = cte_ints[value];
      if(num)
        return cte_ints[value];
      cte_ints[value] = constanIntCont;
      return constanIntCont++;
    }
    
    // Verifica si esta la constante flotante, si no está la agrega a la lista
    int addConstantFLOATVarDir(float value){
      if(constanFloatCont + 1 > 29000)
        throw CompilerError("Error: Memory leak, variables count exceeded");
      int num = cte_floats[value];
      if(num)
        return cte_floats[value];
      cte_floats[value] = constanFloatCont;
      return constanFloatCont++;
    }

    // Verifica si esta la constante carácter, si no está la agrega a la lista
    int addConstantCHARVarDir(char value){
      if(constanCharCont + 1 > 30000)
        throw CompilerError("Error: Memory leak, variables count exceeded");
      int num = cte_chars[value]; 
      if(num)
        return cte_chars[value];
      cte_chars[value] = constanCharCont;
      return constanCharCont++;
    }
    
    // Verifica si esta la constante string, si no está la agrega a la lista
    int addConstantSTRINGVarDir(string value){
      if(constanStringCont + 1 > 31000)
        throw CompilerError("Error: Memory leak, variables count exceeded");
      string valueNew = value;
      if(value != "\n")
        valueNew = value.substr(1,value.length()-2);
      int num = cte_strings[valueNew]; 
      if(num)
        return cte_strings[valueNew];
      cte_strings[valueNew] = constanStringCont;
      return constanStringCont++;
    }
    
    ////////////////////
    //FUNCIONES GLOBALES
    ////////////////////
    
    // Inserta función dentro del directorio de funciónes globales
    int globalInsertFunction(string name, int type, int quadDir){
      FunctionTable::symbolRow * frow = new FunctionTable::symbolRow();
      frow->name = name;
      frow->return_type = type;
      frow->varTable = new VarTable();
      frow->classFunctions = NULL;
      frow->quadDir = quadDir;
      frow->functionId = functions.size();
      frow->ints = 0;
      frow->floats = 0;
      frow->chars = 0;
      globalTable.insert(frow);

      functions.push_back(frow);

      return 1;
    }
    
    // Regresa si existe la función o no dentro del directorio de funciones globales
    // int globalFindFunction(string row){
    //   FunctionTable::symbolRow * exist = globalTable.find(row);
    //   if(exist == NULL){
    //     throw CompilerError("Error, function doesn't exist.");
    //   }
    //   return 1;
    // }

    // Regresa la lista de funciones dentro del directorio de funciones globales
    // Se inserta un string y se busca en el hashmap
    FunctionTable::symbolRow * globalGetFunction(string row){
      FunctionTable::symbolRow * exist = globalTable.find(row);
      if(exist == NULL){
        return NULL;
      }
      return exist;
    }

    // Regresa la lista de funciones dentro del directorio de funciones globales
    // Se inserta un int y se busca en el vector de funciones
    FunctionTable::symbolRow * globalGetFunction(int row){
      if(row >= functions.size())
        throw CompilerError("Error, function doesn't exist.");

      FunctionTable::symbolRow * exist = functions[row];
      if(exist == NULL){
        throw CompilerError("Error, function doesn't exist.");
      }
      return exist;
    }
  
    // Verifica que la variable exista dentro de la tabla de variables globales
    int lookupGlobalVariable(string name){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }

      return 1;
    }

    // Verifica que el nombre la variable exista dentro de la tabla de variables globales y si existe la regresa
    VarTable::symbolRow * getGlobalVariable(string name){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = vt->find(name);
      return vrow;
    }

    // Verifica que la dirección de la variable exista dentro de la tabla de variables globales y si existe la regresa
    VarTable::symbolRow * getGlobalVariable(int dir){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = vt->find(dir);
      return vrow;
    }
    // Regresa un hashmap de la lista de variables dentro de la tabla de variables globales
    unordered_map<int, VarTable::symbolRow*> getGlobalVariableList(){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      unordered_map<int, VarTable::symbolRow*> list = vt->getList();
      return list;
    }

    // Inserta una variable global (nombre y tipo), dentro de la tabla de variables globales si esta no existe ya 
    void insertGlobalVariable(string name, int type){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addGlobalVarDir(type);
      vt->insert(vrow);
    }

    // Inserta una apuntador global (nombre y tipo), dentro de la tabla de variables globales si este no existe ya 
    // Regresa la dirección del apuntador
    int insertGlobalPointer(string name, int type){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addPointerBaseVar(type);
      vt->insert(vrow);
      return vrow->dir;
    }

    // Inserta un arreglo/matriz global (nombre y tipo), dentro de la tabla de variables globales si este no existe ya 
    void insertGlobalVariable(string name, int dim1, int dim2, int type){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addGlobalVarDir(type);
      vrow->dimension1 = dim1;
      vrow->dimension2 = dim2;
      vt->insert(vrow);
      if(dim1 > 0){
        if(dim2 > 0){
          int matrix = dim1*dim2-1;
          for (int i = 0; i < matrix; i++)
          {
            insertGlobalVariable(name + "_" + to_string(i+1),type);
          } 
        }else{
          int array = dim1-1;
          for (int i = 0; i < array; i++)
          {
            insertGlobalVariable(name + "_" + to_string(i+1),type);
          }
        }
      }
    }
    
    // Actualiza variable global (nombre , tipo y dirección) dentro de la tabla de variables globales si este no existe ya 
    void updateGlobalVariable(string name, int type,int dir){
      VarTable * vt = (VarTable *) globalTable.find("global")->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }

      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = dir;
      vt->modify(vrow);
    }

    //Se puede borrar pero tenemos que probarla
    void insertLocalVariable(string name, int type){
      VarTable * vt = (VarTable *) currentTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addLocalVarDir(type);
      vt->insert(vrow);
    }

    ////////////////////////////////////
    //FUNCTION SCOPE - VARIABLES LOCALES
    ////////////////////////////////////
    
    // Establece el scope local de la función, basado en nombre proveído como parámetro, dentro del directorio de funciones y reestablece los contadores locales
    void setFunctionScope(string id){
        FunctionTable::symbolRow * func = globalTable.find(id);
        if(!func){
            throw CompilerError("Error, function doesn't exist.");
        }
        functionScope = func;
        resetLocalConts();
    }

    // Maneja las funciones de nombres repetidos dentro de las distintas clases, regresa la dirección de la variable
    int getScopeDir(){  
      string name = functionScope->name;
      if(classScope){
        name = "_" + classScope->name + name;
      }
      VarTable::symbolRow *var = getGlobalVariable(name);
      if(!var){
        throw CompilerError("Variable "+ name + "doesn't exist.");
      }
      return var->dir;
    }

    // Reestablece el functionScope a nulo
    void freeFunctionScope(){
        functionScope = NULL;
    }

    // Se busca el functionScope en la tabla de variables
    // Si existe el functionScope en la tabla de variables, se busca el nombre de la variable dentro de la tabla de variables
    VarTable::symbolRow * getVarOnScope(string id){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) functionScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = vt->find(id);
      return vrow;
    }

    // Se busca el functionScope en la tabla de variables
    // Si existe el functionScope en la tabla de variables, se busca el número de la variable dentro de la tabla de variables
    VarTable::symbolRow *  getVarOnScope(int id){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) functionScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = vt->find(id);
      return vrow;
    }

    // Verifica el tipo de retorno de función dentro del functionScope
    void verifyTypeOnScope(int type){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      if(type != functionScope->return_type){
        throw CompilerError("Error, return type mismatch");
      }
      
    }
    // Inserta una variable local con su tipo dentro de la tabla local de la función
    int insertVarOnScope(string name, int type){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) functionScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addLocalVarDir(type);

      vt->insert(vrow);
      return vrow->dir;
    }

    // Inserta un arreglo/matriz local con su tipo dentro de la tabla local de la función
    int insertVarOnScope(string name, int dim1, int dim2, int type){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) functionScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addLocalVarDir(type);
      vrow->dimension1 = dim1;
      vrow->dimension2 = dim2;

      vt->insert(vrow);
      if(dim1 > 0){
        if(dim2 > 0){
          int matrix = dim1*dim2-1;
          for (int i = 0; i < matrix; i++)
          {
            insertVarOnScope(name + "_" + to_string(i+1),type);
          } 
        }else{
          int array = dim1-1;
          for (int i = 0; i < array; i++)
          {
            insertVarOnScope(name + "_" + to_string(i+1),type);
          }
        }
      }
      return vrow->dir;
    }

    // Inserta el nombre del parámetro y su tipo dentro del functionScope
    void insertParameterOnScope(string name, int type){
      insertVarOnScope(name,type);
      VarTable::symbolRow * var = getVarOnScope(name);
      (*functionScope).parameters.push_back(*var);
    }

    // Modifica el nombre, tipo y dirección de una variable dentro de la tabla de variables 
    void updateVarOnScope(string name, int type, int dir){
      if(functionScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) functionScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = dir;
      if(vrow == NULL){
        throw CompilerError("Error, variable doesn't exist.");
      }
      vt->modify(vrow);
    }

    //CLASSES

    // Crea una clase dentro de un nuevo directorio de clases (estructura de directorio de funciones)
    int globalInsertClass(string name, int type, int quadDir){
      FunctionTable::symbolRow * frow = new FunctionTable::symbolRow();
      frow->name = name;
      frow->return_type = type;
      frow->varTable = new VarTable();
      frow->classFunctions = new FunctionTable();
      frow->quadDir = quadDir;
      frow->functionId = 100 + classes.size();
      frow->ints = 0;
      frow->floats = 0;
      frow->chars = 0;
      globalTable.insert(frow);

      classes.push_back(frow);

      return 1;
    }

    // Regresa el id(entero) de la clase segun su nombre
    int getClassType(string name){
      FunctionTable::symbolRow * frow = globalTable.find(name);
      if(frow == NULL){
        throw CompilerError("Error, class (" + name + ") doesn't exist.");
      }
      if(frow->return_type != 10){
        throw CompilerError("Error, (" + name + ") is not an Object.");
      }
      return frow->functionId;
    }

    // Regresa el nombre de la clase
    string freeClassScope(){
        string ret = "";
        if(classScope)
            ret = classScope->name;
        classScope = NULL;
        return ret;
    }

    // Busca el nombre de la clase y si la encuentra iguala el classScope a el valor obtenido
    // Reestablece las direcciones de los contadores de las clases
    void setClassScope(string id){
        FunctionTable::symbolRow * func = globalTable.find(id);
        if(!func){
            throw CompilerError("Error, class (" + id + ") doesn't exist.");
        }
        classScope = func;
        resetClassConts();
    }

    // Busca el número de la clase dentro de la lista de clases e iguala classScope al valor obtenido   
    void setClassScopeID(int id){
        FunctionTable::symbolRow * func = classes[id-100];
        if(!func){
            throw CompilerError("Error, class (" + to_string(id) + ") doesn't exist.");
        }
        classScope = func;
    }

    // Busca el nombre de la función dentro de su respectiva clase y si la encuentra iguala el functionScope a el valor obtenido
    // Reestablece las direcciones de los contadores locales
    void setClassFunctionScope(string id){
        FunctionTable::symbolRow * func = classScope->classFunctions->find(id);
        if(!func){
            throw CompilerError("Error, function doesn't exist.");
        }
        functionScope = func;
        resetLocalConts();
    }

    // Busca la variable según su nombre dentro de la tabla de variables dentro del classScope y regresa la variable
    VarTable::symbolRow * getClassVariable(string name){
      if(!classScope){
        throw CompilerError("Error: " + name + " doesn't exists.");
      }
      VarTable * vt = (VarTable *) classScope->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = vt->find(name);
      return vrow;
    }

    // Regresa las variables de una clase
    VarTable::symbolRow * getClassVariable(int dir){
      if(!classScope){
        throw CompilerError("Error: class doesn't exists.");
      }
      VarTable * vt = (VarTable *) classScope->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = vt->find(dir);
      return vrow;
    }

    // Busca el nombre de la variable y el número de la clase dentro de la lista de clases y regresa la variable 
    VarTable::symbolRow * getVariableFromClassDir(int classDir,string var){
      if(classDir-100>=classes.size()){
        throw CompilerError("Error: class doesn't exists.");
      }

      FunctionTable::symbolRow * ft = classes[classDir-100];

      VarTable * vt = (VarTable *) ft->varTable;
      if(!vt){
        throw CompilerError("Error, variable directory doesn't exist.");
      }
      VarTable::symbolRow * vrow = vt->find(var);
      return vrow;
    }

    // Inserta una variable (nombre y tipo) dentro de la tabla de variables de la clase 
    void insertVarOnClassScope(string name, int type){
      if(classScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) classScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addClassVarDir(type);
      vrow->dimension1 = 0;
      vrow->dimension2 = 0;

      vt->insert(vrow);
    }
    
    // Inserta una función  dentro del directorio de funciones de la clase 
    int insertClassFunction(string name, int type, int quadDir){
      FunctionTable::symbolRow * frow = new FunctionTable::symbolRow();
      frow->name = name;
      frow->return_type = type;
      frow->varTable = new VarTable();
      frow->classFunctions = NULL;
      frow->quadDir = quadDir;
      frow->functionId = functions.size();
      frow->ints = 0;
      frow->floats = 0;
      frow->chars = 0;
      classScope->classFunctions->insert(frow);

      functions.push_back(frow);

      return 1;
    }

    // Inserta un arreglo/matriz (nombre y tipo) dentro de la tabla de variables de la clase 
    void insertVarOnClassScope(string name, int dim1, int dim2, int type){
      if(classScope == NULL){
        throw CompilerError("Error, no function on scope.");
      }
      VarTable * vt = (VarTable *) classScope->varTable;
      if(vt == NULL){
        throw CompilerError("Error, no variable directory installed on function.");
      }
      VarTable::symbolRow * vrow = new VarTable::symbolRow();
      vrow->name = name;
      vrow->type = type;
      vrow->dir = addClassVarDir(type);
      vrow->dimension1 = dim1;
      vrow->dimension2 = dim2;

      vt->insert(vrow);
      if(dim1 > 0){
        if(dim2 > 0){
          int matrix = dim1*dim2-1;
          for (int i = 0; i < matrix; i++)
          {
            insertVarOnClassScope(name + "_" + to_string(i+1),type);
          } 
        }else{
          int array = dim1-1;
          for (int i = 0; i < array; i++)
          {
            insertVarOnClassScope(name + "_" + to_string(i+1),type);
          }
        }
      }
    }

    // Regresa una función, según su nombre, dentro del directorio de funciones del directorio de clases
    FunctionTable::symbolRow * getClassFunction(string row){
      FunctionTable::symbolRow * exist = classScope->classFunctions->find(row);
      if(exist == NULL){
        return NULL;
        throw CompilerError("Error, function doesn't exist.");
      }
      return exist;
    }

    // Regresa una función, según su indice de renglón, dentro del directorio de funciones del directorio de clases
    FunctionTable::symbolRow * getClassFunction(int row){
      //FunctionTable::symbolRow * exist = classScope->classFunctions->find(row);
      if(row >= functions.size())
        throw CompilerError("Error, function doesn't exist.");

      FunctionTable::symbolRow * exist = functions[row];
      if(exist == NULL){
        throw CompilerError("Error, function doesn't exist.");
      }
      return exist;
    }

};

// Resuelve los diferentes tipos de expresiones
class ExpressionSolver{
  public:

    // Estructura de nombre, tipo y dirección de una variable, se maneja un flag para constantes
    struct varAndType{
      string var;
      int type;
      int dir;
      int constant;
    };

    stack<struct varAndType> operands;
    stack<int> operators;
    int classInstances = 0;
    
    // Estructura de un cuádruplo, incluye operador, operandos izquierdo y derecho y la variable resultante
    struct quadruple{
        int op;
        string operandLeft;
        string operandRight;
        string result;
    };

    vector<quadruple> result;
    unordered_map<string, int> resmap;
      
  
    // Insertar el id del operando, busca que exista, lo almacena en un varAndType y insertaba en el stack de operandos
    void insertOprndDir(int id){
      VarTable::symbolRow * var = NULL;
      
      if(scope.functionScope == NULL){
        var = scope.getGlobalVariable(id);
      }else{
        var = scope.getVarOnScope(id);
        if(var == NULL){
          var = scope.getGlobalVariable(id);
        }
      }
      if(var==NULL && scope.classScope != NULL){
        var = scope.getClassVariable(id);
      }
      if(var == NULL){
        throw CompilerError("Error, variable not found.");
      }
      varAndType vt = varAndType();
      vt.constant = 0;
      vt.type = var->type;
      vt.var = var->name;
      vt.dir = var->dir;
      operands.push(vt);
    }

    // Insertar el nombre del operando, busca que exista, lo almacena en un varAndType y insertaba en el stack de operandos
    void insertOprnd(string id){
      VarTable::symbolRow * var = NULL;
      
      if(scope.functionScope == NULL){
        var = scope.getGlobalVariable(id);
      }else{
        var = scope.getVarOnScope(id);
        if(var == NULL){
          var = scope.getGlobalVariable(id);
        }
      }
      if(var==NULL && scope.classScope != NULL){
        var = scope.getClassVariable(id);
      }
      if(var == NULL){
        throw CompilerError("Error, variable (" + id + ") not found.");
      }
      varAndType vt = varAndType();
      vt.constant = 0;
      vt.type = var->type;
      vt.var = var->name;
      vt.dir = var->dir;
      operands.push(vt);
    }

    // Saca el operando del stack de operandos, buscandolo en la tabla de variables globales 
    int popOprnd(){
      varAndType vt = operands.top();
      operands.pop();
      return vt.dir;
    }
    
    // Insertar constante de tipo entero en el stack de operandos
    void insertOprnd(int id){
      struct varAndType vt = varAndType();
      int dir = scope.addConstantINTVarDir(id);
      vt.constant = 1;
      vt.type = 0;
      vt.var = to_string(dir);
      vt.dir = dir;
      operands.push(vt);
    }

    // Insertar constante de tipo flotante en el stack de operandos
    void insertOprnd(float id){
      struct varAndType vt = varAndType();
      int dir = scope.addConstantFLOATVarDir(id);
      vt.constant = 1;
      vt.type = 1;
      vt.var = to_string(dir);
      vt.dir = dir;
      operands.push(vt);
    }

    // Insertar constante de tipo carácter en el stack de operandos
    void insertOprnd(char id){
      struct varAndType vt = varAndType();
      int dir = scope.addConstantCHARVarDir(id);
      vt.constant = 1;
      vt.type = 2;
      vt.var = to_string(dir);
      vt.dir = dir;
      operands.push(vt);
    }

    // Insertar operador en el stack de operadores
    void insertOptr(int oper){
      operators.push(oper);
    }
    
    // Crea fondo falso
    void installPar(){
      operators.push(99);
    }
    
    // Termina el fondo falso
    void closePar(){
      int par = operators.top();
      if(par != 99){
        throw CompilerError("Error, incomplete expression.");
      }
      operators.pop();
    }

    // Realiza la asignación del resultado de la expresión y lo guarda dentro del stack de operandos 
    // Saca el operador y los operandos de sus respectivas pilas
    void assign(){

      int assign = operators.top();
      if(assign != 5){
        throw CompilerError("Error, incomplete expression.");
      }
      //operators.pop();
      //operators.push(101);
      checkStacks();
    }

    // Verifica que haya dos operandos y un operador para agruparlos
    int checkStacks(){
        int optr;
        varAndType opndLeft, opndRight;
        if(operands.size() >= 2 && operators.size() >= 1 && operators.top() != 99){
          optr = operators.top();
          operators.pop();
          opndRight = operands.top();
          operands.pop();
          opndLeft = operands.top();
          operands.pop();

          varAndType res = solveQuadruple(optr, opndLeft,opndRight);
          operands.push(res);

          quadruple quad = quadruple();
          quad.op = optr;
          quad.operandLeft = opndLeft.var;
          quad.operandRight = opndRight.var;
          quad.result = res.var;

          if(optr == 5)
              quad.result = opndLeft.var;
          result.push_back(quad);
          return 1;
        }
        return 0;
    }

    // Regresa las instancias de la clase
    int returnClassInstances(){
      return classInstances;
    }

    // Incrementa el número de instancias de la clase
    void insertClassInstance(){
      classInstances++;
    }

    // Resuelve la expresión (linea del cuádruplo)
    varAndType solveQuadruple(int optr, varAndType opndLeft, varAndType opndRight){
        //busca si la operacion es valida (checar cubo) 
        //SemanticConsideration sc = SemanticConsideration();
        int cubeRes= sc.testCube(opndLeft.type, opndRight.type ,optr);
        varAndType vt = varAndType();
        vt.type = cubeRes;
        vt.var = "_t" + to_string(tempCount++);
        if(optr != 5)
            scope.insertVarOnScope(vt.var,vt.type);
        return vt;
         //si es valida hacer operacion, sino mandar error
    }

    // Si la expresión es unicamente un operando devuelve el dirección de la variable
    int singleVar(){
      if(operands.size() == 1 && operators.size() == 0){
        varAndType vt = operands.top();
        operands.pop();
        if(vt.constant){
          return vt.dir;
        }
        VarTable::symbolRow * var = NULL;
        if(scope.functionScope == NULL){
          var = scope.getGlobalVariable(vt.var);
        }else{
          var = scope.getVarOnScope(vt.var);
          if(var == NULL){
            var = scope.getGlobalVariable(vt.var);
          }
        }
        if(var==NULL && scope.classScope != NULL){
          var = scope.getClassVariable(vt.var);
        }

        return var->dir;
      }
      return 0;
    }

    // Verifica si el tipo del parámetro según su dirección,
    // si no la encuentra la busca dentro de las tablas de variables globales, de variables de clases y dentro del varOnScope
    void checkVarType(int dir){
        int paramType;
        if(dir > 20000 && dir < 31001){
            paramType = 0;
            if(dir > 26000)
              paramType = 1;
            if(dir > 29000)
              paramType = 2;
        }else{    
            VarTable::symbolRow * var = NULL;
            if(scope.functionScope == NULL){
              var = scope.getGlobalVariable(dir);
            }else{
              var = scope.getVarOnScope(dir);
              if(var == NULL){
                var = scope.getGlobalVariable(dir);
              }
              if(var==NULL && scope.classScope != NULL){
                var = scope.getClassVariable(dir);
              }
            }
            paramType = var->type;
        }
        scope.verifyTypeOnScope(paramType);
    }

    // Regresa un vector de los cuádruplos
    vector<quadruple> exportQuads(){
      return result;
    }

};

// Clase que maneja y administra la lista de cuádruplos
// La clase almacena una estructura 
class Quad{
  public:

    struct varAndType{
      string var;
      int type;
    };
    
    // Estructura de un cuádruplo, incluye operador, operandos izquierdo y derecho y la variable resultante
    struct quadruple{
        int op;
        int operandLeft;
        int operandRight;
        int result;
    };

    vector<quadruple> quadList;

    // Itera la lista de cuádruplos, hace las verificaciones necesarias y lo mete en el vector de cuádruplos
    void importSolver(ExpressionSolver es){
        vector<ExpressionSolver::quadruple> expList = es.exportQuads();
        for(int i = 0; i<expList.size();i++){
          insertQuad(expList[i]);
        }
    };

    // Verifica si el string es un número
    int isNumber(const string str){
        for (char const c : str) {
            if (isdigit(c) == 0) return 0;
        }

        return 1;
    }

    // Inserta estructura de cuádruplo al vector de cuádruplos
    void insertQuad(ExpressionSolver::quadruple quad){
        int opl, res;
        int opr = -1;
        if(isNumber(quad.operandLeft)){
            opl = stoi(quad.operandLeft);
        }else{
            VarTable::symbolRow * var = scope.getVarOnScope(quad.operandLeft);
            if(var == NULL){
                var = scope.getGlobalVariable(quad.operandLeft);
            }
            if(var == NULL){
                var = scope.getClassVariable(quad.operandLeft);
            }
            opl = var->dir;
        }
        if(isNumber(quad.operandRight)){
            opr = stoi(quad.operandRight);
        }else{
            VarTable::symbolRow * var = scope.getVarOnScope(quad.operandRight);
            if(var == NULL){
                var = scope.getGlobalVariable(quad.operandRight);
            }
            if(var == NULL){
                var = scope.getClassVariable(quad.operandRight);
            }
            opr = var->dir;
        }
        VarTable::symbolRow * resvar = scope.getVarOnScope(quad.result);
        if(resvar == NULL){
            resvar = scope.getGlobalVariable(quad.result);
        }
        if(resvar == NULL){
                resvar = scope.getClassVariable(quad.result);
        }
        res = resvar->dir;
        quadruple convQuad = quadruple();
        convQuad.op = quad.op;
        convQuad.operandLeft = opl;
        convQuad.operandRight = opr;
        convQuad.result = res;
        if(convQuad.op == 5){
          convQuad.operandLeft = opr;
          convQuad.operandRight = -1;
        }
        quadList.push_back(convQuad);
    };

    // Inserta cuádruplo al vector de cuádruplos con su operador, sus operandos y variable resultante
    void insertQuad(int op, int operandLeft, int operandRight, int result){
      quadruple quad = quadruple();
      quad.op = op;
      quad.operandLeft = operandLeft;
      quad.operandRight = operandRight;
      quad.result = result;
      quadList.push_back(quad);
    };

    // Saca el ultimo cuádruplo del vector de cuádruplos
    void deleteLastQuad(){
        quadList.pop_back();
    }

    // Actualiza un cuádruplo según su indice dentro del vector de cuádruplos, usando una estructura de cuádruplo
    void updateQuad(int dir,quadruple newQuad){
        if(dir < 0 || dir >= quadList.size()){
          throw CompilerError("Inexistent Quad");
        }
        quadList[dir] = newQuad;
    }
     
    // Actualiza la variable resultante de un cuádruplo según su indice dentro del vector de cuádruplos
    void updateQuadResult(int dir,int result){
        if(dir < 0 || dir >= quadList.size()){
          throw CompilerError("Inexistent Quad");
        }
        quadList[dir].result = result;
    }
    
    // Actualiza un cuádruplo según su indice dentro del vector de cuádruplos, usando un operador, dos operandos y una variable resultante
    void updateQuad(int dir,int op, int opleft, int opright, int result){
        if(dir < 0 || dir >= quadList.size()){
          throw CompilerError("Inexistent Quad");
        }
        
        quadList[dir].op = op;
        quadList[dir].operandLeft = opleft;
        quadList[dir].operandRight = opright;
        quadList[dir].result = result;
    }

    // Regresa el último cuádruplo del vector de cuádruplos
    quadruple getLastQuad(){
      return quadList[quadList.size()-1];
    }

    // Regresa la dirección del último cuádruplo del vector de cuádruplos
    int getQuadLastDir(){
      return quadList.size();
    }

    // Inserta un GOTO
    void prepareMainQuad(){
        insertQuad(200,-1,-1,-1);
    }

    // Actualiza linea de salto dentro del GOTO  
    void setMainQuad(){
        updateQuad(0,200,-1,-1,getQuadLastDir());
    }

    // Imprime la lista de cuádruplos
    void printQuadList(){
        cout << "Quads List: \n" << endl;
        for(int i = 0; i<quadList.size();i++){
            cout << i << "| "<< quadList[i].op << " || " << quadList[i].operandLeft << " || " << quadList[i].operandRight << " || " << quadList[i].result << endl;
        };
    };
};


// Clase que resuelve arreglos y funciones verificando las inserciones de variables sobre el
// (Funciones) Se verifican los parametros y la cantidad de los mismos para validar la llamada a la función
// (Arreglos) Se verifican los parametros de indexado del arreglo o matriz y que sean de tipado entero
class ParamSolver{
  public:
    FunctionTable::symbolRow * table;
    VarTable::symbolRow * array;
    vector<VarTable::symbolRow> params;
    int paramLength = 0;
    int paramCount = 0;
    int quadGlobalDir = -1;
    int onClass = 0;

    // Busca el nombre de la función, valida si está dentro de una clase
    void searchFunction(string func, int isClass){
      if(isClass){
        table = scope.getClassFunction(func);
        if(scope.classScope && table->return_type != 20){
            quadGlobalDir = scope.getGlobalVariable("_" + scope.classScope->name + table->name)->dir;
        }
      }
      else{
        table = scope.globalGetFunction(func);
        if(table && table->return_type != 20)
            quadGlobalDir = scope.getGlobalVariable(table->name)->dir;
      }
        params = table->parameters;
        paramLength = params.size();
    }

    // Busca la dirección de un arreglo/matriz, valida si está dentro de una clase
    void searchArray(int dir, int isClass){

        if(isClass){
          array = scope.getClassVariable(dir);
        }else{
          array = scope.getVarOnScope(dir);
          if(!array)
              array = scope.getGlobalVariable(dir);
        }

        int parameters = 0;
        if(array->dimension1 > 0){
          parameters++;
        }else{
          throw CompilerError("Error: " + array->name + " is not an Array.");
        }
        if(array->dimension2 > 0){
          parameters++;
        }
        paramLength = parameters;
    }

    // Busca el nombre de una función y crea un parámetro 
    ParamSolver(string function, int isClass){
      onClass = isClass;
      searchFunction(function,isClass);
    }
    
    // Busca la dirección de un arreglo/matriz y crea un parámetro 
    ParamSolver(int dir, int isClass){
      onClass = isClass;
      searchArray(dir, isClass);
    }

    // Regresa el id de la función dentro de la tabla de cuádruplos
    int getFunctionQuadDir(){
      return table->functionId;
    }

    // Regresa la dirección global del cuádruplo
    int getFunctionGlobalDir(){
      return quadGlobalDir;
    }

    // Regresa la variable resultante temporal del cuádruplo y su tipo 
    int generateReturnQuad(){
      Quad::quadruple quad = Quad::quadruple();
      string temp = "_t" + to_string(tempCount++);      
      return scope.insertVarOnScope(temp,table->return_type);
    }

    // Genera y guarda la variable resultante temporal del cuádruplo y el tipo del arreglo
    // Regresa la dirección de la variable resultante temporal del arreglo
    int generateArrayQuad(){
      Quad::quadruple quad = Quad::quadruple();
      string temp = "_t" + to_string(tempCount++);      
      scope.insertVarOnScope(temp,array->type);
      return scope.getVarOnScope(temp)->dir;
    }

    // Regresa la variable resultante temporal del cuádruplo y el tipo de pointer
    int generatePointerQuad(){
      Quad::quadruple quad = Quad::quadruple();
      string temp = "_t" + to_string(tempCount++);      
      return scope.insertGlobalPointer(temp,array->type);
    }

    // Regresa el tipo de variable según la dirección de ésta
    int searchVar(int dir){
      VarTable::symbolRow * var = scope.getVarOnScope(dir);
      if(var == NULL){
          var = scope.getGlobalVariable(dir);
      }
      if(var == NULL){
          var = scope.getClassVariable(dir);
      }
      return var->type;
    }

    // Regresa la dirección del arreglo/matriz
    int getArrayDir(){
      return array->dir;
    }

    // Regresa el tipo del arreglo/matriz
    int getArrayType(){
      return array->type;
    }

    // Regresa las dimensiones del arreglo/matriz
    int getDimension(int pos){
      if(pos == 1){
        return array->dimension1;
      }
      if(pos == 2){
        return array->dimension2;
      }
      return 0;
    }

    // Inserta parámetros dentro de un arreglo/matriz
    int insertParamArray(int dir){
      if(paramCount >= paramLength){
        throw CompilerError("Error: parameters insertion exceeded");
      }
      int paramType;
      if(dir > 20000 && dir < 50000){
          paramType = 0;
          if(dir > 26000)
            paramType = 1;
          if(dir > 29000)
            paramType = 2;
      }else{
          paramType = searchVar(dir);
      }
      if(paramType != 0){
        throw CompilerError("Error: Parameter must be an integer");
      }
      return paramCount++ + 1;
    }

    // Inserta parámetros dentro de un función
    int insertParam(int dir){
      if(paramCount >= paramLength){
        throw CompilerError("Error: parameters insertion exceeded");
      }
      int paramType;
      if(dir > 20000 && dir < 50000){
          paramType = 0;
          if(dir > 26000)
            paramType = 1;
          if(dir > 29000)
            paramType = 2;
      }else{
          paramType = searchVar(dir);
      }
      int funcParamType = params[paramCount].type;
      if(paramType != funcParamType){
          throw CompilerError("Error: type mismatch on parameter " + to_string(paramCount));
      }
      return paramCount++ + 1;
    }

    // Verifica el número de los parámetros 
    void checkParams(){
      if(paramCount != paramLength){
          throw CompilerError("Error: " + to_string(paramLength-paramCount) + " left");
      }
    }

    // Regresa la bandera onClass
    int isClass(){
      return onClass;
    }
};
