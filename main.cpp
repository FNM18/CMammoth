#include <stdio.h>
#include "driver.h"
#include <stack>
#include <vector>
#include "VMachine.cpp"
using namespace std;


int main(int argc, char* argv[]){
  string fileName = "test.txt";
  string debug = "";
  if(argc > 3){
      throw CompilerError("Error: Too many arguments");
  }
  if(argc > 1){
    fileName = argv[1];
    cout << "Filename " << fileName << endl;
    if(argc == 3){
      debug = argv[2];
      cout << "Debug " << debug << endl;
      DEBUG = 1;
    }
  }

  //Analizador sintactico
  parser_driver driver;
  if(!driver.parse(fileName))  
    printf("\nCompiled Successfully\n");
  if(DEBUG){
    quads.printQuadList();
  }
  VMachine vm = VMachine();
  vm.executeFunctions(0);
  
}
