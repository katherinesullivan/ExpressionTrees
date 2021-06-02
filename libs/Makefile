# Makefile

# Macros
OPERADORES = operadores.c
INTERPRETE = interprete.c
TABLAHASH = tablahash.c
FLAGS = -Wall -Wextra -Werror

# Ejecutable de la implementación del Juego.
main: main.c $(INTERPRETE:.c=.o) $(TABLAHASH:.c=.o) $(OPERADORES:.c=.o)
	$(CC) $(FLAGS) -o $@ $^ -lm

# Archivo objeto de la implementación de TABLAHASH.
$(TABLAHASH:.c=.o) : $(TABLAHASH) $(TABLAHASH:.c=.h)
	$(CC) $(FLAGS) -c $(TABLAHASH)

# Archivo objeto de la implementación de Tableros.
$(OPERADORES:.c=.o) : $(OPERADORES) $(OPERADORES:.c=.h)
	$(CC) $(FLAGS) -c $(OPERADORES)

# Archivo objeto de la implementación del Juego.
$(INTERPRETE:.c=.o) : $(INTERPRETE) $(INTERPRETE:.c=.h) $(OPERADORES:.c=.h) $(TABLAHASH:.c=.h)
	$(CC) $(FLAGS) -c $(INTERPRETE)


# Destino simbólico que borra todos los archivos con extensión .o cuando se invoca.
clean: 
	rm *.o

.PHONY : clean
