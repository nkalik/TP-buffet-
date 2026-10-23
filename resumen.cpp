#include <cstdio>  // FILE, fopen, fread, fclose, sprintf
#include <cstring> // strcpy
#include <iostream>

using namespace std;

struct comanda {
  int idmozo;
  int codigoproducto;
  int cantidad;
  float comision;
};

// Estructura que se va a usar para el resumen semanal de cada mozo
struct resumensemanal {
  int idmozo;
  char nombre[50];
  int lenventas;
  int unidadesvendidas;
  float totalcomision;
};

/* Pide un entero entre min y max, si el usuario escribe letras o se equivoca de
numero avisa y vuleve a pedir. Despues de tres intentos devuelve false*/
bool leerentero(const char mensaje[], int min, int max, int &valor) {
  int intentos = 0;

  while (intentos < 3) {
    cout << mensaje;
    if (!(cin >> valor)) {
      // el usuario no escribio un numero, limpia y descarta el tipeo
      cin.clear();
      cin.ignore(100, '\n');
      cout << "\nERROR: ingrese un numero entero" << endl;
      intentos++;
    } else {
      if (valor >= min && valor <= max) {
        return true;
      }
      cout << "\nERROR: el valor debe estar entre " << min << " y " << max
           << endl;
      intentos++;
    }
  }
  return false;
}

/* Pide semana y mes para armar el nombre del archivo, si el archivo no existe
vuelve a pedir. Despues de tres intentos devuelve false*/
bool pedirarchivo(char nombrearchivo[]) {
  int semana, mes;
  int intentos = 0;

  while (intentos < 3) {
    cout << endl;

    if (!leerentero("Ingrese la semana (1-5): ", 1, 5, semana)) {
      return false;
    }
    if (!leerentero("Ingrese el mes (1-12): ", 1, 12, mes)) {
      return false;
    }

    sprintf(nombrearchivo, "comandas_semana_s%d-%02d.dat", semana, mes);

    FILE *archivosemanal = fopen(nombrearchivo, "rb");
    if (archivosemanal != NULL) {
      fclose(archivosemanal);
      return true;
    }
    cout << "\nERROR: No se encuentra " << nombrearchivo << endl;
    cout << "\nIntente otra vez" << endl;
    intentos++;
  }
  cout << "\nTe quedaste sin intentos, volve a ejecutar!!!" << endl;
  return false;
}

int main() {
  cout << "          RESUMEN DE CIERRE          " << endl;

  resumensemanal resumen[100];
  int lenresumen = 0;
  int totalvendido = 0;
  char nombrearchivo[50];

  // Pide semana y mes, si se agotan los intentos se cancela la ejecucion
  if (!pedirarchivo(nombrearchivo)) {
    cout << "Se cancela el resumen.\n" << endl;
  }
  FILE *archivosemanal = fopen(nombrearchivo, "rb");
  if (archivosemanal == NULL) {
    cout << "Error al abrir " << nombrearchivo << endl;
    return 1;
  }

  comanda aux;

  while (fread(&aux, sizeof(comanda), 1, archivosemanal) == 1) {

    // entero auxiliar, se usa en el if solo para el primer registro leido
    int pos = lenresumen - 1;

    // acumula los registros con mismo mozo, y crea un nuevo registro resumen
    // cuando el mozo es distinto al ultimo registro leido
    if (pos == -1 || resumen[pos].idmozo != aux.idmozo) {

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
  cout.width(8);
  cout << "ID";
  cout.width(12);
  cout << "VENTAS";
  cout.width(14);
  cout << "UNIDADES";
  cout.width(10);
  cout << "COMISION";
  cout << endl;
  cout << endl;

  // comision con 2 decimales
  cout.setf(ios::fixed);
  cout.precision(2);

  for (int i = 0; i < lenresumen; i++) {
    cout.width(8);
    cout << resumen[i].idmozo;
    cout.width(12);
    cout << resumen[i].lenventas;
    cout.width(14);
    cout << resumen[i].unidadesvendidas;
    cout.width(10);
    cout << "$" << resumen[i].totalcomision << endl;
  }

  cout << "\nTotal de unidades vendidas en la semana: " << totalvendido << endl;

  return 0;
}
