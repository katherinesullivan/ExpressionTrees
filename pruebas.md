# Pruebas de funcionamiento del programa

## Pruebas con algunas expresiones tradicionales

    > alias = cargar 45 67 +
    > imprimir alias
    La expresión es: 45+67
    > evaluar alias
    La expresión evalúa a 112


    > alias1 = cargar 56 57 /
    > imprimir alias1 
    La expresión es: 56/57
    > evaluar alias1
    La expresión evalúa a 0
 

    > alias2 = cargar 45 67 * 9 6 ^ +
    > imprimir alias2
    La expresión es: 45*67+9^6
    > evaluar alias2
    La expresión evalúa a 534456


    > alias3 = cargar 57 56 / 0 +
    > imprimir alias3
    La expresión es: 57/56+0
    > evaluar alias3
    La expresión evalúa a 1


    > alias4 = cargar 45 46 % 45 / 1 *
    > imprimir alias4
    La expresión es: 45%46/45*1
    > evaluar alias4
    La expresión evalúa a 1


    > alias5 = cargar 45 -- 2 0 ^ * 94 34 % -
    > imprimir alias5
    La expresión es: --45*2^0-94%34
    > evaluar alias5
    La expresión evalúa a -71


## Pruebas para ver el uso correcto de paréntesis 

    > par = cargar 1025 25 + 5 *
    > imprimir par 
    La expresión es: (1025+25)*5
    > evaluar par
    La expresión evalúa a 5250


    > par1 = cargar 2 3 ^ 2 ^
    > imprimir par1
    La expresión es: (2^3)^2
    > evaluar par1
    La expresión evalúa a 64


    > par2 = cargar 2 3 2 ^ ^
    > imprimir par2
    La expresión es: 2^(3^2)
    > evaluar par2
    La expresión evalúa a 512


    > par3 = cargar 2 2 / 8 / 3 + 3 *
    > imprimir par3
    La expresión es: ((2/2)/8+3)*3
    > evaluar par3
    La expresión evalúa a 9


    > par4 = cargar 75 25 - -- 2 -- *
    > imprimir par4
    La expresión es: --(75-25)*--2
    > evaluar par4
    La expresión evalúa a 100


    > par5 = cargar 75 25 - 2 * --
    > imprimir par5
    La expresión es: --((75-25)*2)
    > evaluar par5
    La expresión evalúa a -100


## Pruebas de manejo de errores en expresiones 

    > expr = cargar 40 + 123
    Operador mal posicionado. Asegúrese que su expresión esté en notación postfija


    > expr1 = cargar 56 78 / 10 2 -- +
    No se pudo construir su árbol de expresión. Asegúrese de haber utilizado notación postfija y haber definido solo una expresión.


    > expr2 = cargar 45 67 89 1024
    No se pudo construir su árbol de expresión. Asegúrese de haber utilizado notación postfija y haber definido solo una expresión.


    > expr3 = cargar 45 67 % 78 (
    No se pudo construir su árbol de expresión. Asegúrese de haber utilizado notación postfija y haber definido solo una expresión.


    > expr4 = cargar 45 /
    Operador mal posicionado. Asegúrese que su expresión esté en notación postfija


## Pruebas de expresiones inusuales

    > esp = cargar 45.67 102.34 +
    > imprimir esp
    La expresión es: 45.67+102.34
    > evaluar esp
    La expresión evalúa a 147


    > esp1 = cargar 9,0 3,3 -- -- -
    > imprimir esp1
    La expresión es: (9,0---(--3,3))
    > evaluar esp1
    La expresión evalúa a 6


    > esp2 = cargar 78 90 + 25 2 ^ * 0 /
    > imprimir esp2
    La expresión es: (78+90)*25^2/0
    > evaluar esp2
    No se puede calcular una división por cero.


    > esp3 = cargar 95 78 * 0 % 89 2 * 3 ^ +
    > imprimir esp3
    La expresión es: 95*78%0+(89*2)^3
    > evaluar esp3
    No se puede calcular una división por cero


    > esp4 = cargar 324
    > imprimir esp4
    La expresión es: 324
    > evaluar esp4
    La expresión evalúa a 324


    > esp5 = cargar hola todo + que - bien *
    > imprimir esp5
    La expresión es: (hola+todo-que)*bien
    > evaluar esp5
    La expresión evalúa a 0


## Pruebas de manejo de errores en comandos 

    > nombre = cargr 90 8 *
    Hubo un error al procesar lo que ingresó. Pruebe nuevamente.


    > calcular nombe
    Hubo un error al procesar lo que ingresó. Pruebe nuevamente.


    > nombre = cargar 56 78 -
    > imprimir nombre1
    No existe expresión para imprimir con ese alias
    > evaluar nombre1
    No existe expresión para resolver con ese alias

