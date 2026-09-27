#include <iostream>
#include <cstdio>  // FILE, fopen, fread, fclose, sprintf
#include <cstring> // strcpy
using namespace std;
struct mozo{
    int idmozo;
    char nombre[50];
    char password[20];
    float totalcomision;
};
struct comanda{
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};

// Estructura que se va a usar para el resumen semanal de cada mozo
struct resumensemanal{
    int idmozo;
    char nombre[50];
    int lenventas;
    int unidadesvendidas;
    float totalcomision;
};

bool pedirarchivo(char nombrearchivo[]){
    int semana, mes;
    
    cout << "\nIngrese la semana (ej 1): ";
    cin >> semana;

    cout <<"Ingrese el mes (1-12): ";
    cin >> mes;

    sprintf(nombrearchivo, "comandas_semana_s%d-%02d.dat", semana, mes);

    FILE *archivosemanal = fopen(nombrearchivo, "rb");
    if(archivosemanal == NULL){
        cout << "ERROR: No se encuentra " << nombrearchivo << endl;
        return false;
    }

    fclose(archivosemanal);
    return true;
}

// Busca el mozo por id dentro del array que se cargo
int buscarmozo(mozo mozos[], int lenmozos, int idmozo){
    int i = 0;

    // se ejecuta hasta que coincidan los id
    while (mozos[i].idmozo != idmozo){
        i++;
    }
    return i;
}

int main(){
    cout << "          RESUMEN DE CIERRE          " << endl;

    mozo mozos[100];
    int lenmozos = 0;

    resumensemanal resumen[100];
    int lenresumen = 0;

    int totalvendido = 0;

    FILE *archivomozos = fopen("mozos.dat", "rb");
    if(archivomozos == NULL){
        cout << "ERROR: No se encuentra mozos.dat" << endl;
        return 1;
    }

    // Se cargan los mozos en memoria
    while(fread(&mozos[lenmozos], sizeof(mozo), 1, archivomozos) == 1){
        lenmozos++;
    }

    fclose(archivomozos);

    char nombrearchivo[50];

    // Llama a la funcion y en caso que devuelva false la ejecuta hasta devolver true
    while(!pedirarchivo(nombrearchivo)){
        cout << "Intente otra vez.\n" << endl;
    }

    FILE *archivosemanal = fopen(nombrearchivo, "rb");
    if(archivosemanal == NULL){
        cout <<"Error al abrir " << nombrearchivo << endl;
        return 1;
    }

    comanda aux;

    while(fread(&aux, sizeof(comanda), 1, archivosemanal) == 1){
        // entero auxiliar, se usa en el if solo para el primer registro leido
        int pos = lenresumen - 1; 

        // acumula los registros con mismo mozo, y crea un nuevo registro resumen cuando el mozo es distinto al ultimo registro leido
        if(pos == -1 || resumen[pos].idmozo != aux.idmozo){
            // aca pos vale 0, se crea el primer resumen
            pos = lenresumen;

            resumen[pos].idmozo = aux.idmozo;

            // la posicion donde se encuentra el idmozo
            int posmozo = buscarmozo(mozos, lenmozos, aux.idmozo);

            // una vez encontrada la posicion accedemos al nombre para copiarlo en resumen
            strcpy(resumen[pos].nombre, mozos[posmozo].nombre);

            // inicializamos para que empiecen a contar desde 0
            resumen[pos].lenventas = 0;
            resumen[pos].unidadesvendidas = 0;
            resumen[pos].totalcomision = 0;

            lenresumen++;
        }

        resumen[pos].lenventas++;
        resumen[pos].unidadesvendidas += aux.cantidad;
        resumen[pos].totalcomision += aux.comision;

        totalvendido += aux.cantidad;
    }

    fclose(archivosemanal);

    cout << endl;
    printf("%-8s%-20s%-12s%-14s%-10s\n", "ID", "Nombre", "Ventas", "Unidades", "Comision");
    cout << endl;

    for (int i = 0; i < lenresumen; i++){
        printf("%-8d%-20s%-12d%-14d$%-9.2f\n",
            resumen[i].idmozo,
            resumen[i].nombre,
            resumen[i].lenventas,
            resumen[i].unidadesvendidas,
            resumen[i].totalcomision);
    }

    cout <<"\nTotal de unidades vendidas en la semana: "<< totalvendido << endl;

    return 0;
}
