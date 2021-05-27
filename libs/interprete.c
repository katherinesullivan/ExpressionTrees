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
        printf("Free1\n");
        tablahash_destruir(*dicc);
        printf("Tablahash destruida\n");
        destruir_tablaops(tabla);
        printf("TablaOps destruida\n");
        free(buffer);
        printf("Free buffer\n");
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
        Arbol arbol = crear_expr_tree(expr, tabla);
        tablahash_insertar(*dicc, primera_palabra, arbol);
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

void destruir_stack(StackNode* top) {
    destruir_stack(top->next);
    free(top);
}

int is_empty(StackNode* top) {
    if (top == NULL) {
        return 1;
    }
    return 0;
}

void push (StackNode** top_ref, Arbol t) {
// Pedimos memoria para el elemento
    StackNode* new_tNode = (StackNode*) malloc(sizeof(StackNode)); 

    // Ponemos la información
    new_tNode->t = t; 

    // Lo linkeamos a la lista vieja
    new_tNode->next = (*top_ref); 

    // Movemos el puntero del tope de la pila al elemento ingresado
    (*top_ref) = new_tNode; 
}

// Función para popear el elemento del tope de una pila
Arbol pop(StackNode** top_ref) { 
    Arbol res; 
    StackNode* top; 

    // Si está vacía tenemos un error
    if(is_empty(*top_ref)) { 
        printf("Stack Underflow \n"); 
        getchar(); 
        exit(0); 
    } 
    else{ 
        top = *top_ref; 
        res = top->t; 
        *top_ref = top->next; 
        free(top); 
        return res; 
    } 
} 

Arbol crear_expr_tree(char* expr, TablaOps* tabla) {
    StackNode* stack = NULL;
    Arbol t, t1, t2;

    for(int i = 0; i<strlen(expr); i++) {
        printf("expr[%d]: %c\n", i, expr[i]);
        // ver que no imprime bien

        if (expr[i] == ' ') {printf("Hola soy un espacio\n");}

        // Caso distinguido operador especial --
        else if (expr[i] == '-') {
            if (expr[i+1] == '-') {
                i++; // para no leer el prox caracter
                char* dato = malloc(sizeof(char)*3);
                dato[0] = '-';
                dato[1] = '-';
                dato[2] = '\0';
                t = crear_nodo(dato);
                if (!is_empty(stack))t1 = pop(&stack);
                else {
                    printf("Operador mal posicionado. Asegúrese que su expresión esté en notación postfija\n");
                    destruir_stack(stack);
                    return NULL;
                }
                t->der = t1;
                push(&stack, t);
            }
        }

        else {
            int bandera = 0;
            int j = 0;
            while(!bandera && j < tabla->num_elementos) {
                char* simbolo = malloc(sizeof(char)*2);
                sscanf(expr+i, "%c", simbolo);
                simbolo[1] = '\0';
                printf("Simbolo: %s\n", simbolo);
                // Estamos en el caso de un operador
                if (!strcmp(simbolo, tabla->array[j]->simbolo)) {
                    t = crear_nodo(simbolo);
                    if (!is_empty(stack))t1 = pop(&stack);
                    else {
                        printf("Operador mal posicionado. Asegúrese que su expresión esté en notación postfija\n");
                        destruir_stack(stack);
                        return NULL;
                    }
                    if (!is_empty(stack))t2 = pop(&stack);
                    else {
                        printf("Operador mal posicionado. Asegúrese que su expresión esté en notación postfija\n");
                        destruir_stack(stack);
                        return NULL;
                    }
                    t->izq = t1;
                    t->der = t2;
                    push(&stack, t);
                    bandera = 1;
                }
                j++;
            }

            // Si no se trataba de un operador, se trata de un operando
            if (!bandera) {
                char* operando = malloc(sizeof(char)*10);
                if (sscanf(expr+i, "%s", operando) == 1) {
                    i+=strlen(operando);
                    printf("Length del operando: %d\n", strlen(operando));
                    t = crear_nodo(operando);
                    push(&stack, t);
                }
                else {
                    printf("Hubo un error leyendo su expresión. Asegúrese de que la misma");
                    printf("esté expresada en notación postfija y use los símbolos cargados.\n"); 
                    destruir_stack(stack);
                    return NULL;
                }
            }


        }
    }
    // Creo que queda vacia con esto la stack pero habría que ver
    t = pop(&stack);
    return t;
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
