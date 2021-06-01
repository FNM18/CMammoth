EJECUCIÓN DEL COMPILADOR:

Instalaciones necesarias:
	•Flex : herramienta para generar un  analizador léxico 
		-Descarga: https://github.com/westes/flex
	•Bison : herramienta para generar un  analizador sintáctico (convierte gramática libre de contexto a un parser LR a través de LALR)
		-Descarga: https://www.gnu.org/software/bison/
	•GCC (recomendado) : herramienta de compilación, este caso utilizado para C++
		-Descarga: https://gcc.gnu.org/

La clase main dentro del archivo main.cpp establece la ejecución del archivo test.txt la cual ya se encuentra referenciada para solo hacer alteraciones en el documento de entrada.

Los comandos utilizados son los siguientes (Linux):


	•flex lexer.l
		-Compila el archivo y genera la clase Scanner para C++
	•bison -d parser.yy
		-Compila el archivo formato Bison y genera las clases de C++.
	•c++ Main.cpp Scanner.cpp driver.cpp parser.tab.cc
		-Se compilan en conjunto las siguientes clases generadas para hacer un archivo de salida ejecutable.
	•./a.out
		-Corre el ejecutable creado anteriormente.

Se adjunta un archivo Bash para ejecutar esas líneas de comando y así agilizar el proceso de construcción y ejecución. El archivo es nombrado construct.sh. 
(Ejecución → ./construct.sh)

Si ya se hizo la construcción del archivo solo es necesario correr el comando de ejecución para probar el programa. Los únicos cambios para probar el programa es cambiar el contenido del archivo de entrada (test.txt) para poder realizar las pruebas de código requeridas.

En la entrega vienen los archivos ya generados por las herramientas por lo cual se puede optar por solamente correr el ejecutable: ./a.out de ser en Linux.

Este lenguaje no es sensible a indentaciones pero si es sensible a la capitalización de las letras. Se requiere una función main al final del programa para ejecutar los elementos deseados durante el runtime.

Declaración de Programa
    Program NOMBRE_PROGRAMA :

Declaración de Comentario
    // LINEA_TEXTO

    /* 
     LINEAS_TEXTO    
    */

Declaración de Variable
    NOMBRE_VARIABLE [DIMENSION_1 , DIMENSION_2] : TIPO_VARIABLE ;

Declaración de Variable (Global)
    variables
    NOMBRE_VARIABLE [DIMENSION_1 , DIMENSION_2] : TIPO_VARIABLE ;

Declaración de If-Else
    if(CONDICIONES…) then {
        BLOQUES...
    } else {
    	BLOQUES...
    }

Declaración de While
    while (CONDICIONES…) do {
	BLOQUES...
    }

Declaración de For
    for NOMBRE_VARIABLE = VALOR_INCIAL do {
   	 BLOQUES...
    }

Declaración de Función
    TIPO_RETORNO function NOMBRE_FUNCIÓN( PARÁMETRO_1, PARÁMETRO_N)
	Variables     
    		VARIABLES...
    {
        BLOQUES…
        return NOMBRE_VARIABLE/EXPRESIÓN/FUNCIÓN/ETC ;
    };

Declaración de Clase
    Class NOMBRE_CLASE {
        attributes
            VARIABLES...
        methods
            FUNCIONES...
   };

Declaración de Objeto
    NOMBRE_OBJETO : NOMBRE_CLASE  ;

Declaración de Write
    write(“TEXTO” , NOMBRE_VARIABLE/EXPRESIÓN/FUNCIÓN/ETC);

Declaración de Write
    read(NOMBRE_VARIABLE/EXPRESIÓN/FUNCIÓN/ETC);

Acesso a Variable/Atributo
    NOMBRE_CLASE.NOMBRE_VARIABLE[NUM_RENGLÓN,NUM_COLUMNA]

Llamada de Función/Método
    NOMBRE_CLASE.NOMBRE_FUNCIÓN(PARÁMETROS) 

