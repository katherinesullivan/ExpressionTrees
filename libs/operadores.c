#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "operadores.h"

#define CANT_OPERADORES 10

TablaOps *crear_tabla() {
  TablaOps *tabla = malloc(sizeof(TablaOps));
  NodoTablaOps **array = malloc(sizeof(NodoTablaOps *) * CANT_OPERADORES);
  tabla->array = array;
  tabla->cant = CANT_OPERADORES;
  tabla->num_elementos = 0;
  return tabla;
}

void cargar_operador(TablaOps * tabla, char *simbolo, int aridad,
                     FuncionEvaluacion eval, int prec, int asoc) {
  NodoTablaOps *nuevo_nodo = malloc(sizeof(NodoTablaOps));
  nuevo_nodo->simbolo = simbolo;
  nuevo_nodo->aridad = aridad;
  nuevo_nodo->eval = eval;
  nuevo_nodo->precedencia = prec;
  nuevo_nodo->asoc = asoc;

  tabla->array[tabla->num_elementos] = nuevo_nodo;
  tabla->num_elementos++;

  double factor_carga = tabla->num_elementos / tabla->cant;
  if (factor_carga >= 0.8) {
    agrandar_tablaops(tabla);
  }
}

void agrandar_tablaops(TablaOps * tabla) {
  tabla->array =
      realloc(tabla->array, sizeof(NodoTablaOps *) * tabla->cant * 2);
  tabla->cant = tabla->cant * 2;
}

int suma(int *args) {
  return args[0] + args[1];
}
int resta(int *args) {
  return args[0] - args[1];
}
int opuesto(int *args) {
  return 0 - args[0];
}
int producto(int *args) {
  return args[0] * args[1];
}
int division(int *args) {
  return args[0] / args[1];
}
int modulo(int *args) {
  return args[0] % args[1];
}
int potencia(int *args) {
  return pow(args[0], args[1]);
}

void cargar_operadores_tradicionales(TablaOps * tabla) {
  cargar_operador(tabla, "+", 2, suma, 0, 1);
  cargar_operador(tabla, "-", 2, resta, 0, 0);
  cargar_operador(tabla, "--", 1, opuesto, 2, 0);
  cargar_operador(tabla, "*", 2, producto, 1, 1);
  cargar_operador(tabla, "/", 2, division, 1, 0);
  cargar_operador(tabla, "%", 2, modulo, 1, 0);
  cargar_operador(tabla, "^", 2, potencia, 3, 0);
}

void imprimir_tablaops(TablaOps * tabla) {
  NodoTablaOps *nodo;
  for (int i = 0; i < tabla->num_elementos; i++) {
    nodo = tabla->array[i];
    printf("Operador %d: Tiene s??mbolo = %s y aridad = %d\n", i + 1,
           nodo->simbolo, nodo->aridad);
    int *args = malloc(sizeof(int) * 2);
    args[0] = 6;
    args[1] = 3;
    printf("Una muestra de su funci??n con argumentos 6 y 3 es: %d\n\n",
           nodo->eval(args));
    free(args);
  }
}

void destruir_tablaops(TablaOps * tabla) {
  for (int i = 0; i < tabla->num_elementos; i++) {
    free(tabla->array[i]);
  }
  free(tabla->array);
  free(tabla);
}
