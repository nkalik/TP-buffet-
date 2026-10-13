GRUPO N°5


INTEGRANTES: 

Nicolas Kalik Paredes-nkalik

joaquin gonzalez - joaquco

Paola Polo - paola555polo

Santiago Girona - sgirona-utn

En un principio eramos 5 integrantes, por lo cual quedamos en que el programa de normalizacion lo hacian joaquin y nico, ventas lo hacia paola, cierre lo hacia santiago, y resumen lo hacia el 5 integrante. Debido a que se bajo cuando ya teniamos normalizacion, ventas y cierre terminados, nico se tuvo que poner la 10 y hizo resumen solo.

COMO CORRER LOS PROGRAMAS
aca explican

NORMALIZACION

Para hacer normalizacion, hicimos llamadas por discord y compartiamos pantalla, asi ambos pensabamos en formas de resolver y programar el sistema que queria alberto, ademas de ver los errores del codigo. Usamos visual studio code para ir trabajando, y recien en el momento que teniamos resuelta una parte del problema lo subiamos al github. Yo (joaquin) no tengo muchos commits subidos debido a la forma en que decidimos trabajar.

En el primer punto alberto pide tener una lista de sus mozos prolija, para resolverlo creamos un vector donde guardar los mozos con los datos que eran necesarios, y usamos un while que lee el archivo de comandas historicas, e identifica si el mozo que realizo esa venta ya esta registrado adentro del vector de mozos, utilizando una busqueda secuencial, si no esta registrado lo inserta automaticamente en la siguiente posicion libre del vector y luego suma la comision de la venta a la comision tota (que empieza en 0), si ya esta registrado le suma la comision de la venta a la comision total. Cuando ya no quedan mas datos para agregar al vector, subimos los datos del vector al archivo nuevo, mozos.dat

  En el segundo punto pide ver las ventas dia por dia, que para ello hace falta crear una planilla con las ventas del dia, y luego que este ordenada por mozo. Para crear la planilla hicimos un vector que separa las ventas por dia, y para ello guarda la fecha, los datos de cada venta y cuenta cuantas ventas tuvo ese dia. Los datos se guardan aprovechando el while que usamos para el primer punto, que utiliza los datos de comandas historicas. Se ordena por mozo usando el metodo de seleccion, y se suben los datos a los archivos nuevos, que se llaman  segun la fecha, por ejemplo: "comandas_15-10-2026.dat"

  En el tercer y ultimo punto de normalizacion, pide que se actualize el stock al procesar ventas viejas. Para eso usamos un vector llamado productos, donde se guardan los datos del inventario, y se actualiza el stock con una busqueda secuencial, para luego guardar el inventario actualizado en inventario.dat

  Para que alberto pueda ver por pantalla el inventario actualizado y las ventas por dia hicimos las funciones mostrar_inventario y mostrar_archivos, ademas tambien creamos una contraseña generica para poder probar el programa, que es: 123456 , y una funcion de encriptado para las claves, que se usa en ventas.

  VENTAS

  CIERRE

  RESUMEN

  
