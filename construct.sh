#!/bin/bash

bison -d parser.yy
flex lexer.l
g++ main.cpp Scanner.cpp driver.cpp parser.tab.cc semantic.cpp VMachine.cpp -o CMammoth
./CMammoth "finalTest.txt"