/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla.h"
#include "errores.h"
#define EXTENSION 3     //Espacio para guardar leer mas datos de los ficheros


int obtenerCarpExistentes(int exist_carp)
{/*Funcion que consigue el numero de carpetas almacenados en ficheros*/
    FILE *carpet = fopen("carpetas.txt", "r");
    char datos[INTRODUCIR];
    exist_carp = -1;     //Ya que en el bucle que determina este valor, simpre nos hara un loop extra

    if (carpet == NULL){    //1.A En caso de que no existan datos de carpetas
        carpet = fopen("carpetas.txt", "w");
        exist_carp = 0;
    } else {                //1.B En caso de que haya datos de carpetas
        while(!feof(carpet)){
            fgets(datos, INTRODUCIR, carpet);
            exist_carp++;
        }   //Bucle para detectar cuantas carpetas hay almacenados
    }

    fclose(carpet);
    return exist_carp;
}


int obtenerCorrExistentes(int exist_corr)
{/*Funcion que consigue el numero de correos almacenados en ficheros*/
    FILE *corre = fopen("correos.txt", "r");
    char datos[INTRODUCIR];
    exist_corr = -1;     //Ya que en el bucle que determina este valor, simpre nos hara un loop extra

    if (corre == NULL){    //1.A En caso de que no existan datos de carpetas
        corre = fopen("correos.txt", "w");
        exist_corr = 0;
    } else {                //1.B En caso de que haya datos de carpetas
        while(!feof(corre)){
            fgets(datos, INTRODUCIR, corre);
            exist_corr++;
        }   //Bucle para detectar cuantas carpetas hay almacenados
    }

    fclose(corre);
    return exist_corr;
}


info_carpetas * cargarDatosCarpetas(info_carpetas *carp, int num_carp, int *long_carp)
{/*Funcion que carga los datos de los ficheros en la estructura dinamica de carpetas*/
    FILE *carpet = fopen("carpetas.txt", "r");
    char datos[INTRODUCIR];
    int i = 0, j = 0;

    if (carpet == NULL){    //1.A En caso de que no existan datos de carpetas
        carpet = fopen("carpetas.txt", "w");
    } else {                //1.B En caso de que haya datos de carpetas

        carp = (info_carpetas *)malloc(num_carp * sizeof(info_carpetas));

        for(i = 0; i < num_carp; i++){

            fgets(datos, INTRODUCIR, carpet);
            datos[strcspn(datos, "\n")] = '\0';
            carp[i].contenido.longitud = strlen(datos);
            carp[i].contenido.nombre = (char *)malloc(carp[i].contenido.longitud * sizeof(char));
            for(j = 0; j < carp[i].contenido.longitud; j++)
                carp[i].contenido.nombre[j] = datos[j];
            carp[i].numero_correos = 0;
            carp[i].tipo = i;
            carp[i].eliminar = 'N';

            if(carp[i].contenido.longitud > *long_carp)  //2.A Entra si el nuevo nombre de la carpeta, tiene una longitud mas larga
                *long_carp = carp[i].contenido.longitud;
        }   //Bucle para llenar la estructura dinamica con los datos almacenados
    }

    fclose(carpet);
    return carp;
}


info_correos * cargarDatosCorreos(info_carpetas *carp, info_correos *corr, int num_corr, int *long_corr)
{/*Funcion que carga los datos de carpetas, de los ficheros, a la estructura dinamica de ficheros*/
    FILE *corre = fopen("correos.txt", "r");
    char datos[2 * INTRODUCIR + EXTENSION];
    int tip_carp = 0;   //Permite obtener la carpeta del correo
    int i = 0, j = 0;

    if (corre == NULL){ //1.A En caso de que NO existan datos de correos
        corre = fopen("correos.txt", "w");
    } else {            //1.B En caso de que exitan datos de correos

        corr = (info_correos *)malloc(num_corr * sizeof(info_correos));

        for(i = 0; i < num_corr; i++){

            fscanf(corre, "%d", &tip_carp);
            corr[i].tipo_carpeta = tip_carp; //Posicion en la que se encuentra la posicion de la carpeta a la que pertenece
            carp[corr[i].tipo_carpeta].numero_correos++;

            fgets(datos, 2 * INTRODUCIR + EXTENSION, corre);
            datos[strcspn(datos, "\n")] = '\0';
            corr[i].leido = datos[1];           //Posicion en la que se encuentra si esta o no leido
            corr[i].eliminar = 'N';

            corr[i].contenido.longitud = strlen(datos) - EXTENSION;
            corr[i].contenido.nombre = (char *)malloc(corr[i].contenido.longitud * sizeof(char));
            for(j = 0; j < corr[i].contenido.longitud; j++)
                corr[i].contenido.nombre[j] = datos[j + EXTENSION];

            if(corr[i].contenido.longitud > (*long_corr))//2. En caso de que la longitud del nuevo nombre es mayor que el mayor actual
                *long_corr = corr[i].contenido.longitud;
        }
    }

    fclose(corre);
    return corr;
}


void imprimirCarpFich(FILE *carpet, info_carpetas *carp, int max_carp, int inicio)
{/*Funcion que premite escribit datos de las estructuras dinamicas en los ficheros*/
    int i = 0, j = 0;

    for(i = inicio; i < max_carp; i++){
        for(j = 0; j < carp[i].contenido.longitud; j++)
            fprintf(carpet, "%c", carp[i].contenido.nombre[j]);
        fprintf(carpet, "\n");
    }
}


void guardarCarpetas(info_carpetas *carp, int max_carp, int inicio)
{/*Funcion que nos pertmite guardar datos de las carpetas*/
    FILE *carpet = fopen("carpetas.txt", "r");

    if(carpet == NULL){ //1.A En caso de que la carpeta no exista, la crea e introduce los datos
        carpet = fopen("carpetas.txt", "w");
        imprimirCarpFich(carpet, carp, max_carp, inicio);

    } else {            //1.B En caso de que la carpeta exista, anyade los datos
        fclose(carpet);
        carpet = fopen("carpetas.txt", "a");
        imprimirCarpFich(carpet, carp, max_carp, inicio);
    }

    fclose(carpet);
}


void imprimirCorrFich(FILE *corre, info_correos *corr, int max_corr, int inicio, int exist_carp)
{/*Funcion que permite imprimir en los ficheros, datos de los correos*/
    int i = 0, j = 0;

    for(i = inicio; i < max_corr; i++){
        fprintf(corre, "%d:%c:", corr[i].tipo_carpeta + exist_carp, corr[i].leido);
        for(j = 0; j < corr[i].contenido.longitud; j++)
            fprintf(corre, "%c", corr[i].contenido.nombre[j]);
        fprintf(corre, "\n");
    }
}


void guardarCorreos(info_correos *corr, int max_corr, int inicio, int exist_carp)
{/*Funcion que permite guardar datos de las carpetas*/
    FILE *corre = fopen("correos.txt", "r");

    if(corre == NULL){  //1.A En caso de que la carpeta no exista, la crea e introduce los datos
        corre = fopen("correos.txt", "w");
        imprimirCorrFich(corre, corr, max_corr, inicio, exist_carp);

    } else {            //1.B En caso de que la carpeta exista, anyade los datos
        fclose(corre);
        corre = fopen("correos.txt", "a");
        imprimirCorrFich(corre, corr, max_corr, inicio, exist_carp);
    }

    fclose(corre);
}


int obtenerCarpRstnts(info_carpetas *carp, int total_carp)
{/*Funcion que nos permite obtener cuantas carpetas hay que mantener, si es que hay que eliminar alguna*/
    int num_borr = 0;   //Numero de carpetas que queremos eliminar
    int pscn_borr = 0;  //Posicion de la carpeta que borremos
    int i = 0;

    do{
        printf("\nIntroduce cuantas carpetas quieres eliminar: ");
        scanf("%d", &num_borr);
        if(num_borr < 0 || num_borr >= total_carp) //1.A En caso de que la carpeta seleccionada no exista
            errorTamanyo(&num_borr);

        else if(num_borr > 0){                    //1.B En caso de que haya carpetas que eliminar
            carpetasExistentes(carp, &total_carp);
            for(i = 0; i < num_borr; i++){
                do{
                    printf("\nIntroduce el numero de la carpeta: ");
                    scanf("%d", &pscn_borr);
                    if(pscn_borr < 0 || pscn_borr >= total_carp) //2.A En caso de que la carpeta seleccionada no exista
                        errorAccederCarp(&pscn_borr);
                    else                                        //2.B Indica que hay que eliminar la carpeta
                        carp[pscn_borr].eliminar = 'S';
                }while(pscn_borr < 0 || pscn_borr >= total_carp);
            }
        }
    }while(num_borr < 0 || num_borr >= total_carp);

    return (total_carp - num_borr);
}


info_carpetas * eliminarCarpetas(info_carpetas *carp, info_correos *corr, int *total_carp, int total_corr, int mtnr_carp, int *mtnr_corr)
{/*Borra las carpetas de los correos borrados, y edita la estructura dinamica de las carpetas*/
    info_carpetas *auxiliar = NULL;
    int i = 0, j = 0, k = 0;
    int *direcciones;       //Auxiliar para guardar las direcciones de los correos
    *mtnr_corr = total_corr;

    direcciones = (int *)malloc(total_corr * sizeof(int));
    auxiliar = (info_carpetas *)malloc(mtnr_carp * sizeof(info_carpetas));

    for(i = 0; i < (*total_carp); i++){
        if(carp[i].eliminar == 'N')         //1.A Carpetas que hay que conservar
        {
            auxiliar[j].contenido.nombre    = carp[i].contenido.nombre;
            auxiliar[j].contenido.longitud  = carp[i].contenido.longitud;
            auxiliar[j].numero_correos      = carp[i].numero_correos;
            auxiliar[j].tipo                = j;
            j++;
        }
        else if(carp[i].eliminar == 'S')    //1.B Carpetas que hay que borrar
        {
            for(k = 0; k < total_corr; k++){
                if(carp[i].tipo == corr[k].tipo_carpeta)
                    corr[k].eliminar = 'S';
            }
            *mtnr_corr -= carp[i].numero_correos;
            free(carp[i].contenido.nombre);
        }
    }

    for(i = 0; i < total_corr; i++)
        direcciones[i] = corr[i].tipo_carpeta;

    for(i = 0; i < (*total_carp); i++){
        if(carp[i].eliminar == 'S'){
            for(j = 0; j < total_corr; j++){
                if(carp[i].tipo <= corr[j].tipo_carpeta)
                    direcciones[j]--;
            }
        }
    }
    for(i = 0; i < total_corr; i++)
        corr[i].tipo_carpeta = direcciones[i];
/*Si no se realiza esto, los correos almacenados en carpetas superiores a los que se han borrado, no cambiarian de posicion*/

    carp = (info_carpetas *) realloc(carp, mtnr_carp * sizeof(info_carpetas));

    for(i = 0; i < mtnr_carp; i++){   //Recoloca los datos almacenados en la auxiliar, a las carpetas
        carp[i].contenido.nombre    = auxiliar[i].contenido.nombre;
        carp[i].contenido.longitud  = auxiliar[i].contenido.longitud;
        carp[i].numero_correos      = auxiliar[i].numero_correos;
        carp[i].tipo                = auxiliar[i].tipo;
        carp[i].eliminar            = 'N';
    }

    free(direcciones);
    free(auxiliar);     //Para que no haya problemas al volver a eliminar carpetas
    *total_carp = mtnr_carp;
    return carp;
}


int obtenerCorrRstnts(info_carpetas *carp, info_correos *corr, int total_corr)
{/*Permite obtener el numero de correos que deseamos conservar*/
    int num_borr = 0;
    int pscn_borr = 0;
    int i = 0;

    do{
        printf("\nIntroduce cuantos correos quieres eliminar: ");
        scanf("%d", &num_borr);
        if(num_borr < 0 || num_borr >= total_corr) //1.A En caso de que la carpeta seleccionada no exista
            errorTamanyo(&num_borr);

        else if(num_borr > 0){                    //1.B En caso de que haya carpetas que eliminar
            correosExistentes(corr, &total_corr);

            for(i = 0; i < num_borr; i++){
                do{
                    printf("\nIntroduce el numero del correo: ");
                    scanf("%d", &pscn_borr);
                    if(pscn_borr < 0 || pscn_borr >= total_corr) //2.A En caso de que la carpeta seleccionada no exista
                        errorAccederCarp(&pscn_borr);
                    else{                                       //2.B Indica que hay que eliminar la carpeta
                        corr[pscn_borr].eliminar = 'S';
                        carp[corr[pscn_borr].tipo_carpeta].numero_correos--;
                    }
                }while(pscn_borr < 0 || pscn_borr >= total_corr);
            }
        }
    }while(num_borr < 0 || num_borr >= total_corr);

    return (total_corr - num_borr);
}


info_correos * eliminarCorreos(info_correos *corr, int *total_corr, int mantener_corr)
{/*Permite eliminar los correos indicados*/
    info_correos *auxiliar;
    int i = 0, j = 0;

    auxiliar = (info_correos *) malloc(mantener_corr * sizeof(info_correos));

    for(i = 0; i < (*total_corr); i++)
    {
        if(corr[i].eliminar == 'N'){
            auxiliar[j].contenido.nombre    = corr[i].contenido.nombre;
            auxiliar[j].contenido.longitud  = corr[i].contenido.longitud;
            auxiliar[j].leido               = corr[i].leido;
            auxiliar[j].tipo_carpeta        = corr[i].tipo_carpeta;
            j++;
        }
        else if(corr[i].eliminar == 'S')
            free(corr[i].contenido.nombre);
    }

    corr = (info_correos *) realloc(corr, mantener_corr * sizeof(info_correos));

    for(i = 0; i < mantener_corr; i++){
        corr[i].contenido.nombre    = auxiliar[i].contenido.nombre;
        corr[i].contenido.longitud  = auxiliar[i].contenido.longitud;
        corr[i].leido               = auxiliar[i].leido;
        corr[i].tipo_carpeta        = auxiliar[i].tipo_carpeta;
        corr[i].eliminar            = 'N';
    }

    free(auxiliar);
    *total_corr = mantener_corr;
    return corr;
}


void finalGuardarDatos(info_carpetas *carp, info_correos *corr, int total_carp, int total_corr, int exist_carp, int exist_corr)
{/*Guarda los datos al final del programa, en caso de no haver decidido cargarlos en ningun punto*/
    char sobreescribir = 'N';

    if(exist_carp > 0){ //1. En caso de que haya datos ya almacenados
        do{
            printf("\n\nQuieres sobreescribir tus nuevos correo(s) y carpeta(s) [ S / N ]: ");
            fflush(stdin);
            scanf("%c", &sobreescribir);
            if(sobreescribir == 'S' || sobreescribir == 's'){//2.A En caso querer sobreescribir los datos
                remove("carpetas.txt");
                exist_carp = 0;
                remove("correos.txt");
                exist_corr = 0;
            }
            else if(sobreescribir != 'S' && sobreescribir != 's' && sobreescribir != 'N' && sobreescribir != 'n')
                //2.B Si se introduce un comando desconocido
                errorComandoDesconocido();
        }while(sobreescribir != 'S' && sobreescribir != 's' && sobreescribir != 'N' && sobreescribir != 'n');
    }
    guardarCarpetas(carp, total_carp, INICIO);
    guardarCorreos(corr, total_corr, INICIO, exist_carp);
}