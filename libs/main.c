#include <stdio.h>
#include <stdlib.h>
#include "interprete.h"

#define MAX_INPUT 99

unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 11 * hashval;
    return hashval;
}

unsigned hash2(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 13 * hashval; // no puede ser 31 si voy a arrancar con 31 pq necesito la coprimalidad (es decir su mcd -> 1) entre el hash2 y la cap de la tabla
    return hashval;
}

int main() {
    int flag = 1;
    free(NULL);
    puts("hi!");
    char* buffer = malloc(sizeof(char)*MAX_INPUT);
    TablaHash* diccionario = tablahash_crear(31, hash, &hash2);
    TablaOps* tablaops = crear_tabla();
    cargar_operadores_tradicionales(tablaops);
    do {
        flag = interpretar(tablaops, &diccionario, buffer);
    } while (flag);
    return 0;
}
