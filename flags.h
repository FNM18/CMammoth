#include <string>
#include <vector>
#include <stack>
using namespace std;

//Flags con sus respectivos getters y setters.
int globalFlag = 0; // Indica estatus global
int classFlag = 0; // Indica estatus dentro de clase
int onClass = 0; // Indica si variable anterior fue una clase
int expressionFlag = 0; // Indica si se encuentra resolviendo una expresion
int paramsFlag = 0; // Indica si se encuentra resolviendo una expresion dentro de un parametro
int currentType = -1; //Tipo de dato para asignar a variables
int classAssign = 0; //Flag que indica si se debe cerrar una clase por ser asignada

//Estrcutra de variable con sus respectivas dimensiones
struct varDims{
    string id;
    int dim1 = 0;
    int dim2 = 0;
};

//Guarda nombre de variable
string currentId = "";
//Guarda nombre de clase 
string className = "";

//Stack de valores de matriz
stack<int> matrixDir;

//Saltos de dirección
stack<int> jDir;

//Nombres de variables
stack<string> forID;

//Lista de variables para asignar
vector<struct varDims> currentIds;

//////////////////////////////
//SET Y GETS DE FLAGS Y LISTAS
//////////////////////////////
void setClassName(string dir){
    className = dir;
}

string getClassName(){
    return className;
}

void setforID(string dir){
    forID.push(dir);
}

string getforID(){
    string top = forID.top();
    forID.pop();
    return top;
}

void setJDir(int dir){
    jDir.push(dir);
}

int getJDir(){
    int top = jDir.top();
    jDir.pop();
    return top;
}

void setCurrentId(string id){
    currentId = id;
}

string getCurrentId(string id){
    return currentId;
}

void resetID(){
    currentIds.clear();
    currentType = -1;
};

void setID(std::string id,int dim1, int dim2){
    struct varDims vari = varDims();
    vari.id = id;
    vari.dim1 = 0;
    vari.dim2 = 0;
    currentIds.push_back(vari);
};

void setCurrentType(int type){
    currentType = type;
}

void resetCurrentType(){
    currentType = -1;
}

