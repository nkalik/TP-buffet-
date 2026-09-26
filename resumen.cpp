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
    
    cout << "Ingrese la semana (ej 1): ";
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

        resumen[lenresumen].idmozo = aux.idmozo;
        resumen[lenresumen].lenventas++;
        resumen[lenresumen].unidadesvendidas += aux.cantidad;
        resumen[lenresumen].totalcomision += aux.comision;

        lenresumen++;

        totalvendido += aux.cantidad;
    }

    fclose(archivosemanal);
    return 0;
}
