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
    hashval = *s + 13 * hashval;
  return hashval;
}

int main() {
  int flag = 1;
  char *buffer = malloc(sizeof(char) * MAX_INPUT);
  TablaHash *diccionario = tablahash_crear(31, hash, &hash2);
  TablaOps *tablaops = crear_tabla();
  cargar_operadores_tradicionales(tablaops);
  do {
    flag = interpretar(tablaops, &diccionario, buffer);
  } while (flag);
  return 0;
}
