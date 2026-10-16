GRUPO N°5


INTEGRANTES: 

Nicolas Kalik Paredes-nkalik

joaquin gonzalez - joaquco

Paola Polo - paola555polo

Santiago Girona - sgirona-utn

En un principio eramos 5 integrantes, por lo cual quedamos en que el programa de normalizacion lo hacian joaquin y nico, ventas lo hacia paola, cierre lo hacia santiago, y resumen lo hacia el 5 integrante. Debido a que se bajo cuando ya teniamos normalizacion, ventas y cierre terminados, nico se tuvo que poner la 10 y hizo resumen solo.

COMO CORRER LOS PROGRAMAS:

Compilado con UCRT64 (MSYS2), desde la terminal de Visual Studio Code:

Orden de ejecución (cada programa usa los archivos que deja el anterior):

  g++ -o normalizacion normalizacion.cpp
  g++ -o ventas ventas.cpp
  g++ -o cierre cierre.cpp
  g++ -o resumen resumen.cpp

Disposición de los archivos:

Todo en la misma carpeta:

    TP_Buffet/
        normalizacion.cpp   ventas.cpp   cierre.cpp   resumen.cpp
        comandas_historicas.dat     <- lo da la cátedra
        inventario.dat              <- lo da la cátedra (se va actualizando)

        -- se generan al ejecutar --
        mozos.dat
        comandas_dd-mm-aaaa.dat     (uno por cada día)
        comandas_semana_sX-mm.dat   (uno por cada semana)

<!---------------------------------------------------------->
NORMALIZACION
 <!---------------------------------------------------------->
Para hacer normalizacion, hicimos llamadas por discord y compartiamos pantalla, asi ambos pensabamos en formas de resolver y programar el sistema que queria alberto, ademas de ver los errores del codigo. Usamos visual studio code para ir trabajando, y recien en el momento que teniamos resuelta una parte del problema lo subiamos al github. Yo (joaquin) no tengo muchos commits subidos debido a la forma en que decidimos trabajar.

En el primer punto alberto pide tener una lista de sus mozos prolija, para resolverlo creamos un vector donde guardar los mozos con los datos que eran necesarios, y usamos un while que lee el archivo de comandas historicas, e identifica si el mozo que realizo esa venta ya esta registrado adentro del vector de mozos, utilizando una busqueda secuencial, si no esta registrado lo inserta automaticamente en la siguiente posicion libre del vector y luego suma la comision de la venta a la comision tota (que empieza en 0), si ya esta registrado le suma la comision de la venta a la comision total. Cuando ya no quedan mas datos para agregar al vector, subimos los datos del vector al archivo nuevo, mozos.dat

  En el segundo punto pide ver las ventas dia por dia, que para ello hace falta crear una planilla con las ventas del dia, y luego que este ordenada por mozo. Para crear la planilla hicimos un vector que separa las ventas por dia, y para ello guarda la fecha, los datos de cada venta y cuenta cuantas ventas tuvo ese dia. Los datos se guardan aprovechando el while que usamos para el primer punto, que utiliza los datos de comandas historicas. Se ordena por mozo usando el metodo de seleccion, y se suben los datos a los archivos nuevos, que se llaman  segun la fecha, por ejemplo: "comandas_15-10-2026.dat"

  En el tercer y ultimo punto de normalizacion, pide que se actualize el stock al procesar ventas viejas. Para eso usamos un vector llamado productos, donde se guardan los datos del inventario, y se actualiza el stock con una busqueda secuencial, para luego guardar el inventario actualizado en inventario.dat

  Para que alberto pueda ver por pantalla el inventario actualizado y las ventas por dia hicimos las funciones mostrar_inventario y mostrar_archivos, ademas tambien creamos una contraseña generica para poder probar el programa, que es: 123456 , y una funcion de encriptado para las claves, que se usa en ventas.
<!---------------------------------------------------------->
  VENTAS
<!---------------------------------------------------------->
El flujo principal del programa se divide en las siguientes etapas:Ingreso y Validación de Fecha:

-Al iniciar, el programa muestra un encabezado descriptivo y solicita por pantalla la fecha de la jornada (formato DD-MM-AAAA) una única vez. Se implementó una función robusta validarFecha que verifica la longitud exacta, la posición de los guiones y valida los rangos lógicos para días (1-31), meses (1-12) y años (>= 2000). Con esta fecha validada, se construye de manera dinámica el nombre del archivo diario, por ejemplo, comandas_15-10-2026.dat.

-Login y Seguridad de Mozos:
El sistema permite el ingreso interactivo del mozo solicitando su ID y su contraseña. Para validar la identidad, el programa abre el archivo mozos.dat generado previamente por el módulo de normalización. Antes de comparar la clave ingresada, se aplica la función encriptarClave sumando el corrimiento constante K=5 a cada carácter (respetando la misma lógica simétrica utilizada por el grupo en la normalización). Si el ID existe y la clave encriptada coincide, el sistema da la bienvenida; caso contrario, rechaza el acceso.

-Carga, Validación y Descuento de Stock:
Una vez superado el login, se solicitan el código del producto y la cantidad vendida. Mediante una búsqueda secuencial sobre el archivo binario inventario.dat (ubicado en la raíz), el programa localiza el producto, captura su posición exacta (long pos) y valida dos condiciones críticas:
 Que el producto exista en el inventario.
 Que el stock actual sea mayor o igual a la cantidad solicitada.
Si hay stock suficiente, se procede a descontar las unidades de manera directa sobre el archivo abierto en modo de actualización (rb+), utilizando posicionamiento exacto con fseek y reescribiendo el registro con fwrite.

-Cálculo de Comisión y Registro de la Venta:
Se calcula el importe de la venta multiplicando el precio unitario por la cantidad y se le aplica la tasa institucional del 10% (tasa_comision = 0.10f) para obtener la comisión correspondiente. Esta nueva comanda se agrega al final del archivo diario (comandas_dd-mm-aaaa.dat) usando el modo de apertura ab. Asimismo, se actualiza de forma acumulativa la comisión total del mozo directamente en el archivo mozos.dat.

-Cierre y Ordenamiento:
El programa permite al usuario registrar múltiples ventas consecutivas mediante un bucle de control. Al finalizar la carga de la jornada, se invoca a la función ordenarArchivo que aplica un método de ordenamiento burbuja directamente sobre los registros binarios del archivo diario, dejándolo perfectamente ordenado por idmozo para que el siguiente módulo (cierre.cpp) pueda procesarlo sin inconvenientes.
<!---------------------------------------------------------->
  CIERRE
<!---------------------------------------------------------->

Este programa lo hice yo con la ayuda de mis compañeros. El enunciado pide juntar todas las planillas de los días de la semana en una sola planilla semanal, y que quede ordenada por mozo, sin tener que reordenar todo de nuevo, ya que cada planilla de día sale ordenada por mozo gracias a `ventas.cpp`.

Primero pregunta cuántos días tiene la semana a cerrar, y pide la fecha de cada uno, usando la misma validación de formato `DD-MM-AAAA` que usa `ventas.cpp`, para que las dos pantallas se comporten igual. Con cada fecha arma el nombre del archivo de ese día (`comandas_dd-mm-aaaa.dat`). Después pide el número de semana y el mes, y con eso arma el nombre del archivo de salida: `comandas_semana_sX-mm.dat`. Por ejemplo, semana 1 y mes 6 arma `comandas_semana_s1-06.dat`.

Para juntar los archivos sin reordenar, abre todos los archivos de los días al mismo tiempo, y guarda en dos vectores el registro actual de cada uno (el primer dato que cada archivo tiene esperando) y si ese archivo todavía tiene datos para dar. En cada vuelta, se busca cuál de los archivos abiertos tiene el mozo con el número más chico, se graba ese registro en la planilla semanal, y se avanza solamente ese archivo al siguiente registro. Esto se repite hasta que ningún archivo tiene más registros para dar. Como cada archivo de día ya viene ordenado por mozo, siempre se encuentra el mozo más chico que queda en total, así que la planilla semanal sale ordenada sin reordenar nada a mano: solo se combina lo que ya estaba ordenado.

Si algún día de los que se pidió todavía no tiene planilla generada (por ejemplo, porque ese día no se cargó ninguna venta), se avisa por pantalla que ese archivo no existe y se sigue el cierre con los demás días, en vez de cortar el programa entero. Al final, para verificar que el cierre salió bien, se recorre la planilla semanal recién creada y se muestra por pantalla.

  RESUMEN
<!---------------------------------------------------------->
  Es el ultimo programa en ejecutarse. Lee la planilla semanal y muestra por pantalla, para cada mozo, cuántas ventas hizo, cuántas unidades vendió y cuánta         comisión hay que pagarle. Abajo de todo muestra el total de unidades que vendió el buffet en la semana.
  Necesita que cierre.cpp ya haya generado comandas_semana_sX-mm.dat en la misma carpeta.

  El programa pide: La semana (de 1 a 5) y el mes (de 1 a 12). Con eso arma el nombre del archivo. 
  Por ejemplo, semana 1 y mes 6 abre comandas_semana_s1-06.dat.
  Lee solo comandas_semana_sX-mm.dat y lo recorre con corte de control. El archivo semanal viene ordenado por mozo, así que todas las ventas de un mismo mozo        están seguidas. Por cada registro leído:
  -Si el mozo es distinto al del último resumen (o es el primer registro), se crea un resumen nuevo en el vector resumen[] con todo en cero.
  -Siempre se acumulan en el resumen actual las ventas, las unidades (cantidad) y la comisión.
  -Se suma también al total de unidades del buffet.

  No hace falta buscar al mozo en el vector, porque solo se compara con el último resumen. Por eso alcanza con recorrer el archivo una vez.
  "Productos vendidos" se cuenta como unidades, o sea la suma de cantidad de cada venta. Además se muestra cuántas ventas hizo cada mozo.

  El último mozo no necesita de una resolución especial. Como se acumula en cada registro, su resumen ya está completo cuando termina el archivo.      
  
