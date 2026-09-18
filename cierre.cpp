// primer piloto
#include <iostream>
#include <cstdio>   // FILE, fopen, fread, fwrite, fclose, sprintf
#include <cstring>  // strlen

using namespace std;

// CONSTANTES
const int MAX_DIAS = 7; // una semana tiene como maximo 7 planillas diarias

// STRUCT comanda

struct comanda
{
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};


bool validarFecha(const char fecha[]);
bool pedirFecha(char fecha[]);
void mostrarResultado(const char nombreArchivo[]);

// FUNCIONES

// Validacion de fecha DD-MM-AAAA

bool validarFecha(const char fecha[]) // (misma logica que ventas.cpp, para que ambas pantallas se comporten igual)
{
    if (strlen(fecha) != 10)
        return false;

    if (fecha[2] != '-' || fecha[5] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;

        if (fecha[i] < '0' || fecha[i] > '9')
            return false;
    }

    int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
    int anio = (fecha[6] - '0') * 1000 +
               (fecha[7] - '0') * 100 +
               (fecha[8] - '0') * 10 +
               (fecha[9] - '0');

    if (dia < 1 || dia > 31)
        return false;

    if (mes < 1 || mes > 12)
        return false;

    if (anio < 2000)
        return false;

    return true;
}

// Pide una fecha por teclado y la valida
bool pedirFecha(char fecha[])
{
    cout << "Fecha (DD-MM-AAAA): ";
    cin >> fecha;

    if (!validarFecha(fecha))
    {
        cout << "Error: formato de fecha invalido." << endl;
        return false;
    }

    return true;
}

// Recorre y muestra por pantalla el archivo semanal ya generado
void mostrarResultado(const char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL)
        return;

    cout << "\n===== " << nombreArchivo << " =====" << endl;
    cout << "mozo   codigo   cantidad   comision" << endl;
    cout << "------------------------------------" << endl;

    comanda c;

    while (fread(&c, sizeof(comanda), 1, archivo) == 1)
    {
        cout << c.idmozo << "      "
             << c.codigoproducto << "       "
             << c.cantidad << "        "
             << c.comision
             << endl;
    }

    fclose(archivo);
}

// MAIN
int main()
{
    cout << "===== CIERRE SEMANAL =====" << endl;

    // 1) Cuantos dias componen la semana a cerrar
    int cantDias;

    do
    {
        cout << "Cuantos dias tiene la semana a cerrar (1 a " << MAX_DIAS << "): ";
        cin >> cantDias;

        if (cantDias < 1 || cantDias > MAX_DIAS)
        {
            cout << "Error: ingrese un valor entre 1 y " << MAX_DIAS << "." << endl;
        }

    } while (cantDias < 1 || cantDias > MAX_DIAS);

    // 2) Pedir la fecha de cada dia y armar el nombre de archivo de cada uno
    char nombresArchivos[MAX_DIAS][40];

    for (int i = 0; i < cantDias; i++)
    {
        char fecha[11];

        cout << "\n--- Dia " << (i + 1) << " de la semana ---" << endl;

        while (!pedirFecha(fecha))
        {
            cout << "Intente nuevamente." << endl;
        }

        sprintf(nombresArchivos[i], "comandas_%s.dat", fecha);
    }

    // 3) Numero de semana y mes, para armar el nombre del archivo de salida
    int numSemana, mes;

    cout << "\nNumero de semana (ej: 1): ";
    cin >> numSemana;

    cout << "Mes de la semana (1-12): ";
    cin >> mes;

    char nombreSalida[40];
    sprintf(nombreSalida, "comandas_semana_s%d-%02d.dat", numSemana, mes);

    // 4) Abrir todos los archivos de dia (lectura anticipada del apareo)
    FILE *archivos[MAX_DIAS];
    comanda actual[MAX_DIAS];
    bool hayRegistro[MAX_DIAS];
    int archivosAbiertos = 0;

    for (int i = 0; i < cantDias; i++)
    {
        archivos[i] = fopen(nombresArchivos[i], "rb");

        if (archivos[i] == NULL)
        {
            cout << "\nAviso: no existe " << nombresArchivos[i]
                 << ", se omite del cierre." << endl;

            hayRegistro[i] = false;
        }
        else
        {
            archivosAbiertos++;

            // lectura anticipada: primer registro de cada archivo del dia
            hayRegistro[i] = (fread(&actual[i], sizeof(comanda), 1, archivos[i]) == 1);
        }
    }

    if (archivosAbiertos == 0)
    {
        cout << "\nNo se encontro ningun archivo de la semana. Cierre abortado." << endl;
        return 1;
    }

    // 5) Abrir el archivo semanal de salida
    FILE *salida = fopen(nombreSalida, "wb");

    if (salida == NULL)
    {
        cout << "Error al crear " << nombreSalida << endl;

        for (int i = 0; i < cantDias; i++)
        {
            if (archivos[i] != NULL)
                fclose(archivos[i]);
        }

        return 1;
    }

    // 6) APAREO generalizado a cantDias archivos: todos ya ordenados por
    //    idmozo (lo dejo asi ventas.cpp). En cada paso se busca, entre los
    //    archivos que todavia tienen registro, el de idmozo mas chico,
    //    se graba en la salida y se avanza SOLO ese archivo. Cuando ningun
    //    archivo tiene mas registros, se corta.
    int totalRegistros = 0;
    bool quedanRegistros = true;

    while (quedanRegistros)
    {
        int posMenor = -1;

        for (int i = 0; i < cantDias; i++)
        {
            if (hayRegistro[i])
            {
                if (posMenor == -1 || actual[i].idmozo < actual[posMenor].idmozo)
                {
                    posMenor = i;
                }
            }
        }

        if (posMenor == -1)
        {
            quedanRegistros = false; // ningun archivo tiene mas registros
        }
        else
        {
            fwrite(&actual[posMenor], sizeof(comanda), 1, salida);
            totalRegistros++;

            hayRegistro[posMenor] =
                (fread(&actual[posMenor], sizeof(comanda), 1, archivos[posMenor]) == 1);
        }
    }

    // 7) Cerrar todo
    for (int i = 0; i < cantDias; i++)
    {
        if (archivos[i] != NULL)
            fclose(archivos[i]);
    }

    fclose(salida);

    cout << "\n" << nombreSalida << " generado correctamente." << endl;
    cout << "Total de comandas de la semana: " << totalRegistros << endl;

    // 8) Mostrar el resultado para verificar el cierre
    mostrarResultado(nombreSalida);

    cout << "\n===== CIERRE TERMINADO =====" << endl;

    return 0;
}
