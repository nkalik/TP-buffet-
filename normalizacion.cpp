#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct comandahistorica {
    char fecha[11];
    char nombremozo[50];
    int codigoproducto;
    int cantidad;
    float comision;
};

struct producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockactual;
};

struct mozo {
    int idmozo;
    char nombre[50];
    char password[20];
    float totalcomision;
};

struct comanda {
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};

struct ventaspordia {
    char fecha[11];
    comanda ventas[100];
    int lenventas;
};


// BUSCAR UN MOZO POR NOMBRE
int buscar_mozo(mozo mozos[], int lenmozos, char nombre[]) {

    for(int i = 0; i < lenmozos; i++) {

        if(strcmp(mozos[i].nombre, nombre) == 0) {
            return i;
        }
    }

    return -1;
}

// ORDENAR LAS VENTAS POR ID DE MOZO
void ordenar_ventas(comanda ventas[], int lenventas) {

    for(int i = 0; i < lenventas - 1; i++) {

        int posmenor = i;

        for(int j = i + 1; j < lenventas; j++) {

            if(ventas[j].idmozo < ventas[posmenor].idmozo) {
                posmenor = j;
            }
        }

        if(posmenor != i) {

            comanda aux = ventas[i];
            ventas[i] = ventas[posmenor];
            ventas[posmenor] = aux;
        }
    }
}

// ORDENAR Y GUARDAR LAS VENTAS DE CADA DIA
void guardar_archivos(ventaspordia comandas[]) {

    for(int dia = 1; dia <= 31; dia++) {

        if(comandas[dia].lenventas > 0) {

            ordenar_ventas(comandas[dia].ventas, comandas[dia].lenventas);

            char nombrearchivo[50];

            sprintf(nombrearchivo, "comandas_%s.dat", comandas[dia].fecha);

            FILE *archivo = fopen(nombrearchivo, "wb");

            if(archivo != NULL) {

                fwrite(comandas[dia].ventas, sizeof(comanda), comandas[dia].lenventas, archivo);

                fclose(archivo);

                cout << "archivo creado: "<< nombrearchivo<< " - ventas: "<< comandas[dia].lenventas<< endl;
            }
        }
    }
}

void mostrar_inventario(producto productos[], int lenproductos){

    printf("%-10s%-20s%-10s%-10s\n", "codigo", "descripcion", "precio", "stock");
    printf("--------------------------------------------------------\n");

    for(int i = 0; i < lenproductos; i++){
        printf("%-10d%-20s%-10.2f%-10d\n",
               productos[i].codigo,
               productos[i].descripcion,
               productos[i].precio,
               productos[i].stockactual);
    }
}

void mostrar_archivos(ventaspordia comandas[]) {

    for(int dia = 1; dia <= 31; dia++) {

        if(comandas[dia].lenventas > 0) {

            char nombrearchivo[50];

            sprintf(nombrearchivo, "comandas_%s.dat", comandas[dia].fecha);

            FILE *archivo = fopen(nombrearchivo, "rb");

            if(archivo != NULL) {

                cout << "===== " << nombrearchivo << " =====" << endl;
                cout << "mozo   codigo   cantidad   comision" << endl;
                cout << "------------------------------------" << endl;

                comanda aux;

                while(fread(&aux, sizeof(comanda), 1, archivo) == 1) {

                    cout << aux.idmozo << "      "
                    << aux.codigoproducto << "       "
                    << aux.cantidad << "        "
                    << aux.comision
                    << endl;
                }

                fclose(archivo);
            }
        }
    }
}

int main() {

    cout << "===== INICIO DE NORMALIZACION =====" << endl;

   
    // LEER COMANDAS HISTORICAS
    FILE *archivoscomandas = fopen("comandas_historicas.dat", "rb");

    if(archivoscomandas == NULL) {
        cout << "error al abrir comandas_historicas.dat"<< endl;
        return 1;
    }

    cout << "comandas_historicas.dat abierto correctamente"<< endl;

    mozo mozos[100];
    int lenmozos = 0;

    ventaspordia comandas[32] = {};

    comandahistorica aux;

    while(fread(&aux, sizeof(comandahistorica), 1, archivoscomandas) == 1) {

        // BUSCAR EL MOZO
        int posmozo = buscar_mozo(mozos, lenmozos, aux.nombremozo);

        // SI NO EXISTE, AGREGAR MOZO
        if(posmozo == -1) {

            posmozo = lenmozos;

            mozos[posmozo].idmozo = lenmozos + 1;

            strcpy(mozos[posmozo].nombre, aux.nombremozo);

            mozos[posmozo].password[0] = '\0';

            mozos[posmozo].totalcomision = 0;

            lenmozos++;

            cout << "mozo: "<< mozos[posmozo].idmozo<< " - "<< mozos[posmozo].nombre<< endl;
        }

        // ACUMULAR LA COMISION
        mozos[posmozo].totalcomision += aux.comision;

        // OBTENER EL DIA
        int dia =(aux.fecha[0] - '0') * 10 +(aux.fecha[1] - '0');

        // GUARDAR LA FECHA
        if(comandas[dia].lenventas == 0) {
            strcpy(comandas[dia].fecha, aux.fecha);
        }

        // GUARDAR LA COMANDA 
        int posventa = comandas[dia].lenventas;

        comandas[dia].ventas[posventa].idmozo = mozos[posmozo].idmozo;
        comandas[dia].ventas[posventa].codigoproducto = aux.codigoproducto;
        comandas[dia].ventas[posventa].cantidad = aux.cantidad;
        comandas[dia].ventas[posventa].comision = aux.comision;

        comandas[dia].lenventas++;
    }

    fclose(archivoscomandas);

    cout << "mozos encontrados: "<< lenmozos<< endl;

    // GUARDAR MOZOS.DAT
    FILE *archivomozos = fopen("mozos.dat", "wb");

    if(archivomozos == NULL) {
        cout << "error al crear mozos.dat"<< endl;
        return 1;
    }

    fwrite(mozos, sizeof(mozo), lenmozos, archivomozos);

    fclose(archivomozos);

    cout << "mozos.dat guardado correctamente"<< endl;

    // ORDENAR Y GUARDAR ARCHIVOS DIARIOS
    guardar_archivos(comandas);
    mostrar_archivos(comandas);

    // CARGAR INVENTARIO
    producto productos[100];
    int lenproductos = 0;

    FILE *archivoinventario = fopen("inventario.dat", "rb");

    if(archivoinventario == NULL) {
        cout << "error al abrir inventario.dat"<< endl;
        return 1;
    }

    while(fread(&productos[lenproductos], sizeof(producto), 1, archivoinventario) == 1) {
        lenproductos++;
    }

    fclose(archivoinventario);

    cout << "inventario cargado. productos: "<< lenproductos<< endl;

    // ACTUALIZAR STOCK
    for(int dia = 1; dia <= 31; dia++) {

        for(int i = 0; i < comandas[dia].lenventas; i++) {

            int codigo = comandas[dia].ventas[i].codigoproducto;
            int cantidad = comandas[dia].ventas[i].cantidad;

            for(int j = 0; j < lenproductos; j++) {

                if(productos[j].codigo == codigo) {
                    productos[j].stockactual -= cantidad;
                    break;
                }
            }
        }
    }

    // GUARDAR INVENTARIO ACTUALIZADO
    archivoinventario = fopen("inventario.dat", "wb");

    if(archivoinventario == NULL) {
        cout << "error al guardar inventario.dat"<< endl;
        return 1;
    }

    fwrite(productos, sizeof(producto), lenproductos, archivoinventario);

    fclose(archivoinventario);

    cout << "inventario.dat actualizado correctamente"<< endl;

    mostrar_inventario(productos, lenproductos);

    cout << "===== NORMALIZACION TERMINADA ====="<< endl;

    return 0;
}
