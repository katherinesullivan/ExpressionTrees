#ifndef __OPERADORES_H__
#define __OPERADORES_H__

#include <stddef.h>

/* Tipo para las funciones evaluadoras */
typedef int (*FuncionEvaluacion) (int *args);

/* Estructura para el nodo de la tabla de operadores */
typedef struct _NodoTablaOps {
  char *simbolo;
  int aridad;
  FuncionEvaluacion eval;
  int precedencia;
  int asoc;
  struct _NodoTablaOps *sig;
} NodoTablaOps;

/* Estructura para la tabla de operadores */
typedef struct _TablaOps {
  NodoTablaOps **array;
  int cant;
  int num_elementos;
} TablaOps;

/* Función que permite crear una tabla de operadores */
TablaOps *crear_tabla();

/* Función provista para cargar operadores individulamente a la tabla */
void cargar_operador(TablaOps * tabla, char *simbolo, int aridad,
                     FuncionEvaluacion eval, int precedencia, int asoc);

/* Función que permite aumentar la capacidad de la tabla de operadores */
void agrandar_tablaops(TablaOps * tabla);

/* Destruye una tabla de operadores */
void destruir_tablaops(TablaOps * tabla);

/* Funciones evaluadoras a ser usadas */
int suma(int *args);
int resta(int *args);
int opuesto(int *args);
int producto(int *args);
int division(int *args);
int modulo(int *args);
int potencia(int *args);

/* Función útil para cargar los operadores que se muestran dentro 
del enunciado a la tabla */
void cargar_operadores_tradicionales(TablaOps * tabla);

/* Función útil para imprimir una tabla de operadores */
void imprimir_tablaops(TablaOps * tabla);

#endif                          /* __OPERADORES_H__ */
