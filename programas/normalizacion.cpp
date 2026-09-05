#include <iostream>
#include <cstdio>     // FILE, fopen, fread, fwrite, fseek, fclose, sprintf
#include <cstring>    // strcpy, strcmp

using namespace std;

// ESTRUCTURAS //


struct comandahistorica {
    char  fecha[11];        // "DD-MM-AAAA"
    char  nombremozo[50];   // nombre completo, repetido en cada venta
    int   codigoproducto;
    int   cantidad;
    float comision;
};


struct producto {
    int   codigo;
    char  descripcion[50];
    float precio;
    int   stockactual;
};


struct mozo {
    int   idmozo;
    char  nombre[50];
    char  password[20];     
    float totalcomision;
};


struct comanda {
    int   idmozo;
    int   codigoproducto;
    int   cantidad;
    float comision;
};

//  CONSTANTES 
const float tasa_comision = 0.10f; // 10% de comisión sugerido por la cátedra

int main(){
    
    mozo mozos[100];                                  // vector de mozos hasta 100.
    int lenmozos = 0;                                 
    producto productos[100];                          // vector de productos hasta 100.

    FILE* archivoscomandas = fopen("comandas_historicas.dat","rb");      // abrimos el archivos de comandas.

        if(archivoscomandas == NULL){
            cout<<"error al abrir el archivo de comandas historicas"<<endl;    // verificar que el archivo se abrio correctamente.
            return 1;
        }

    comandahistorica aux;                             // registro auxiliar de comandashistoricas para leer los campos del archivo.

    for(int  i = 0; i<100; i++){                      // inicializar la comisiontotal de cada mozo en 0.
        mozos[i].totalcomision = 0;
    }

    fread(&aux,sizeof(comandahistorica),1,archivoscomandas);          // leemos el primer registro de comandashistoricas.

        for(int i = 0; i<100; i++){                                  // recorrer el vector de mozos para ver si el nombre del mozo ya existe, si existe, sumarle la comision.
             
            if(fread(&aux,sizeof(comandahistorica),1,archivoscomandas) !=1){
                break;
            }

            if(mozos[i].nombre[0]=='\0'){
               strcpy(mozos[i].nombre, aux.nombremozo);
                mozos[i].idmozo = i+1;
                mozos[i].totalcomision += aux.comision;
                fread(&aux, sizeof(comandahistorica),1,archivoscomandas);      // leemos el siguiente registro de comandashistoricas.
            }

            else{
                if(strcmp(mozos[i].nombre,aux.nombremozo)== 0){
                    mozos[i].totalcomision += aux.comision;
                    fread(&aux, sizeof(comandahistorica),1,archivoscomandas);   // leemos el siguiente registro de comandashistoricas.
                }                
            }
        
        }

        for(int i =0; i<5; i++){                                  // mostrar los mozos y su comision total.
            if(mozos[i].nombre[0]!='\0'){
                cout<<"Nombre del mozo: "<<mozos[i].nombre<<endl;
                cout<<"Comision total: "<<mozos[i].totalcomision<<endl;
            }
        }
 
    fclose(archivoscomandas);



    FILE *archivomozos = fopen("mozos.dat","wb");

        if (archivomozos == NULL){
            cout << "error al crear mozos.dat" << endl;
            return 1;
        }
        
    fwrite(mozos, sizeof(mozo), lenmozos, archivomozos);      // escribir el vector de mozos en el archivo mozos.dat

    fclose(archivomozos);                                     // cerrar archivo de mozos


    cout << "archivo mozos.dat creado correctamente"<< endl;

    return 0;
}
