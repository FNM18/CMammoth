#include "driver.h"
#include "semantic.cpp"
#include <stack>
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

class VMachine;

// Vectores para funciones y clases 
static vector<FunctionTable::symbolRow *> funcs;
static vector<FunctionTable::symbolRow *> classes;

// Clase objeto el cual al ser instanciado  guarda las variables de un objeto creado
// Este almacena las variables de del tipo de clase construida en compilación
class Object{
    public:
        int classId = -1;
        int * classInts;
        float * classFloats;
        char * classChars;
        int quadDir = -1;

        void constructObject(int quadDr,int id, int intsV, int floatsV, int charsV){
            classId = id;
            quadDir = quadDr;
            classInts = new int[intsV];
            classFloats = new float[floatsV];
            classChars = new char[charsV];
        }

        // Regresa el valor del entero según la dirección dentro del arreglo de enteros de la clase
        int getIntVar(int dir){
            return classInts[dir - 31001];
        }
        
        // Regresa el valor del flotante según la dirección dentro del arreglo de flotantes de la clase
        float getFloatVar(int dir){
            return classFloats[dir - 36001];
        }

        // Regresa el valor del caracter según la dirección dentro del arreglo de caracteres de la clase
        char getCharVar(int dir){
            return classChars[dir - 39001];
        }

        // Iguala el valor de una variable dentro del arreglo de enteros de la clase según la dirección y un valor de entrada
        void setIntVar(int dir, int value){
            classInts[dir - 31001] = value;
        }
        
        // Iguala el valor de una variable dentro del arreglo de flotantes de la clase según la dirección y un valor de entrada 
        void setFloatVar(int dir, float value){
            classFloats[dir - 36001] = value;
        }

        // Iguala el valor de una variable dentro del arreglo de caracteres de la clase según la dirección y un valor de entrada 
        void setCharVar(int dir, char value){
            classChars[dir - 39001] = value;
        }
};

// Clase función el cual al ser instanciado  crea variables temporales
// Las variables temporales son eliminadas al finalizar o retornar la función descrita en compilación
// Este almacena las variables de del tipo de clase construida en compilación
class Function{
    public:
        int functionId = -1;
        int * localInts;
        float * localFloats;
        char * localChars;
        int params = 0;
        int quadDir = -1;
        
        Function(int quadDr,int id, int paramCount, int intsV, int floatsV, int charsV){
            functionId = id;
            quadDir = quadDr;
            params = paramCount;
            localInts = new int[intsV];
            localFloats = new float[floatsV];
            localChars = new char[charsV];
        }

        // Libera la memoria de las variables locales
        void deleteMemory(){
            delete localInts;
            delete localFloats;
            delete localChars;
        }

        // Regresa el valor del entero según la dirección dentro del arreglo de enteros locales
        int getIntVar(int dir){
            return localInts[dir - 10001];
        }

        // Regresa el valor del flotante según la dirección dentro del arreglo de flotantes locales
        float getFloatVar(int dir){
            return localFloats[dir - 16001];
        }

        // Regresa el valor del caracter según la dirección dentro del arreglo de caracteres locales
        char getCharVar(int dir){
            return localChars[dir - 19001];
        }

        // Iguala el valor de una variable dentro del arreglo de enteros locales según la dirección y un valor de entrada
        void setIntVar(int dir, int value){
            localInts[dir - 10001] = value;
        }

        // Iguala el valor de una variable dentro del arreglo de flotantes locales según la dirección y un valor de entrada
        void setFloatVar(int dir, float value){
            localFloats[dir - 16001] = value;
        }

        // Iguala el valor de una variable dentro del arreglo de caracteres locales según la dirección y un valor de entrada
        void setCharVar(int dir, char value){
            localChars[dir - 19001] = value;
        }

        // Inserta la dirección y el valor de un parámetro de tipo entero 
        void insertParam(int dir, int value){
            setIntVar(dir,value);
        }

        // Inserta la dirección y el valor de un parámetro de tipo flotante 
        void insertParam(int dir, float value){
            setFloatVar(dir,value);
        }
        // Inserta la dirección y el valor de un parámetro de tipo carácter
        void insertParam(int dir, char value){
            setCharVar(dir,value);
        }
};

// Clase Virtual Machine encargada de ejecutar la lista de cuádruplos
// Esta utiliza  variables globales, temporales, constantes, funciones y clases generadas en compilación
// Clase interactúa con instancias de funciones y clases manejandolas en un sistema de posicionamiento por stacks       
class VMachine{
    public:

        int quadSize = 0; // Tamaño de lista de cuádruplos
        int global = 1; // Flag de acceso de memoria global o local

        vector<Quad::quadruple> quadList; //Lista de cuádruplos
        
        //Variables globales
        unordered_map<int,int> globalInts;
        unordered_map<int,float> globalFloats;
        unordered_map<int,char> globalChars;
        unordered_map<int,Object> globalObjects;
        unordered_map<int,int> pointers;

        //Variables constantes
        unordered_map<int,int> constantInts;
        unordered_map<int,float> constantFloats;
        unordered_map<int,char> constantChars;
        unordered_map<int,string> constantStrings;

        //Stacks manejadores de funciones y clases
        stack<Function> functions; //Funciones que generan acceso a memoria de funciones
        stack<Function> tempfunctions; //Funciones pregeneradas mientras se insertan parámetros
        stack<Object> classdirs; // Objetos que generan acceso a memoria de clases

        //Busca y asigna variables, cuadruplos, clases y funcione
        //Asigna localmente lo buscado para asi generar un entorno de ejecución
        VMachine(){
            quadList = quads.quadList;
            quadSize = quadList.size();
            classes = scope.classes;
            importConstants();
            importGlobalVars();
            funcs = scope.functions;
        }

        // Iteran los hashmaps de las constantes de los diferentes tipos de datos (int, floar, char, string)
        // Se guardan los valores dentro de los arreglos respectivos 
        void importConstants(){
            
            unordered_map<int, int>::iterator it = scope.cte_ints.begin();
            while(it != scope.cte_ints.end())
            {
                constantInts[it->second] = it->first;
                it++;
            }

            unordered_map<float, int>::iterator it2 = scope.cte_floats.begin();
            while(it2 != scope.cte_floats.end())
            {
                constantFloats[it2->second] = it2->first;
                it2++;
            }

            unordered_map<char, int>::iterator it3 = scope.cte_chars.begin();
            while(it3 != scope.cte_chars.end())
            {
                constantChars[it3->second] = it3->first;
                it3++;
            }

            unordered_map<string, int>::iterator it4 = scope.cte_strings.begin();
            while(it4 != scope.cte_strings.end())
            {
                constantStrings[it4->second] = it4->first;
                it4++;
            }
        }

        // Se obtiene la lista de variables globales, se itera y se clasifica según su tipo 
        // Se asignan a los Hashmaps de ints, floats, chars y objetos globales 
        void importGlobalVars(){
            unordered_map<int,VarTable::symbolRow*> list = scope.getGlobalVariableList();

            unordered_map<int,VarTable::symbolRow*>::iterator it = list.begin();
            
            while(it != list.end())
            {
                if(it->first>=50000){
                    pointers[it->first] = 0;
                }else{
                    if(it->second->type == 0){
                        globalInts[it->first] = 0;
                    }else if(it->second->type == 1){
                        globalFloats[it->first] = 0.0;
                    }else if(it->second->type == 2){
                        globalChars[it->first] = '\0';
                    }else if(it->second->type >= 100){
                        auto clase = classes[it->second->type - 100];
                        globalObjects[it->first] = Object();
                        globalObjects[it->first].constructObject(clase->quadDir,clase->functionId,clase->ints,clase->floats,clase->chars);
                    }
                }
                it++;
            }
        }

        // Regresa el tipo y scope según la dirección
        int getType(int dir){
            if(dir<6001){
                return 0; // Global int 
            }else if(dir<9001){
                return 1; // Global float
            }else if(dir<10001){
                return 2; // Global char
            }else if(dir<16001){
                return 3; // Local int
            }else if(dir<19001){
                return 4; // Local float
            }else if(dir<20001){
                return 5; // Local char
            }else if(dir<26001){
                return 6; // Constant int
            }else if(dir<29001){
                return 7; // Constant float
            }else if(dir<30001){
                return 8; // Constant char
            }else if(dir<31001){
                return 9; // Constant strings
            }else if(dir<36001){
                return 0; // Class int
            }else if(dir<39001){
                return 1; // Class float
            }else if(dir<40001){
                return 2; // Class char
            }else if(dir>=50000){
                return 10;
            }
            return -1;
        }

        // Regresa un apuntador según la dirección
        int getPointer(int dir){
            return pointers[dir];
        }

        // Regresa el valor del entero del arreglo de enteros globales según la dirección
        int getIntVar(int dir){
            if(dir>31000 && dir<40001)
                return classdirs.top().getIntVar(dir);

            if(!global && dir>10000 && dir<20001)
                return functions.top().getIntVar(dir);

            if(dir>20000 && dir < 50001)
                return constantInts[dir];
            return globalInts[dir];
        }

        // Regresa el valor flotante del arreglo de flotantes globales según la dirección
        float getFloatVar(int dir){
            if(dir>31000 && dir<40001)
                return classdirs.top().getFloatVar(dir);

            if(!global && dir>10000 && dir<20001)
                return functions.top().getFloatVar(dir);

            if(dir>20000 && dir < 50001)
                return constantFloats[dir];
            return globalFloats[dir];
        }

        // Regresa el valor caracter del arreglo de caracteres globales según la dirección
        char getCharVar(int dir){
            if(dir>31000 && dir<40001)
                return classdirs.top().getCharVar(dir);

            if(!global && dir>10000 && dir<20001)
                return functions.top().getCharVar(dir);

            if(dir>20000 && dir < 50001)
                return constantChars[dir];
            return globalChars[dir];
        }

        // Regresa el valor string del arreglo de strings globales según la dirección
        string getStringVar(int dir){
            if(dir>30000)
                return constantStrings[dir];
            return constantStrings[dir];
        }

        // Iguala un apuntador dentro del arreglo de apuntadores al valor ingresado, basandose también en la dirección
        void setPointer(int dir, int value){
            pointers[dir] = value;
        }

        // Iguala un entero dentro del arreglo de enteros al valor ingresado, basandose también en la dirección
        void setIntVar(int dir, int value){
            if(dir>=50000)
                setPointer(dir,value);
            if(dir>31000 && dir<40001)
                classdirs.top().setIntVar(dir,value);

            if(!global && dir>10000 && dir<20001)
                functions.top().setIntVar(dir,value);
            else
                globalInts[dir] = value;
        }

        // Iguala un flotante dentro del arreglo de flotantes al valor ingresado, basandose también en la dirección
        void setFloatVar(int dir, float value){
            if(dir>31000 && dir<40001)
                classdirs.top().setFloatVar(dir,value);
            if(!global && dir>10000 && dir<20001)
                functions.top().setFloatVar(dir,value);
            else
                globalFloats[dir] = value;
        }

        // Iguala un caracter dentro del arreglo de caracteres al valor ingresado, basandose también en la dirección
        void setCharVar(int dir, char value){
            if(dir>31000 && dir<40001)
                classdirs.top().setCharVar(dir,value);
            if(!global && dir>10000 && dir<20001)
                functions.top().setCharVar(dir,value);
            else
                globalChars[dir] = value;
        }

        // Regresa el resultado entero de una operación aritmética segun los operandos y el operador 
        int arithmeticIntOperator(int a, int b, int op){
            switch(op){
                case 0:
                    return a + b;
                case 1:
                    return a - b;
                case 2:
                    return a * b;
                case 3:
                    return a / b;
                case 4:
                    return a % b;
                default:
                    throw runtime_error("Arithmetic Operator Error.");
            } 
        }

        // Regresa el resultado flotante de una operación aritmética segun los operandos y el operador 
        float arithmeticFloatOperator(float a, float b, int op){
            switch(op){
                case 0:
                    return a + b;
                case 1:
                    return a - b;
                case 2:
                    return a * b;
                case 3:
                    return a / b;
                default:
                    throw runtime_error("Arithmetic Operator Error.");
            } 
        }
         
        // Verifica y resuelve las expresiones aritméticas de los cuádruplos
        // Mapea variable en caso de tener algun operando de apuntador 
        void arithmeticFunction(const Quad::quadruple quad){
            int typeRes = getType(quad.result);
            int typeLeft = getType(quad.operandLeft);
            int typeRight = getType(quad.operandRight);
            int assigniLeft;
            int assigniRight;
            float assignfLeft;
            float assignfRight;
            Quad::quadruple quadMod = quad;
            if(typeLeft == 10){
                quadMod.operandLeft = getPointer(quad.operandLeft);
                typeLeft = getType(quadMod.operandLeft);
            }
            if(typeRight == 10){
                quadMod.operandRight = getPointer(quad.operandRight);
                typeRight = getType(quadMod.operandRight);
            }
            if(typeRes == 10000){
                quadMod.result = getPointer(quad.result);
                typeRes = getType(quadMod.result);
            }
            if(typeLeft == 0 || typeLeft == 6 || typeLeft == 3){
                assigniLeft = getIntVar(quadMod.operandLeft);
                assignfLeft = assigniLeft;
            } else if(typeLeft == 1 || typeLeft == 7 || typeLeft == 4){
                assignfLeft = getFloatVar(quadMod.operandLeft);
            }
            if(typeRight == 0 || typeRight == 6 || typeRight == 3){
                assigniRight = getIntVar(quadMod.operandRight);
                assignfRight = assigniRight;
            } else if(typeRight == 1 || typeRight == 7 || typeRight == 4){
                assignfRight = getFloatVar(quadMod.operandRight);
            } 

            if(typeRes==3 || typeRes==10){
                setIntVar(quad.result,arithmeticIntOperator(assigniLeft,assigniRight,quad.op));
            } else if(typeRes==4){
                setFloatVar(quad.result,arithmeticFloatOperator(assignfLeft,assignfRight,quad.op));
            }
        }
        
        // Verifica y resuelve las expresiones lógicas de los cuádruplos
        void compareFunction(const Quad::quadruple quad){
            int typeRes = getType(quad.result);
            int typeLeft = getType(quad.operandLeft);
            int typeRight = getType(quad.operandRight);
            int assigniLeft;
            int assigniRight;
            float assignfLeft;
            float assignfRight;
            Quad::quadruple quadMod = quad;
            if(typeLeft == 10){
                quadMod.operandLeft = getPointer(quad.operandLeft);
                typeLeft = getType(quadMod.operandLeft);
            }
            if(typeRight == 10){
                quadMod.operandRight = getPointer(quad.operandRight);
                typeRight = getType(quadMod.operandRight);
            }
            if(typeRes == 10){
                quadMod.result = getPointer(quad.result);
                typeRes = getType(quadMod.result);
            }

            if(typeLeft == 0 || typeLeft == 6 || typeLeft == 3){
                assigniLeft = getIntVar(quadMod.operandLeft);
                assignfLeft = assigniLeft;
            } else if(typeLeft == 1 || typeLeft == 7 || typeLeft == 4){
                assignfLeft = getFloatVar(quadMod.operandLeft);
            }
            if(typeRight == 0 || typeRight == 6 || typeRight == 3){
                assigniRight = getIntVar(quadMod.operandRight);
                assignfRight = assigniRight;
            } else if(typeRight == 1 || typeRight == 7 || typeRight == 4){
                assignfRight = getFloatVar(quadMod.operandRight);
            }else if(typeRight == 2 || typeRight == 8 || typeRight == 5){
                assignfRight = (int)getCharVar(quadMod.operandRight);
            }
            switch(quadMod.op){
                case 6:
                    setIntVar(quadMod.result,assignfLeft == assignfRight);
                break;
                case 7:
                    setIntVar(quadMod.result,assignfLeft != assignfRight);
                break;
                case 8:
                    setIntVar(quadMod.result,assignfLeft > assignfRight);
                break;
                case 9:
                    setIntVar(quadMod.result,assignfLeft < assignfRight);
                break;
                case 10:
                    setIntVar(quadMod.result,assignfLeft >= assignfRight);
                break;
                case 11:
                    setIntVar(quadMod.result,assignfLeft <= assignfRight);
                break;
                case 12:
                    setIntVar(quadMod.result,assigniLeft && assigniRight);
                break;
                case 13:
                    setIntVar(quadMod.result,assigniLeft || assigniRight);
                break;
                default:
                break;
            } 
        }
        
        // Asigna el valor de la variable resultante
        void assignFunction(const Quad::quadruple quad){
            int typeRes = getType(quad.result);
            int typeLeft = getType(quad.operandLeft);
            int assigni;
            float assignf;
            char assignc;
            Quad::quadruple quadMod = quad;
            if(typeLeft == 10){
                quadMod.operandLeft = getPointer(quad.operandLeft);
                typeLeft = getType(quadMod.operandLeft);
            }
            if(typeRes == 10){
                quadMod.result = getPointer(quad.result);
                typeRes = getType(quadMod.result);
            }
            if(typeLeft == 0 || typeLeft == 6 || typeLeft == 3){
                assigni = getIntVar(quadMod.operandLeft);
            } else if(typeLeft == 1 || typeLeft == 7 || typeLeft == 4){
                assignf = getFloatVar(quadMod.operandLeft);
            } else if(typeLeft == 2 || typeLeft == 8 || typeLeft == 5){
                assignc = getCharVar(quadMod.operandLeft);
            }

            if(typeRes==0 || typeRes==3 || typeRes==6){
                setIntVar(quadMod.result,assigni);
            } else if(typeRes==1 || typeRes==4 || typeRes==7){
                setFloatVar(quadMod.result,assignf);
            } else if(typeRes==2 || typeRes==5 || typeRes==8){
                setCharVar(quadMod.result,assignc);
            }
        }
        
        // Asigna un valor de entrada a una variable
        void readFunction(const Quad::quadruple quad){
            int typeRes = getType(quad.result);
            Quad::quadruple quadMod = quad;
            if(typeRes == 10){
                quadMod.result = getPointer(quad.result);
                typeRes = getType(quadMod.result);
            }
            if(typeRes == 0){
                int read;
                cin >> read;
                setIntVar(quadMod.result,read);
            }else if(typeRes == 1){
                float read;
                cin >> read;
                setFloatVar(quadMod.result,read);
            }else if(typeRes == 2){
                char read;
                cin >> read;
                setCharVar(quadMod.result,read);
            }
        }

        // Imprime una variables o string
        void writeFunction(const Quad::quadruple quad){
            int typeLeft = getType(quad.operandLeft);
            Quad::quadruple quadMod = quad;
            if(typeLeft == 10){
                quadMod.operandLeft = getPointer(quad.operandLeft);
                typeLeft = getType(quadMod.operandLeft);
            }
            if(typeLeft == 0 || typeLeft == 6 || typeLeft == 3){
                cout << getIntVar(quadMod.operandLeft)<< " ";
            } else if(typeLeft == 1 || typeLeft == 7 || typeLeft == 4){
                cout << getFloatVar(quadMod.operandLeft)<< " ";
            } else if(typeLeft == 2 || typeLeft == 8 || typeLeft == 5){
                cout << getCharVar(quadMod.operandLeft)<< " ";
            }else if(typeLeft == 9){
                string text =  getStringVar(quadMod.operandLeft);
                cout << (text == "\n" ? text: text + " ");
            }
        }

        // Realiza un GOTO
        int gotoFunction(const Quad::quadruple quad){
            return quad.result;
        }

        // Realiza un GOTO en falso
        int gotoFalseFunction(const Quad::quadruple quad, int currentQuad){
            return getIntVar(quad.operandLeft) ? currentQuad : quad.result;
        }

        // Asigna el tamaño de memoria local necesitada durante la ejecución
        void era(const Quad::quadruple quad){
            int id = quad.operandLeft;
            Function f = Function(funcs[id]->quadDir,
                    id,funcs[id]->parameters.size(),
                    funcs[id]->ints,
                    funcs[id]->floats,
                    funcs[id]->chars);
            tempfunctions.push(f);
        }

        // Inserta los valores y direcciónes de los parametros, separandolos por tipos
        void param(const Quad::quadruple quad){
            Function f = tempfunctions.top();
            int dir = funcs[f.functionId]->parameters[quad.result-1].dir;
            int type = funcs[f.functionId]->parameters[quad.result-1].type;
            
            int operand = quad.operandLeft;
            if(getType(operand) == 10){
                operand = getPointer(quad.operandLeft);
            }
            if(type == 0){
                int val = getIntVar(operand);     
                f.insertParam(dir,val);
            }else if(type == 1){
                float val = getFloatVar(operand);    
                f.insertParam(dir,val);
            }else if(type == 2){
                char val = getCharVar(operand);    
                f.insertParam(dir,val);
            }
        }

        // Realiza un GOTO a la linea previa y asigna la función al stack de acceso de memoria de clases
        // Libera la memoria de la tabla de variables locales al finalizar función
        void goSub(const Quad::quadruple quad){
            functions.push(tempfunctions.top());
            tempfunctions.pop();
            global = 0;
            executeFunctions(functions.top().quadDir);
            functions.top().deleteMemory();
            functions.pop();
            if(functions.size() < 1){
                global = 1;
            }
        }
        
        // Verifica las dimensiones de los arreglos separandolas según el tipo 
        void ver(const Quad::quadruple quad){
            int typeLeft = getType(quad.operandLeft);
            int assigni;
            float assignf;
            char assignc;
            Quad::quadruple quadMod = quad;
            if(typeLeft == 10){
                quadMod.operandLeft = getPointer(quad.operandLeft);
                typeLeft = getType(quadMod.operandLeft);
            }
            if(typeLeft == 0 || typeLeft == 6 || typeLeft == 3){
                assigni = getIntVar(quadMod.operandLeft);
                if(assigni >= quadMod.result)
                    throw runtime_error("Error: Index out of bounds.");
            } else if(typeLeft == 1 || typeLeft == 7 || typeLeft == 4){
                assignf = getFloatVar(quadMod.operandLeft);
                if(assignf >= quadMod.result)
                    throw runtime_error("Error: Index out of bounds.");
            } else if(typeLeft == 2 || typeLeft == 8 || typeLeft == 5){
                assignc = getCharVar(quadMod.operandLeft);
                if(assignc >= quadMod.result)
                    throw runtime_error("Error: Index out of bounds.");
            }
        }
        // Asigna clase al stack de acceso de memoria de clases
        void openClass(const Quad::quadruple quad){
            classdirs.push(globalObjects[quad.operandRight]);
        }
        
        // Libera clase del stack de acceso de memoria de clases
        void closeClass(const Quad::quadruple quad){
            if(classdirs.size()>0)
                classdirs.pop();
            else
                throw runtime_error("Se rompe");
        }

        // Crea una variable resultante según el cuádruplo
        void returnState(const Quad::quadruple quad){
            assignFunction(quad);
        }

        // Ejecuta y procesa los cuádruplos 
        int executeFunctions(int startCount){
            int quadCount = startCount;
            while (quadCount < quadSize)
            {
                switch (quadList[quadCount].op)
                {
                case 0 ... 4:
                    arithmeticFunction(quadList[quadCount]);
                    break;
                case 5:
                    assignFunction(quadList[quadCount]);
                    break;
                case 6 ... 13:
                    compareFunction(quadList[quadCount]);
                    break;
                case 102:
                    writeFunction(quadList[quadCount]);
                    break;
                case 103:
                    readFunction(quadList[quadCount]);
                    break;
                case 200:
                    quadCount = gotoFunction(quadList[quadCount])-1;
                    break;
                case 201:
                    quadCount = gotoFalseFunction(quadList[quadCount],quadCount+1)-1;
                    break;
                case 202:
                    era(quadList[quadCount]);
                    break;
                case 203:
                    param(quadList[quadCount]);
                    break;
                case 204:
                    goSub(quadList[quadCount]);
                    break;
                case 205:
                    ver(quadList[quadCount]);
                    break;
                case 206:
                    openClass(quadList[quadCount]);
                    break;
                case 207:
                    closeClass(quadList[quadCount]);
                    break;
                case 298:
                    returnState(quadList[quadCount]);
                    return 0;
                    break;
                case 299:
                    return 0;
                    break;
                case 999:
                    return 0;
                    break;
                default:
                    break;
                }
                quadCount++;
            }
            return 0;
        }
};