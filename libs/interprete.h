#ifndef __INTERPRETE_H__ 
#define __INTERPRETE_H__

#include <stddef.h>
#include "operadores.h"
#include "tablahash.h"

int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer);

TablaHash* crear_dicc_alias();

Arbol crear_expr_tree(char* expr); // ver https://www.geeksforgeeks.org/expression-tree/

void agregar_alias(TablaHash* dicc);

void printf_infix(CasillaHash arbol); // tener en cuenta parentesis (van a ser segun el nivel y si son + -)

void resolver(CasillaHash arbol);

/* Una vez ingresada la palabra salir, se libera la memoria asignada a la tabla de operaciones y
 al diccionario de arboles de expresiones*/
void salir(TablaOps* tabla, TablaHash* dicc);

/* Chequea que posterior a la primer palabra clave se ingrese algo respetando el formato */
int chequeo_formato(int entered, int expected);

/* Chequeo que el alias no empiee con un número y no sea ninguna de las palabras claves */
int chequeo_alias_valido(char* alias);

#endif /* __INTERPRETE_H__ */