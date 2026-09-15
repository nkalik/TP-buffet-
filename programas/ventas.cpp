#include <iostream>
#include <cstdio>  // FILE, fopen, fread, fwrite, fseek, fclose, sprintf
#include <cstring> // strcpy, strcmp

using namespace std;

// Constantes
const float tasa_comision = 0.10f; // 10% de comision
const int CLAVE_K = 5;             // Corrimiento para la clave


struct producto
{
    int codigo;
    char descripcion[50];
    float precio;
    int stockactual;
};

struct mozo
{
    int idmozo;
    char nombre[50];
    char password[20];
    float totalcomision;
};

struct comanda
{
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};

// ====================
// PROTOTIPOS
// ====================

void encriptarClave(const char *entrada, char *salida);

// SOLO PARA PRUEBAS LOCALES
// ELIMINAR CUANDO NORMALIZACION.CPP GENERE mozos.dat
void crearMozosDePrueba();

bool validarLogin(int idBuscado, const char *claveIngresada);

bool buscarProducto(int codigoBuscado, producto &p, long &pos);

void actualizarStock(long pos, producto p, int cantidad);

void actualizarComisionMozo(int idMozo, float comision);

void ordenarArchivo(const char nombreArchivo[]);

bool validarFecha(const char fecha[]);

void mostrarEncabezado();

bool pedirFecha(char fecha[], char nombreArchivo[]);

bool pedirLogin(int &idMozo);

bool pedirDatosVenta(int &codigoProducto, int &cantidad);

bool verificarProducto(
    int codigoProducto,
    producto &prod,
    long &posicion,
    int cantidad);

bool guardarVenta(
    const char nombreArchivo[],
    int idMozo,
    int codigoProducto,
    int cantidad,
    producto prod,
    long posicion);


// ====================
// FUNCIONES
// ====================

// Encripta la clave sumando K a cada caracter
void encriptarClave(const char *entrada, char *salida)
{
    int i = 0;
    while (entrada[i] != '\0')
    {
        salida[i] = entrada[i] + CLAVE_K;
        i++;
    }
    salida[i] = '\0';
}

// FUNCION AUXILIAR: Crea mozos.dat de prueba localmente
void crearMozosDePrueba()
{
    FILE *f = fopen("mozos.dat", "wb");
    if (f == NULL)
        return;

    mozo m1, m2;
    m1.idmozo = 1;
    strcpy(m1.nombre, "Juan Perez");
    encriptarClave("sol", m1.password); // "xtq"
    m1.totalcomision = 0.0f;

    m2.idmozo = 2;
    strcpy(m2.nombre, "Maria Gomez");
    encriptarClave("1234", m2.password); // "6789"
    m2.totalcomision = 0.0f;

    fwrite(&m1, sizeof(mozo), 1, f);
    fwrite(&m2, sizeof(mozo), 1, f);
    fclose(f);
}

// Validacion de login contra mozos.dat
bool validarLogin(int idBuscado, const char *claveIngresada)
{
    FILE *f = fopen("mozos.dat", "rb");

    if (f == NULL)
    {
        cout << "Error: No se encuentra mozos.dat" << endl;
        return false;
    }

    char claveEncriptada[20];
    encriptarClave(claveIngresada, claveEncriptada);

    mozo m;

    while (fread(&m, sizeof(mozo), 1, f) == 1)
    {
        if (m.idmozo == idBuscado)
        {
            fclose(f);

            if (strcmp(m.password, claveEncriptada) == 0)
            {
                cout << "Bienvenido/a " << m.nombre << endl;
                return true;
            }

            cout << "Clave incorrecta." << endl;
            return false;
        }
    }

    fclose(f);

   cout << "Error: Mozo inexistente." << endl;
    return false;
}


// Buscar producto en inventario.dat en la raiz 
bool buscarProducto(int codigoBuscado, producto &p, long &pos)
{
    FILE *arch =fopen("inventario.dat", "rb");

  if (arch == NULL)
    {
        cout << "Error: No se encuentra inventario.dat en la raiz." << endl;
        return false;
    }

    pos = 0;

    while (fread(&p, sizeof(producto), 1, arch) == 1)
    {
        if (p.codigo == codigoBuscado)
        {
            fclose(arch);
            return true;
        }

        pos++;
    }

    fclose(arch);
    return false;
}

// Actualizar stock en inventario.dat
void actualizarStock(long pos, producto p, int cantidad)
{
    FILE *arch = fopen("inventario.dat", "rb+");

   if (arch == NULL)
    {
        cout << "Error al abrir inventario.dat para actualizar stock." << endl;
        return;
    }

    p.stockactual -= cantidad;

    fseek(arch, pos * sizeof(producto), SEEK_SET);

    fwrite(&p, sizeof(producto), 1, arch);

    fclose(arch);
}


//Actualiza la comisión acumulada del mozo en mozos.dat
void actualizarComisionMozo(int idMozo, float comision)
{
    FILE* arch = fopen("mozos.dat", "rb+");

   if (arch == NULL)
    {
        cout << "Error al abrir mozos.dat para actualizar comision." << endl;
        return;
    }

    mozo m;
//long posicionRegistro = 0;  //para q es?
    while(fread(&m, sizeof(mozo), 1, arch) == 1)
    {
        if(m.idmozo == idMozo)
        {
            m.totalcomision += comision;

            fseek(arch, -((long)sizeof(mozo)), SEEK_CUR);

            fwrite(&m, sizeof(mozo), 1, arch);

            break;
        }
        //posicionRegistro++;// investigar esto
    }

    fclose(arch);
}


// Ordenar archivo de comandas - Método burbuja
// Este ordena comandas_dd-mm-aaaa.dat por idmozo-
/* Al finalizar la carga se ordena el archivo del día por idmozo
   utilizando ordenamiento burbuja sobre archivo binario
   mediante fseek, fread y fwrite, ya que la cantidad de
   ventas diarias es reducida y la implementación resulta sencilla.
*/
void ordenarArchivo(const char nombreArchivo[])
{
    FILE *arch = fopen(nombreArchivo, "rb+");

    if (arch == NULL)
        return;

    fseek(arch, 0, SEEK_END);
//long tamBytes = ftell(arch);
// int cantidadRegistros = tamBytes / sizeof(comanda);

    int cantidadRegistros = ftell(arch) / sizeof(comanda);

    comanda c1;
    comanda c2;

    for (int i = 0; i < cantidadRegistros - 1; i++)
    {
        for (int j = 0; j < cantidadRegistros - 1 - i; j++)
        {
            fseek(arch, j * sizeof(comanda), SEEK_SET);

            fread(&c1, sizeof(comanda), 1, arch);
            fread(&c2, sizeof(comanda), 1, arch);

            if (c1.idmozo > c2.idmozo)
            {
                fseek(arch, j * sizeof(comanda), SEEK_SET);

                fwrite(&c2, sizeof(comanda), 1, arch);
                fwrite(&c1, sizeof(comanda), 1, arch);
            }
        }
    }

    fclose(arch);
}

//
void mostrarEncabezado()
{
    cout << "==========================================" << endl;
    cout << "   BUFFET ALBERT EINSTEIN - CARGA VENTAS  " << endl;
    cout << "==========================================" << endl;
}

//Función pedirFecha
bool pedirFecha(char fecha[], char nombreArchivo[])
{
    cout << "Ingrese fecha (DD-MM-AAAA): ";
    cin >> fecha;

    if(!validarFecha(fecha))
    {
        cout << "Error: formato de fecha invalido." << endl;
        return false;
    }

    sprintf(nombreArchivo, "comandas_%s.dat", fecha);

    return true;
}

//Validación de fecha
/*Ejemplos que aceptará
03-06-2025
15-11-2026
01-01-2030

Ejemplos que rechazará
3-6-2025
99-99-9999
hola
abc
12/06/2025*/
bool validarFecha(const char fecha[])
{
    if(strlen(fecha) != 10)
        return false;

    if(fecha[2] != '-' || fecha[5] != '-')
        return false;

    for(int i = 0; i < 10; i++)
    {
        if(i == 2 || i == 5)
            continue;

        if(fecha[i] < '0' || fecha[i] > '9')
            return false;
    }

    int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
    int anio = (fecha[6] - '0') * 1000 +
               (fecha[7] - '0') * 100 +
               (fecha[8] - '0') * 10 +
               (fecha[9] - '0');

    if(dia < 1 || dia > 31)
        return false;

    if(mes < 1 || mes > 12)
        return false;

    if(anio < 2000)
        return false;

    return true;
}

//Función pedirLogin
bool pedirLogin(int &idMozo)
{
    char clave[20];

    cout << "\nID del mozo: ";
    cin >> idMozo;

    cout << "Clave: ";
    cin >> clave;

    return validarLogin(idMozo, clave);
}
//pedir datos de venta:
bool pedirDatosVenta(int &codigoProducto, int &cantidad)
{
    cout << "Codigo de producto: ";
    cin >> codigoProducto;

    cout << "Cantidad: ";
    cin >> cantidad;

    if(cantidad <= 0)
    {
        cout << "Error: La cantidad debe ser mayor a cero." << endl;
        return false;
    }

    return true;
}

//Función verificarProducto
bool verificarProducto(
    int codigoProducto,
    producto &prod,
    long &posicion,
    int cantidad)
{
    if(!buscarProducto(codigoProducto, prod, posicion))
    {
        cout << "Producto inexistente." << endl;
        return false;
    }

  if (prod.stockactual < cantidad)
    {
        cout << "Error: Stock insuficiente. (Quedan " << prod.stockactual << " unidades)." << endl;
        return false;
    }

    return true;
}
// Función guardarVenta
bool guardarVenta(
    const char nombreArchivo[],
    int idMozo,
    int codigoProducto,
    int cantidad,
    producto prod,
    long posicion)
{
    comanda nuevaVenta;

    nuevaVenta.idmozo = idMozo;
    nuevaVenta.codigoproducto = codigoProducto;
    nuevaVenta.cantidad = cantidad;

    float importeVenta = prod.precio * cantidad;

    nuevaVenta.comision = importeVenta * tasa_comision;

    FILE *archivoComandas = fopen(nombreArchivo, "ab");

    if(archivoComandas == NULL)
    {
        cout << "Error al abrir archivo del dia." << endl;
        return false;
    }

    fwrite(&nuevaVenta, sizeof(comanda), 1, archivoComandas);

    fclose(archivoComandas);

    actualizarStock(posicion, prod, cantidad);

    actualizarComisionMozo(idMozo, nuevaVenta.comision);

cout << "-> Venta registrada correctamente. (Comision generada: $" << nuevaVenta.comision << ")" << endl;
    return true;
}

int main()
{
// Verificacion de integridad binaria exigida por la catedra
    if (sizeof(producto) != 64)
    {
        cout << "ERROR CRITICO: sizeof(producto) es " << sizeof(producto) << " (debe ser 64)." << endl;
        return 1;
    }

    // SOLO para pruebas locales.
    // Se puede comentar o borrar cuando normalizacion.cpp genere el mozos.dat definitivo.
    crearMozosDePrueba();

    mostrarEncabezado();

    char fecha[11];
    char nombreArchivo[40];

 // Pedimos la fecha una sola vez
    while (!pedirFecha(fecha, nombreArchivo))
    {
        cout << "Intente nuevamente.\n" << endl;
    }

    int continuar = 1;

    while (continuar == 1)
    {
        int idMozo;

        if (!pedirLogin(idMozo))
            continue;

        int codigoProducto;
        int cantidad;

        if (!pedirDatosVenta(codigoProducto, cantidad))
            continue;

        producto prod;
        long posicion;

        if (!verificarProducto(codigoProducto, prod, posicion, cantidad))
            continue;

        if (!guardarVenta(nombreArchivo, idMozo, codigoProducto, cantidad, prod, posicion))
        {
            continue;
        }

        cout << "\n¿Desea cargar otra venta? (1=SI / 0=NO): ";
        cin >> continuar;
    }

    // Ordenar la planilla del día antes de finalizar
    ordenarArchivo(nombreArchivo);

    cout << "\nArchivo ordenado por id de mozo." << endl;
//cout << "\nPlanilla '" << nombreArchivo << "' ordenada exitosamente por ID de mozo." << endl;
    return 0;
}

