#ifndef __INTERPRETE_H__ 
#define __INTERPRETE_H__

#include <stddef.h>
#include "operadores.h"
#include "tablahash.h"

int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer);

TablaHash* crear_dicc_alias();

Arbol crear_expr_tree(char* expr, TablaOps* tabla); // ver https://www.geeksforgeeks.org/expression-tree/

/* Para la implementación de crear_expr_tree necesitaremos una pila */
// Estructura de un nodo perteneciente a una pila. 
// Implementación con listas simplemente enlazadas.
typedef struct _StackNode { 
  Arbol t; 
  struct _StackNode* next; 
} StackNode;

void destruir_stack(StackNode* top);

void push(StackNode** top_ref, Arbol t); 

Arbol pop(StackNode** top_ref); 

int is_empty(StackNode* top);

void agregar_alias(TablaHash* dicc);

void printf_infix(Arbol arbol); // tener en cuenta parentesis (van a ser segun el nivel y si son + -)

void resolver(Arbol arbol);

/* Una vez ingresada la palabra salir, se libera la memoria asignada a la tabla de operaciones y
 al diccionario de arboles de expresiones*/
void salir(TablaOps* tabla, TablaHash* dicc);

/* Chequea que posterior a la primer palabra se ingrese algo respetando el formato */
int chequeo_formato(int entered, int expected);

/* Chequeo que el alias no empiece con un número */
int chequeo_alias_valido(char* alias);

#endif /* __INTERPRETE_H__ */