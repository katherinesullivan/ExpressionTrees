Lucas Abruzzo: Buen día, dentro de los temas vengo arrastrando 2 dudas que me interesaría entender:
-las instrucciones #ifndef _SLIST_ (por ejemplo) se que son instrucciones para la compilación, pero por que es necesario ponerlo en el archivo .h que sucedería en el caso de no hacerlo.
-por que se definen las funciónes de copiado, impresión etc que son auxiliares en el tema de void pointer, en el main como el static, no se pueden definir en el archivo .h? Muchas gracias

Valentina Bini: Hola Lucas!

La instrucción #ifndef sirve para evitar que cuando compilemos nuestro programa haya cosas definidas más de una vez. Esto es porque al generar programas muy grandes, puede que incluyamos cierta librería más de una vez, en distintos archivos. Lo que hace el compilador al incluir una librería es "pegar" el código de la misma ahí. Entonces si se incluyó la misma librería en varios archivos puede que al compilar se defina lo mismo más de una vez, porque se usa en varios lugares. El #ifndef __SLIST_H__ hace que no se entre al código si __SLIST_H__ ya está definido y así evita repeticiones. Es por eso también que después del #ifndef ponemos el #define para que ya quede definido __SLIST_H__ (en este ejemplo) y luego no vuelva a entrar ahí.

La idea es que nuestra librería (el .h) sea genérica y pueda usarse con cualquier tipo de dato que querramos almacenar en el void*, es por eso que usamos funciones auxiliares en un principio. Si definiéramos las funciones auxiliares dentro del .h estaríamos especificando el tipo de dato que vamos a almacenar dentro la librería misma. Es por esto que siempre las definimos desde donde vayamos a llamar a las funciones de nuestra librería, ya que ahí es donde sabemos realmente que tipo vamos a guardar.