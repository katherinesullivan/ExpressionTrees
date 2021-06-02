#include "interprete.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 99
#define MAX_ALIAS 32
#define MAX_OP 3

int interpretar(TablaOps * tabla, TablaHash ** dicc, char *buffer) {
  printf("> ");
  buffer[0] = '\0';
  fgets(buffer, MAX_INPUT, stdin);
  buffer[strlen(buffer)] = '\0';

  char *primera_palabra = malloc(sizeof(char) * MAX_ALIAS);
  primera_palabra[0] = '\0';
  sscanf(buffer, "%s ", primera_palabra);

  // Nos fijamos cuál fue el comando ingresado 

  if (!strcmp(primera_palabra, "salir")) {
    free(primera_palabra);
    tablahash_destruir(*dicc);
    destruir_tablaops(tabla);
    free(buffer);
    return 0;
  }

  if (!strcmp(primera_palabra, "imprimir")) {
    char *alias = malloc(sizeof(char) * MAX_ALIAS);
    if (chequeo_formato
        (sscanf(buffer + strlen(primera_palabra), "%s", alias), 1)) {
      // Vemos si existe el alias especificado
      Arbol arbol = tablahash_buscar(*dicc, alias, 0);
      if (arbol) {
        char *rdo = malloc(sizeof(char) * MAX_INPUT);
        rdo[0] = '\0';
        rdo = print_infix(arbol, tabla, rdo, 0);
        printf("La expresión es: %s\n", rdo);
        free(rdo);
      } else
        printf("No existe expresión para imprimir con ese alias\n");
    }
    free(primera_palabra);
    free(alias);
    return 1;
  }

  if (!strcmp(primera_palabra, "evaluar")) {
    char *alias = malloc(sizeof(char) * MAX_ALIAS);
    if (chequeo_formato
        (sscanf(buffer + strlen(primera_palabra), "%s", alias), 1)) {
      // Vemos si existe una expresión con ese nombre
      Arbol arbol = tablahash_buscar(*dicc, alias, 0);
      if (arbol) {
        // Tenemos un chequeo para ver si fue posible
        // el cálculo de la expresuón
        int *error = malloc(sizeof(int));
        *error = 0;
        int resultado = resolver(arbol, tabla, error);
        if (*error)
          printf("No se puede calcular una división por cero.\n");
        else
          printf("La expresión evalúa a %d\n", resultado);
        free(error);
      } else
        printf("No existe expresión para resolver con ese alias\n");
    }
    free(primera_palabra);
    free(alias);
    return 1;
  }
  
  // Si la primer palabra del comando no fue igual a ninguna de las palabras claves,
  // vamos a suponer que representa un alias
  char *expr = malloc(sizeof(char) * (MAX_INPUT - MAX_ALIAS - 10));
  if (chequeo_formato
      (sscanf(buffer + strlen(primera_palabra), " = cargar %[^\n]\n", expr),
       1)) {
    // Si ya había una expresión con ese nombre la piso
    if (tablahash_buscar(*dicc, primera_palabra, 1))
      tablahash_eliminar(*dicc, primera_palabra);
    Arbol arbol = crear_expr_tree(expr, tabla);
    tablahash_insertar(*dicc, primera_palabra, arbol);
  } else {
    free(primera_palabra);
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

void destruir_stack(StackNode * top) {
  if (top != NULL) {
    destruir_stack(top->next);
    arbol_destruir(top->t);
    free(top);
  }
}

int is_empty(StackNode * top) {
  if (top == NULL) {
    return 1;
  }
  return 0;
}

void push(StackNode ** top_ref, Arbol t) {
// Pedimos memoria para el elemento
  StackNode *new_tNode = (StackNode *) malloc(sizeof(StackNode));

  // Ponemos la información
  new_tNode->t = t;

  // Lo linkeamos a la lista vieja
  new_tNode->next = (*top_ref);

  // Movemos el puntero del tope de la pila al elemento ingresado
  (*top_ref) = new_tNode;
}

// Función para popear el elemento del tope de una pila
Arbol pop(StackNode ** top_ref) {
  Arbol res;
  StackNode *top;

  // Si está vacía tenemos un error
  if (is_empty(*top_ref)) {
    printf("Stack Underflow \n");
    getchar();
    exit(0);
  } else {
    top = *top_ref;
    res = top->t;
    *top_ref = top->next;
    free(top);
    return res;
  }
}

Arbol crear_expr_tree(char *expr, TablaOps * tabla) {
  StackNode *stack = NULL;
  Arbol t, t1, t2;

  // Recorremos los operandos y operadores de la expresión
  // guardándolos en una pila 
  for (unsigned int i = 0; i < strlen(expr); i++) {

    if (expr[i] == ' ') {
    }

    else {
      int bandera = 0;
      int j = 0;
      char *simbolo = malloc(sizeof(char) * MAX_OP);
      while (!bandera && j < tabla->num_elementos) {
        int length = 0;
        sscanf(expr + i, "%s%n", simbolo, &length);
        simbolo[length] = '\0';

        // Vemos si se trata de un operador
        if (!strcmp(simbolo, tabla->array[j]->simbolo)) {
          t = crear_nodo(simbolo);
          int aridad2 = (tabla->array[j]->aridad) - 1;
          if (!is_empty(stack))
            t1 = pop(&stack);
          else {
            printf("Operador mal posicionado. ");
            printf
                ("Asegúrese que su expresión esté en notación postfija\n");
            arbol_destruir(t);
            destruir_stack(stack);
            return NULL;
          }
          t->der = t1;
          if (aridad2) {
            if (!is_empty(stack))
              t2 = pop(&stack);
            else {
              printf("Operador mal posicionado. ");
              printf
                  ("Asegúrese que su expresión esté en notación postfija\n");
              arbol_destruir(t);
              destruir_stack(stack);
              return NULL;
            }
            t->izq = t2;
          }
          if (length > 1) {
            i += length;
          }
          push(&stack, t);
          bandera = 1;
        }
        j++;
      }

      // Si no se trataba de un operador, se trata de un operando
      if (!bandera) {
        free(simbolo);
        char *operando = malloc(sizeof(char) * 10);
        if (sscanf(expr + i, "%s", operando) == 1) {
          i += strlen(operando);
          t = crear_nodo(operando);
          push(&stack, t);
        } else {
          printf
              ("Hubo un error leyendo su expresión. Asegúrese de que la misma");
          printf
              ("esté expresada en notación postfija y use los símbolos cargados.\n");
          destruir_stack(stack);
          return NULL;
        }
      }
    }
  }

  t = pop(&stack);
  /*printf("-----Inorder tree traversal:-----\n");
     arbol_imprimir_inorder(t);
     printf("---------------------------------\n"); */
  if (!is_empty(stack)) {
    printf("No se pudo construir su árbol de expresión. ");
    printf("Asegúrese de haber utilizado notación postfija y ");
    printf("haber definido solo una expresión.\n");
    t1 = pop(&stack);
    arbol_destruir(t1);
    destruir_stack(stack);
    arbol_destruir(t);
    return NULL;
  }
  return t;
}

char *print_infix(Arbol arbol, TablaOps * tabla, char *rdo, int prec_parent) {
  /* Voy a realizar un recorrido inorder y ponerlo en una string 
     pero teniendo en cuenta de poner paréntesis si tengo algo de
     menor precedencia como hijo de algo con mayor precedencia. */

  if (!arbol)
    return rdo;

  // Caso operando
  if (!arbol->izq && !arbol->der) {
    rdo = print_infix(arbol->izq, tabla, rdo, prec_parent);
    sprintf(rdo + strlen(rdo), "%s", arbol->dato);
    rdo = print_infix(arbol->der, tabla, rdo, prec_parent);
  }
  // Caso operador
  else {
    int i = 0;
    int band = 1;
    while (band && i < tabla->num_elementos) {
      if (!strcmp(arbol->dato, tabla->array[i]->simbolo)) {
        if (tabla->array[i]->precedencia < prec_parent)
          sprintf(rdo + strlen(rdo), "(");
        rdo = print_infix(arbol->izq, tabla, rdo, tabla->array[i]->precedencia);
        sprintf(rdo + strlen(rdo), "%s", arbol->dato);
        rdo = print_infix(arbol->der, tabla, rdo, tabla->array[i]->precedencia);
        band = 0;
        if (tabla->array[i]->precedencia < prec_parent)
          sprintf(rdo + strlen(rdo), ")");
      }
      i++;
    }
    if (band) {
      // No es una hoja pero no hay un símbolo
      printf("Hubo un problema imprimiendo su expresión. ");
      printf("Fijése de haberla definido de manera correcta y de haber ");
      printf("cargado todos los operadores correspondientes \n");
      return NULL;
    }
  }
  return rdo;
}

int resolver(Arbol arbol, TablaOps * tabla, int *error) {
  // Árbol vacío  
  if (!arbol)
    return 0;

  // Hoja 
  if ((!arbol->izq) && (!arbol->der))
    return atoi(arbol->dato);

  // Calculo los valores a derecha e izquierda para luego 
  // obtener el resultado final
  int valor_izq = resolver(arbol->izq, tabla, error);
  int valor_der = resolver(arbol->der, tabla, error);
  int *args = malloc(sizeof(int) * 2);

  int i = 0;
  while (i < tabla->num_elementos) {
    if (!strcmp(arbol->dato, tabla->array[i]->simbolo)) {
      if (!strcmp(arbol->dato, "/") || !strcmp(arbol->dato, "%")) {
        args[1] = valor_der;
        if (args[1] == 0) {
          // Encontramos una división por cero,
          // único comportamiento no definido con
          // los operadores tradicionales.
          *error = 1;
          free(args);
          return 1;
        }
      }
      if (tabla->array[i]->aridad == 1) {
        args[0] = valor_der;
        args[1] = valor_izq;    // no va a ser usado
        int rdo = tabla->array[i]->eval(args);
        free(args);
        return rdo;
      }
      args[0] = valor_izq;
      args[1] = valor_der;
      int rdo = tabla->array[i]->eval(args);
      free(args);
      return rdo;
    }
    i++;
  }

  // No se encontró el símbolo
  free(args);
  printf("Hubo un problema evaluando su expresión. ");
  printf("Fijése de haberla definido de manera correcta y de haber ");
  printf("cargado todos los operadores correspondientes \n");

  return 0;
}
