#include "interprete.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 99
#define MAX_ALIAS 32

int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer) {
    fgets(buffer, MAX_INPUT, stdin);
    buffer[strlen(buffer)] = '\0';

    char* primera_palabra = malloc(sizeof(char)*MAX_ALIAS);
    sscanf(buffer, "%s ", primera_palabra);

    if (!strcmp(primera_palabra,"salir")) {
        printf("Protocolo de salida\n");
        free(primera_palabra);
        tablahash_destruir(*dicc);
        
        return 0;
    }
    
    if (!strcmp(primera_palabra,"imprimir")) {
        printf("Imprimir: Chequeo que haya un alias, luego que ese alias este en el dicc, y luego ejecuto printf infix\n");
        free(primera_palabra);
        return 1;
    }

    if (!strcmp(primera_palabra,"evaluar")) {
        printf("Evaluar: Chequeo que haya un alias, luego que ese alias este en dicc, y luego ejecuto resolver\n");
        free(primera_palabra);
        return 1;
    }

    // Si la primer palabra del comando no fue igual a ninguna de las palabras claves, vamos a suponer que representa un alias
    printf("El alias es %s. Voy a chequear que venga seguido por un igual usando el rdo de scanf que luego tenga un cargar y luego una expresión (podría ver que la expresión es válida) y luego cargo el alias con el dato armado al dicc\n", primera_palabra);
    // SOLO NO ACA free palabra o en

    return 1;
}

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
    char* buffer = malloc(sizeof(char)*MAX_INPUT);
    TablaHash* diccionario = tablahash_crear(31, hash, &hash2);
    TablaOps* tablaops = crear_tabla();
    cargar_operadores_tradicionales(tablaops);
    do {
        flag = interpretar(tablaops, &diccionario, buffer);
    } while (flag);
    return 0;
}
