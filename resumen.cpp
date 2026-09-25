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
