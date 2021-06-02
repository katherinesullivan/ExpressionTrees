#include "interprete.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 99
#define MAX_ALIAS 32
#define MAX_OP 3

int interpretar(TablaOps* tabla, TablaHash** dicc, char* buffer) {
    printf("> ");
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
            if (arbol){
                char* rdo = malloc(sizeof(char)*MAX_INPUT);
                rdo[0] = '\0';
                rdo = print_infix(arbol, tabla, rdo, 0);
                printf("La expresión es: %s\n", rdo);
                free(rdo);
            }
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
            if (arbol) {
                resolver(arbol, tabla);
                printf("La expresión evalúa a %d\n", resolver(arbol, tabla));
            }
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
        if (tablahash_buscar(*dicc, primera_palabra,1)) tablahash_eliminar(*dicc, primera_palabra);
        Arbol arbol = crear_expr_tree(expr, tabla);
        tablahash_insertar(*dicc, primera_palabra, arbol);
    }
    else {
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

void destruir_stack(StackNode* top) {
    if (top != NULL) {
        destruir_stack(top->next);
        arbol_destruir(top->t);
        free(top);
    }
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

    for(unsigned int i = 0; i<strlen(expr); i++) {
        printf("expr[%d]: %c\n", i, expr[i]);
        // ver que no imprime bien

        if (expr[i] == ' ') {printf("Hola soy un espacio\n");}

        else {
            /*if (expr[i] == '-' && expr[i+1] == '-') {
                // Caso distinguido operador especial --
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
                t->izq = NULL;
                push(&stack, t);
            }*/

            /*else {*/
            int bandera = 0;
            int j = 0;
            char* simbolo = malloc(sizeof(char)*MAX_OP);
            while(!bandera && j < tabla->num_elementos) {
                int length = 0;
                sscanf(expr+i, "%s%n", simbolo, &length);
                simbolo[length] = '\0';
                printf("Simbolo: %s Con length: %d\n", simbolo, length);
                // Estamos en el caso de un operador
                if (!strcmp(simbolo, tabla->array[j]->simbolo)) {
                    t = crear_nodo(simbolo);
                    int aridad2 = (tabla->array[j]->aridad)-1; 
                    if (!is_empty(stack))t1 = pop(&stack);
                    else {
                        printf("Operador mal posicionado. Asegúrese que su expresión esté en notación postfija\n");
                        arbol_destruir(t);
                        destruir_stack(stack);
                        return NULL;
                    }
                    t->der = t1;
                    if (aridad2) {
                        if (!is_empty(stack))t2 = pop(&stack);
                        else {
                            printf("Operador mal posicionado. Asegúrese que su expresión esté en notación postfija\n");
                            arbol_destruir(t);
                            destruir_stack(stack);
                            return NULL;
                        }
                        t->izq = t2;
                    }
                    if (length > 1) {
                        i+=length;
                    }
                    push(&stack, t);
                    bandera = 1;
                }
                j++;
            }

            // Si no se trataba de un operador, se trata de un operando
            if (!bandera) {
                free(simbolo);
                char* operando = malloc(sizeof(char)*10);
                if (sscanf(expr+i, "%s", operando) == 1) {
                    i+=strlen(operando);
                    printf("Length del operando: %ld\n", strlen(operando));
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
            /*}*/
        }
    }

    // Creo que queda vacia con esto la stack pero habría que ver
    t = pop(&stack);
    printf("-----Inorder tree traversal:-----\n");
    arbol_imprimir_inorder(t);
    printf("---------------------------------\n");
    if (!is_empty(stack)) {
        printf("No se pudo construir su árbol de expresión. Asegúrese de haber utilizado notación postfija y haber definido solo una expresión.\n");
        t1 = pop(&stack);
        arbol_destruir(t1);
        destruir_stack(stack);
        arbol_destruir(t);
        return NULL;
    }
    return t;
}

char* print_infix(Arbol arbol, TablaOps* tabla, char* rdo, int prec_parent) {
    /* Voy a realizar un recorrido inorder y ponerlo en una string 
       pero teniendo en cuenta de poner paréntesis si tengo algo de
       menor precedencia como hijo de algo con mayor precedencia. */

    if (!arbol) return rdo;

    // Caso Hoja 
    if (!arbol->izq && !arbol->der) {
        rdo = print_infix(arbol->izq, tabla, rdo, prec_parent);
        sprintf(rdo+strlen(rdo), "%s", arbol->dato);
        rdo = print_infix(arbol->der, tabla, rdo, prec_parent);
    }
    // Caso símbolo
    else {
        int i = 0;
        int band = 1;
        while (band && i<tabla->num_elementos) {
            if (!strcmp(arbol->dato, tabla->array[i]->simbolo)) {
                printf("Soy %s y tengo precedencia %d\n", arbol->dato, tabla->array[i]->precedencia);
                if (tabla->array[i]->precedencia < prec_parent) sprintf(rdo+strlen(rdo), "(");
                rdo = print_infix(arbol->izq, tabla, rdo, tabla->array[i]->precedencia);
                sprintf(rdo+strlen(rdo), "%s", arbol->dato);
                rdo = print_infix(arbol->der, tabla, rdo, tabla->array[i]->precedencia);
                band = 0;
                if (tabla->array[i]->precedencia < prec_parent) sprintf(rdo+strlen(rdo), ")");
            }
            i++;
        }
        if (band) {
            // No es una hoja pero no hay un símbolo
            printf("Hubo un problema imprimiendo su impresión. Fijése haberla definido de manera correcta y de haber cargado todos los operadores correspondientes \n");
            return NULL;
        }
    }


    
    return rdo;
}

int resolver(Arbol arbol, TablaOps* tabla) {
    // Árbol vacío  
    if (!arbol) return 0;

    // Hoja 
    if ((!arbol->izq) && (!arbol->der)) return atoi(arbol->dato);

    // Caso operador 
    int valor_izq = resolver(arbol->izq, tabla);
    int valor_der = resolver(arbol->der, tabla);
    int* args = malloc(sizeof(int)*2);

    int i = 0;
    while(i<tabla->num_elementos) {
        if (!strcmp(arbol->dato, tabla->array[i]->simbolo)) {
            if (tabla->array[i]->aridad == 1) {
                args[0] = valor_der;
                args[1] = valor_izq; // no va a ser usado 0
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
    printf("Hubo un problema evaluando su expresión. Fijése haberla definido de manera correcta y de haber cargado todos los operadores correspondientes\n");

    return 0;
}

/*unsigned hash(char *s) {
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
    printf("Size of arbol: %ld\n", sizeof(Arbol));
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
*/