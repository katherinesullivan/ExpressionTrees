#include <stdlib.h>
#include <math.h> // acordate de incluir -lm
#include <stdio.h>
#include <string.h>
#include "operadores.h"

TablaOps* crear_tabla() {
    TablaOps* tabla = malloc(sizeof(TablaOps));
    tabla->head = NULL;
    tabla->cant = 0;
    return tabla;
}

void cargar_operador(TablaOps* tabla, char* simbolo, int aridad, FuncionEvaluacion eval) {
    NodoTablaOps* nuevo_nodo = malloc(sizeof(NodoTablaOps));
    nuevo_nodo->simbolo = simbolo;
    nuevo_nodo->aridad = aridad;
    nuevo_nodo->eval = eval;
    nuevo_nodo->sig = tabla->head;

    tabla->head = nuevo_nodo;
    tabla->cant++;
}

int suma (int* args) {
    return args[0] + args[1];
}
int resta (int* args) {
    return args[0] - args[1];
}
int opuesto (int* args) {
    return -args[0];
}
int producto (int* args){
    return args[0]*args[1];
}
int division (int* args){
    return args[0]/args[1];
}
int modulo (int* args){
    return args[0]%args[1];
}
int potencia (int* args){
    return pow(args[0],args[1]);
}

void cargar_operadores_tradicionales (TablaOps* tabla) {
    cargar_operador(tabla, "+", 2, suma);
    cargar_operador(tabla, "-", 2, resta);
    cargar_operador(tabla, "--", 1, opuesto);
    cargar_operador(tabla, "*", 2, producto);
    cargar_operador(tabla, "/", 2, division);
    cargar_operador(tabla, "%", 2, modulo);
    cargar_operador(tabla, "^", 2, potencia);
}

void imprimir_tablaops (TablaOps* tabla) {
    NodoTablaOps* nodo = tabla->head;
    for (int i=0;i<tabla->cant;i++) {
        printf("Operador %d: Tiene símbolo = %s y aridad = %d\n", i+1, nodo->simbolo, nodo->aridad);
        int* args = malloc(sizeof(int)*2);
        args[0] = 6;
        args[1] = 3;
        printf("Una muestra de su función con argumentos 6 y 3 es: %d\n\n", nodo->eval(args));
        nodo = nodo->sig;
    }
}

/*int main () {
    printf("%%%%\n"); // 2 %
    char* c = malloc(sizeof(char)*3);
    scanf("%s", c);
    printf("%s\n", c);
    if (strcmp(c,"%") == 0) {
        printf("yes\n");
    }
    TablaOps* tabla = crear_tabla();
    cargar_operadores_tradicionales(tabla);
    imprimir_tablaops(tabla);
    printf("Dos^Tres: %d\n", 2^3);
    return 0;
}*/