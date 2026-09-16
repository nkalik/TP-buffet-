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

struct todaslasventas{
    int codigoproducto;
    int cantidad;
    int idmozo;
    float comision;
};

struct ventaspordia{
    char fecha[11];
    todaslasventas ventas [50];
    int lenventas = 0;
};

//  CONSTANTES 
const float tasa_comision = 0.10f; // 10% de comisión sugerido por la cátedra

int main(){
    
   
    mozo mozos[100];                                    // vector de mozos hasta 100.
    int lenmozos = 0;  

    producto productos[100];                            // vector de productos hasta 100.

    comandahistorica comandashistoricas[100];           // vector de comandashistoricas hasta 100.
    int lencomandashistoricas = 0; 

    FILE* archivoscomandas = fopen("comandas_historicas.dat","rb");      // abrimos el archivos de comandas.

    if(archivoscomandas == NULL){
        cout<<"error al abrir el archivo de comandas historicas"<<endl;    // verificar que el archivo se abrio correctamente.
        return 1;
    }

    comandahistorica aux;                 // registro auxiliar de comandashistoricas para leer los campos del archivo.

    for(int  i = 0; i<100; i++){     
        mozos[i].nombre[0] = '\0';        // inicializar la comisiontotal de cada mozo en 0.
        mozos[i].totalcomision = 0;
    }

    while(lencomandashistoricas < 100 && fread(&aux,sizeof(comandahistorica),1,archivoscomandas) == 1){   // leer el archivo de comandas historicas hasta 100 registros o hasta que no haya mas registros.
         
        comandashistoricas[lencomandashistoricas] = aux;        // vector de comandashistoricas para almacenar los registros leidos del archivo.
        lencomandashistoricas++;
             
        int pos = -1;                                           // variable para almacenar la posicion del mozo en el vector de mozos.
               
        for(int i = 0; i<lenmozos;i++){                         // recorrer el vector de mozos para ver si el nombre del mozo ya existe.
            if(strcmp(mozos[i].nombre,aux.nombremozo)== 0){
                pos = i;
                mozos[pos].totalcomision += aux.comision;       // si el nombre del mozo ya existe, almacenar la posicion en pos.
            }
        }

        if(pos == -1){                                          // si el nombre del mozo no existe, agregarlo al vector de mozos.
            strcpy(mozos[lenmozos].nombre, aux.nombremozo);
            mozos[lenmozos].idmozo = lenmozos+1;
            mozos[lenmozos].totalcomision += aux.comision;
                
            lenmozos++;
        }                             
    }

    cout<<"\ncomandas leidas correctamente: "<<lencomandashistoricas<<endl;
 
    fclose(archivoscomandas);

    FILE *archivomozos = fopen("mozos.dat","wb");

    fwrite(mozos, sizeof(mozo), lenmozos, archivomozos);            // escribir el vector de mozos en el archivo mozos.dat

        if (archivomozos == NULL){
            cout << "\nerror al crear mozos.dat" << endl;
            return 1;
        }
        else{
            cout <<"\narchivo mozos.dat creado correctamente" << endl;
        }

    fclose(archivomozos);                                           // cerrar archivo de mozos

    archivomozos = fopen("mozos.dat","rb");                         // abrir archivo de mozos para leerlo

    mozo auxmozo;                                                   // registro auxiliar de mozo para leer los campos del archivo.

    while(fread(&auxmozo,sizeof(mozo),1,archivomozos) == 1){        // leer el archivo de mozos hasta que no haya mas registros.
        cout<<"\nID del mozo: "<<auxmozo.idmozo<<endl;
        cout<<"Nombre: "<<auxmozo.nombre<<endl;
        cout<<"Comision total: "<<auxmozo.totalcomision<<endl;
    }
    
    fclose(archivomozos);                                           // cerrar archivo de mozos

    archivoscomandas = fopen("comandas_historicas.dat","rb");

    ventaspordia comandas[32];                                      // vector de ventas por dia hasta 100 dias.
    
    while(fread(&aux,sizeof(comandahistorica),1,archivoscomandas) == 1){

        int dia = (aux.fecha[0] - '0') * 10 + (aux.fecha[1] - '0');
        int iddia = 0;

        strcpy(comandas[dia].fecha, aux.fecha);
        
        for(int i = 0; i<lenmozos; i++){
            if(strcmp(mozos[i].nombre, aux.nombremozo) == 0){
                iddia = mozos[i].idmozo;
            }
        }

        comandas[dia].ventas[comandas[dia].lenventas].codigoproducto = aux.codigoproducto;
        comandas[dia].ventas[comandas[dia].lenventas].cantidad = aux.cantidad;
        comandas[dia].ventas[comandas[dia].lenventas].idmozo = iddia;
        comandas[dia].ventas[comandas[dia].lenventas].comision = aux.comision;

        comandas[dia].lenventas++;
    }

    for(int j = 0; j<32; j++){

        cout<<endl;
        cout<<comandas[j].fecha;
        cout<<endl;

    }

    return 0;
}
