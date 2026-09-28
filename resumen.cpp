#include <iostream>
#include <cstdio>  // FILE, fopen, fread, fclose, sprintf
#include <cstring> // strcpy
using namespace std;

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


int main(){
    cout << "          RESUMEN DE CIERRE          " << endl;

    resumensemanal resumen[100];
    int lenresumen = 0;

    int totalvendido = 0;

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
    printf("%-8s%-12s%-14s%-10s\n", "ID", "Ventas", "Unidades", "Comision");
    cout << endl;

    for (int i = 0; i < lenresumen; i++){
        printf("%-8d%-12d%-14d$%-9.2f\n",
            resumen[i].idmozo,
            resumen[i].lenventas,
            resumen[i].unidadesvendidas,
            resumen[i].totalcomision);
    }

    cout <<"\nTotal de unidades vendidas en la semana: " << totalvendido << endl;

    return 0;
}
