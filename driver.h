#include <string>
#include "parser.tab.hh"
#include <unordered_map>
#include <stdexcept>

using namespace std;
using namespace yy;
#ifndef DRIVER
#define DRIVER
#define YY_DECL \
parser::symbol_type yylex (parser_driver& driver)
YY_DECL;

static int DEBUG;

// Guarda y extrae el tipo de dato 
class SymbolTable{
  public:
    string getType();
    void setType(string typ){
      type = typ;
    };
  protected:
    string type;
};

// Regresa un mensaje según el error de compilación
class CompilerError: public std::exception{
  public:
    explicit CompilerError(const std::string& message) : msg_(message) {};

  virtual const char* what() const noexcept {
       return msg_.c_str();
    };
    
  protected:
    std::string msg_;
};

//clase para verificar el tipo de salida dependiendo de las combinaciones de los operadores
//se utilizan enumeradores y una matriz para realizar las combinaciones , al igual se incluyen funciones y switches  para verificar su funcionamiento 
class SemanticConsideration{
  public:
   
   // Enumerador de retornos segun el tipo resultante, incluye error
    enum varType{
      Int , 
      Float, 
      Char,
      Err
    };

    // Enumerador de tipo de operaciones
    enum opType{
      Sum, //0
      Sub,
      Mul, //2
      Div,
      Mod, //4
      Assign,
      Eq, //6
      Ne,
      Gt, //8
      Lt,
      Ge, //10
      Le,
      And, //12
      Or
    };

    // Matriz de que guarda los tipos resultantes de las combinaciones de operador con los tipos de datos
    int cube[3][3][14] = {
        {
          {Int,Int,Int,Int,Int,Int,Int,Int,Int,Int,Int,Int,Int,Int},
          {Float,Float,Float,Float,Float,Err,Int,Int,Int,Int,Int,Int,Int,Int},
          {Err,Err,Err,Err,Err,Err,Int,Int,Int,Int,Int,Int,Int,Int}
        },
        {
          {Float,Float,Float,Float,Err,Float,Int,Int,Int,Int,Int,Int,Int,Int},
          {Float,Float,Float,Float,Err,Float,Int,Int,Int,Int,Int,Int,Int,Int},
          {Err,Err,Err,Err,Err,Err,Int,Int,Int,Int,Int,Int,Int,Int}
        },
        {
          {Err,Err,Err,Err,Err,Err,Int,Int,Int,Int,Int,Int,Int,Int},
          {Err,Err,Err,Err,Err,Err,Int,Int,Int,Int,Int,Int,Int,Int},
          {Err,Err,Err,Err,Err,Char,Int,Int,Int,Int,Int,Int,Int,Int}
        }
      }; 

      // Regresa el tipo de dato segun la combinación de los tipo de operandos y el operador
      int testCube(int lv, int rv, int op){
        if(lv == 3 || rv == 3){
          throw CompilerError("Error, operands and operator mismatch.");
        }
        
        int cubeRes = cube[lv][rv][op];
        
        if(cubeRes == 3){
          throw CompilerError("Error, operands and operator mismatch.");
        }

         return cubeRes;
      }
};

// Ejecuta el parser
class parser_driver{
  public:
    void scannerInit();
    void scannerEnd();
    int parse(const string &archivo);
    string file;
};
#endif