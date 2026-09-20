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

// STRUCT QUE USAMOS PARA LAS PLANILLAS POR DIA
struct ventaspordia {
    char fecha[11]; // GUARDA LA FECHA
    comanda ventas[100]; // GUARDA LOS DATOS DE CADA VENTA, MAXIMO 100
int lenventas; // CUENTA CUANTAS VENTAS HUBO ESE DIA
};

const int CLAVE_K = 5; // NUMERO QUE USAMOS PARA EL ENCRIPTADO

const char password_generica[20] = "123456"; // CONTRASEÑA GENERICA

// FUNCION QUE SUMA LA CLAVE K A CADA CARACTER
void encriptarClave(const char *entrada, char *salida) {
    int i = 0;
    while (entrada[i] != '\0') {
        salida[i] = entrada[i] + CLAVE_K;
        i++;
    }
    salida[i] = '\0';
};

// FUNCION QUE BUSCA UN MOZO POR NOMBRE
int buscar_mozo(mozo mozos[], int lenmozos, char nombre[]) {

    for(int i = 0; i < lenmozos; i++) {

        if(strcmp(mozos[i].nombre, nombre) == 0) {
            return i;
        }
    }

    return -1;
}

// FUNCION QUE ORDENA LAS VENTAS POR ID DE MOZO   
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

// FUNCION QUE ORDENA Y GUARDA LAS VENTAS DE CADA DIA
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
// FUNCION QUE MUESTRA LA INFORMACION DEL INVENTARIO, DE FORMA LINDA
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
// FUNCION QUE MUESTRA LA INFORMACION DE LAS VENTAS DEL DIA
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
    
    // ABRE EL ARCHIVO
    FILE *archivoscomandas = fopen("datos/comandas_historicas.dat", "rb");
    
    // VERIFICA QUE SE ABRIO CORRECTAMENTE
    if(archivoscomandas == NULL) {
        cout << "error al abrir comandas_historicas.dat"<< endl;
        return 1;
    }
    
    cout << "comandas_historicas.dat abierto correctamente"<< endl;
    
    mozo mozos[100]; // VECTOR DE MOZOS, MAXIMO 100
    int lenmozos = 0;

    ventaspordia comandas[32] = {}; // VECTOR PARA PLANILLAS DE VENTAS POR DIA

    comandahistorica aux;

    while(fread(&aux, sizeof(comandahistorica), 1, archivoscomandas) == 1) {

        // BUSCA EL MOZO
        int posmozo = buscar_mozo(mozos, lenmozos, aux.nombremozo);

        // SI NO EXISTE, AGREGA EL MOZO
        if(posmozo == -1) {

            posmozo = lenmozos;

            mozos[posmozo].idmozo = lenmozos + 1;

            strcpy(mozos[posmozo].nombre, aux.nombremozo);

            encriptarClave(password_generica, mozos[posmozo].password);

            mozos[posmozo].totalcomision = 0;

            lenmozos++;
        }

        // ACUMULA LA COMISION
        mozos[posmozo].totalcomision += aux.comision;

        // OBTIENE EL DIA
        int dia =(aux.fecha[0] - '0') * 10 +(aux.fecha[1] - '0');

        // GUARDA LA FECHA
        if(comandas[dia].lenventas == 0) {
            strcpy(comandas[dia].fecha, aux.fecha);
        }

        // GUARDA LA COMANDA EN LA PLANILLA DEL DIA
        int posventa = comandas[dia].lenventas;

        comandas[dia].ventas[posventa].idmozo = mozos[posmozo].idmozo;
        comandas[dia].ventas[posventa].codigoproducto = aux.codigoproducto;
        comandas[dia].ventas[posventa].cantidad = aux.cantidad;
        comandas[dia].ventas[posventa].comision = aux.comision;

        comandas[dia].lenventas++;
    }

    fclose(archivoscomandas);

    cout << "mozos encontrados: "<< lenmozos<< endl;

    // GUARDA MOZOS.DAT
    FILE *archivomozos = fopen("mozos.dat", "wb");

    if(archivomozos == NULL) {
        cout << "error al crear mozos.dat"<< endl;
        return 1;
    }

    fwrite(mozos, sizeof(mozo), lenmozos, archivomozos);

    fclose(archivomozos);

    cout << "mozos.dat guardado correctamente"<< endl;

    // registro auxiliar para leer mozos.dat
    mozo auxmozos;          

    archivomozos = fopen("mozos.dat", "rb");

    if(archivomozos == NULL){
        cout<< "error al leer mozos.dat"<< endl;
        return 1;
    }

    while(fread(&auxmozos, sizeof(mozo), 1, archivomozos) == 1){
        cout << auxmozos.idmozo << " - "
        << auxmozos.nombre << " - " 
        << auxmozos.totalcomision << endl;
    }

    fclose(archivomozos);

    // ORDENA Y GUARDA LAS PLANILLAS DEL DIA
    guardar_archivos(comandas);
    mostrar_archivos(comandas);

    // CARGA INVENTARIO
    producto productos[100];
    int lenproductos = 0;

    FILE *archivoinventario = fopen("datos/inventario.dat", "rb");

    if(archivoinventario == NULL) {
        cout << "error al abrir inventario.dat"<< endl;
        return 1;
    }

    while(fread(&productos[lenproductos], sizeof(producto), 1, archivoinventario) == 1) {
        lenproductos++;
    }

    fclose(archivoinventario);

    cout << "inventario cargado. productos: "<< lenproductos<< endl;

    // ACTUALIZA STOCK
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

    // GUARDA EL INVENTARIO ACTUALIZADO
    archivoinventario = fopen("datos/inventario.dat", "wb");

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
