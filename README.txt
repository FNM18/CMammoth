******************************************
    Estructuras Sintácticas del Lenguaje
******************************************
Declaración de Programa
    Program NOMBRE_PROGRAMA :

Declaración de Comentario
    // LINEA_TEXTO

    /* 
     LINEAS_TEXTO    
    */

Declaración de Variable
    NOMBRE_VARIABLE : TIPO_VARIABLE ;
    NOMBRE_VARIABLE, NOMBRE_VARIABLE, ... : TIPO_VARIABLE ;
    NOMBRE_VARIABLE [DIMENSION_1 , DIMENSION_2] : TIPO_VARIABLE ;

Declaración de Variable (Global)
    variables
    NOMBRE_VARIABLE : TIPO_VARIABLE ;
    NOMBRE_VARIABLE, NOMBRE_VARIABLE, ... : TIPO_VARIABLE ;
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
    for NOMBRE_VARIABLE = VALOR_INCIAL in  EXPRESIÓN do {
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

Declaración de Read
    read(NOMBRE_VARIABLE);

Acesso a Variable/Atributo
    NOMBRE_CLASE.NOMBRE_VARIABLE[EXPRESIÓN ENTERA,EXPRESIÓN ENTERA]

Llamada de Función/Método
    NOMBRE_CLASE.NOMBRE_FUNCIÓN(PARÁMETROS) 
