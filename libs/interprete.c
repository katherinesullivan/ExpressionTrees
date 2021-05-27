#include "interprete.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 99
#define MAX_ALIAS 32

int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer) {
    printf(">");
    buffer[0] = '\0';
    fgets(buffer, MAX_INPUT, stdin);
    buffer[strlen(buffer)] = '\0';

    char* primera_palabra = malloc(sizeof(char)*MAX_ALIAS);
    primera_palabra[0] = '\0';
    sscanf(buffer, "%s ", primera_palabra);

    if (!strcmp(primera_palabra,"salir")) {
        printf("Protocolo de salida\n");
        free(primera_palabra);
        tablahash_destruir(*dicc);
        destruir_tablaops(tabla);
        free(buffer);
        return 0;
    }
    
    if (!strcmp(primera_palabra,"imprimir")) {
        char* alias = malloc(sizeof(char)*MAX_ALIAS);
        if (chequeo_formato(sscanf(buffer+strlen(primera_palabra), "%s", alias), 1)) {
            Arbol arbol = tablahash_buscar(*dicc, alias, 0);
            if (arbol) printf_infix(arbol);
            else printf("No existe expresión para imprimir con ese alias\n");
        }
        printf("%s\n", alias);

        printf("Imprimir: Chequeo que haya un alias, luego que ese alias este en el dicc, y luego ejecuto printf infix\n");
        free(primera_palabra);
        free(alias);
        return 1;
    }

    if (!strcmp(primera_palabra,"evaluar")) {
        char* alias = malloc(sizeof(char)*MAX_ALIAS);
        if (chequeo_formato(sscanf(buffer+strlen(primera_palabra), "%s", alias), 1)) {
            Arbol arbol = tablahash_buscar(*dicc, alias, 0);
            if (arbol) resolver(arbol);
            else printf("No existe expresión para resolver con ese alias\n");
        }
        printf("%s\n", alias);


        printf("Evaluar: Chequeo que haya un alias, luego que ese alias este en dicc, y luego ejecuto resolver\n");
        free(primera_palabra);
        free(alias);
        return 1;
    }

    // Si la primer palabra del comando no fue igual a ninguna de las palabras claves, vamos a suponer que representa un alias
    printf("El alias es %s. Voy a chequear que venga seguido por un igual usando el rdo de scanf que luego tenga un cargar y luego una expresión (podría ver que la expresión es válida) y luego cargo el alias con el dato armado al dicc\n", primera_palabra);
    // SOLO NO ACA free palabra
    char* expr = malloc(sizeof(char)*(MAX_INPUT-MAX_ALIAS-10));
    if (chequeo_formato(sscanf(buffer+strlen(primera_palabra), " = cargar %[^\n]\n", expr), 1)) {
        printf("Todo piola por aca\n");
        // Arbol arbol = crear_expr_tree(char* expr);
        // tablahash_insertar(*dicc, primera_palabra, arbol);
    }
    free(expr);

    return 1;
}

int chequeo_formato(int entered, int expected) {
  if (entered == expected) {
    return 1;
  }
  printf("Hubo un error al procesar lo que ingresó. Pruebe nuevamente.\n");
  return 0;
}

void printf_infix(Arbol arbol) {
    return;
}
void resolver(Arbol arbol) {
    return;
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
