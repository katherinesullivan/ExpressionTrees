#ifndef __OPERADORES_H__ 
#define __OPERADORES_H__

#include <stddef.h>

typedef int (*FuncionEvaluacion) (int* args);

typedef struct _NodoTablaOps {
    char* simbolo;
    int aridad;
    FuncionEvaluacion eval;
    struct _NodoTablaOps* sig;
} NodoTablaOps;

typedef struct _TablaOps {
    NodoTablaOps** array;
    int cant;
    int num_elementos;
} TablaOps;

TablaOps* crear_tabla();

void cargar_operador(TablaOps* tabla, char* simbolo, int aridad, FuncionEvaluacion eval);

void agrandar_tablaops (TablaOps* tabla);

int suma (int* args);
int resta (int* args);
int opuesto (int* args);
int producto (int* args);
int division (int* args);
int modulo (int* args);
int potencia (int* args);

void cargar_operadores_tradicionales (TablaOps* tabla);

void imprimir_tablaops (TablaOps* tabla);

#endif /* __OPERADORES_H__ */