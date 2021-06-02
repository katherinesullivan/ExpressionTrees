#ifndef __INTERPRETE_H__ 
#define __INTERPRETE_H__

#include <stddef.h>
#include "operadores.h"
#include "tablahash.h"

/* Función que maneja la parte interactiva del programa */
int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer);

/* A partir de una expresión construye un árbol de expresiones */
Arbol crear_expr_tree(char* expr, TablaOps* tabla);

/* Para la implementación de crear_expr_tree necesitaremos una pila */
// Estructura de un nodo perteneciente a una pila. 
// Implementación con listas simplemente enlazadas.
typedef struct _StackNode { 
  Arbol t; 
  struct _StackNode* next; 
} StackNode;

/* Destruye una pila */
void destruir_stack(StackNode* top);

/* Agrega un elemento a la pila */
void push(StackNode** top_ref, Arbol t); 

/* Elimina un elemento de la pila y lo devuelve */
Arbol pop(StackNode** top_ref); 

/* Devuelve 1 si la pila pasada está vacía, 0 en caso contrario */
int is_empty(StackNode* top);

/* Imprime la expresión del árbol en notación infija intentando usar la 
menor cantidad de paréntesis posibles */
char* print_infix(Arbol arbol, TablaOps* tabla, char* rdo, int prec_parent);

/* Dado un árbol de expresiones, la tabla de operadores, y una bandera de error
resuelve la expresión del árbol correctamente o indica un error */
int resolver(Arbol arbol, TablaOps* tabla, int* error);

/* Una vez ingresada la palabra salir, se libera la memoria asignada a la 
tabla de operaciones y al diccionario de arboles de expresiones */
void salir(TablaOps* tabla, TablaHash* dicc);

/* Chequea que posterior a la primer palabra se ingrese algo respetando el 
formato */
int chequeo_formato(int entered, int expected);

/* Chequeo que el alias no empiece con un número */
int chequeo_alias_valido(char* alias);

#endif /* __INTERPRETE_H__ */