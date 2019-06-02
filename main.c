/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: r0p3
 *
 * Created on 25 de mayo de 2019, 20:19
 */

#include <stdio.h>
#include <stdlib.h>
#include "tabla.h"
#include "errores.h"
#define MINCARP 1       //Numero minimo posible para introducir carpetas
#define MINCORR 0       //Numero minimo posible para introducir correos
#define INICIO 0

/*
 *
 */

void tipoLinea(texto *, info_carpetas *, info_correos *, int*, int*, int, int, int, int);
void imprimirInfoCarp(info_carpetas *, int, int *);


int main(int argc, char** argv)
{
    int simbolos_simples[] = {196, 179, 218, 192, 217, 191, 194, 195, 193, 180, 197, 32};   //Numeros en el codigo ASCII de lineas simples que conformaran las celdas
    int simbolos_dobles[] = {205, 186, 201, 200, 188, 187, 203, 204, 202, 185, 206, 32};    //Numeros en el codigo ASCII de lineas dobles que conformaran las celdas
    /*Los numeros, que representan los caracteres a imprimir, estan en arrays para tenerlos juntos*/
    texto usuario;                    //Almacena el nombre de usuario
    info_carpetas *carpetas = NULL;   //Puntero a estructura, para tener con memoria dinamica las carpetas necesarias
    info_correos *correos = NULL;     //Puntero a estructura, para tener con memoria dinamica los correos necesarios
    char datos_antiguos = 'N';
    int exist_carp = 0;         //Numero de carpetas existentes
    int new_carp = 0;           //Numero de carpetas a anyadidas
    int total_carp = 0;
    int long_carp = 0;          //Longitud maxima de las carpetas
    int exist_corr = 0;         //Numero de correos existentes
    int new_corr = 0;           //Numero de correos a anyadir
    int total_corr = 0;
    int long_corr = 0;          //Longitud maxima de los correos
    char repetir = 'N';         //Caracter que nos servirá para decidir reimprimir una tabla o no
    char anyadir = 'N';
    char modif_carp = 'N';
    char modif_corr = 'N';
    int i = 0;
    
    pedirNombreUsuario(&usuario);
    imprimirEncabezadoInicial(simbolos_simples, &usuario);
    imprimirEncabezadoInicial(simbolos_dobles, &usuario);
    exist_carp = obtenerCarpExistentes(exist_carp);
    exist_corr = obtenerCorrExistentes(exist_corr);
    
    do{
        printf("\nQuieres cargar los datos de las carpetas anteriores? [ S / N ]: ");
        fflush(stdin);
        scanf("%c", &datos_antiguos);
        switch (datos_antiguos)
        {
            case 'S':
            case 's':
                carpetas = cargarDatosCarpetas(carpetas, exist_carp, &long_carp);
                correos  = cargarDatosCorreos(carpetas, correos, exist_corr, &long_corr);
                do{
                    printf("\nQuieres anyadir mas correos y carpetas [ S / N ]: ");
                    fflush(stdin);
                    scanf("%c", &anyadir);
                    if(anyadir != 'S' && anyadir != 's' && anyadir != 'N' && anyadir != 'n')
                        errorComandoDesconocido();
                    else if (anyadir == 'S' || anyadir == 's')
                    {
                        pedirNumeroCC(&new_carp, "anyadir de carpetas (minimo 1)", MINCARP);
                        carpetas = (info_carpetas *) realloc(carpetas, (exist_carp + new_carp) * sizeof(info_carpetas));
                        printf("\n\n");
                        pedirNombreCarpetas(carpetas, &long_carp, (exist_carp + new_carp), exist_carp);
                        guardarCarpetas(carpetas, (exist_carp + new_carp), exist_carp);
                        printf("\n\n");
                        pedirNumeroCC(&new_corr, "anyadir de correos (minimo 0)", MINCORR);

                        if(new_corr > 0){ //1. En caso de que haya correos que usar
                            correos = (info_correos *) realloc(correos, (exist_corr + new_corr) * sizeof(info_correos));
                            printf("\n");
                            pedirNombreCorreos(correos, &long_corr, (exist_corr + new_corr), exist_corr);
                            printf("\n\n");
                            meterCorreosACarpetas(carpetas, correos, (exist_carp + new_carp), (exist_corr + new_corr), exist_corr);
                            guardarCorreos(correos, (exist_corr + new_corr), exist_corr, INICIO);
                        }
                    }
                }while(anyadir != 'S' && anyadir != 's' && anyadir != 'N' && anyadir != 'n');
                total_carp = exist_carp + new_corr;
                total_corr = exist_corr + new_corr;
                break;
            
            case 'N':
            case 'n':
                pedirNumeroCC(&new_carp, "de carpetas (minimo 1)", MINCARP);
                total_carp = new_carp;
                carpetas = (info_carpetas *) malloc(new_carp * sizeof(info_carpetas));
                printf("\n\n");
                pedirNombreCarpetas(carpetas, &long_carp, new_carp, INICIO);
                //guardarCarpetas(carpetas, new_carp, INICIO);
                printf("\n\n");
                pedirNumeroCC(&new_corr, "de correos (minimo 0)", MINCORR);
                total_corr = new_corr;

                if(new_corr > 0){ //1. En caso de que haya correos que usar
                    correos = (info_correos *) malloc(new_corr * sizeof(info_correos));
                    printf("\n");
                    pedirNombreCorreos(correos, &long_corr, new_corr, INICIO);
                    printf("\n\n");
                    meterCorreosACarpetas(carpetas, correos, new_carp, new_corr, INICIO);
                    //guardarCorreos(correos, new_corr, INICIO);
                }
                break;
            
            default:
                errorComandoDesconocido();
        }
    }while(datos_antiguos != 'S' && datos_antiguos != 's' && datos_antiguos != 'N' && datos_antiguos != 'n');
    
    
    do{ //Bucle que nos permite reimprimir, o no, una tabla
      tipoLinea(&usuario, carpetas, correos, simbolos_simples, simbolos_dobles, total_carp, long_carp, long_corr, exist_carp);

        do{//Bucle para introducir un caracter acertado
            printf("\n\nQuieres imprimir una nueva tabla? [ S / N ]: ");
            fflush(stdin);
            scanf("%c", &repetir);
            if(repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n')
                //2. Se activa en caso de NO introducir un caracter para continuar o terminar el programa
                errorComandoDesconocido();
            if(repetir == 'S' || repetir == 's'){
                
                if(datos_antiguos == 'N' || datos_antiguos == 'n'){
                    do{
                        printf("Quieres cargar tus datos antiguos [ S / N ]: ");
                        fflush(stdin);
                        scanf("%c", &datos_antiguos);
                        if(datos_antiguos == 'S' || datos_antiguos == 's'){
                            guardarCarpetas(carpetas, total_carp, INICIO);
                            guardarCorreos(correos, total_corr, INICIO, exist_carp);
                            for(i = 0; i < total_corr; i++)
                                free(correos[i].contenido.nombre);
                            for (i = 0; i < total_carp; i++)
                                free(carpetas[i].contenido.nombre);
                            free(correos);
                            free(carpetas);
                            exist_carp += total_carp;
                            exist_corr += total_corr;
                            carpetas    = cargarDatosCarpetas(carpetas, exist_carp, &long_carp);
                            total_carp  = exist_carp;
                            correos     = cargarDatosCorreos(carpetas, correos, exist_corr, &long_corr);
                            total_corr  = exist_corr;
                        }
                        else if(datos_antiguos != 'S' && datos_antiguos != 's' && datos_antiguos != 'N' && datos_antiguos != 'n')
                            errorComandoDesconocido();
                    }while(datos_antiguos != 'S' && datos_antiguos != 's' && datos_antiguos != 'N' && datos_antiguos != 'n');
                }
                
                
                
                /*do{
                    printf("\n\nQuieres modificar las carpetas? [ Quitar / Anyadir / Dejar ] [ Q / A / D ]: ");
                    fflush(stdin);
                    scanf("%c", &modif_carp);
                    switch (modif_carp)
                    {
                        case 'Q':   //3.A En caso de querer quitar carpetas
                        case 'q':
                            carpetas = eliminarCarpetas(carpetas, &total_carp, &exist_corr);
                            break;

                        case 'A':   //3.B En caso de querer anyadir carpetas
                        case 'a':
                            pedirNumeroCC(&new_carp, "anyadir de carpetas (minimo 1)", MINCARP);
                            carpetas = (info_carpetas *) realloc(carpetas, (new_carp + total_carp) * sizeof(info_carpetas));
                            pedirNombreCarpetas(carpetas, &long_carp, (new_carp + total_carp), total_carp);
                            guardarCarpetas(carpetas, (new_carp + total_carp), total_carp);
                            total_carp += new_carp;
                            break;

                        case 'D':   //3.C En caso de no querer cambiar nada
                        case 'd':
                            break;

                        default:    //3.D En caso de pulsar una tecla equivocada
                            errorComandoDesconocido();
                    }

                }while(modif_carp != 'Q' && modif_carp != 'q' && modif_carp != 'A' && modif_carp != 'a' && modif_carp != 'D' && modif_carp != 'd');

                /*do{
                    printf("\n\nQuieres mopdificar los correos? [ Quitar / Anyadir / Dejar ] [ Q / A / D ]: ");
                    fflush(stdin);
                    scanf("%c", &modif_corr);
                    switch (modif_corr)
                    {
                        case 'Q':   //4.A En caso de querer quitar correos
                        case 'q':

                            break;

                        case 'A':   //4.B En caso de querer anyadir correos
                        case 'a':
                            pedirNumeroCC(&new_corr, "anyadir de correos (minimo 0)", MINCORR);

                            if(new_corr > 0){ //1. En caso de que haya correos que usar
                                correos = (info_correos *) realloc(correos, new_corr * sizeof(info_correos));
                                printf("\n");
                                pedirNombreCorreos(correos, &long_corr, (new_corr + exist_corr), exist_corr);
                                printf("\n\n");
                                meterCorreosACarpetas(carpetas, correos, total_carp, total_corr, exist_corr);
                                guardarCorreos(correos, (new_corr + exist_corr), exist_corr);
                            }
                            break;

                        case 'D':   //4.C En caso de no querer cambiar nada
                        case 'd':
                            break;

                        default:    //4.D En caso de pulsar una tecla equivocada
                            errorComandoDesconocido();
                    }

                }while(modif_corr != 'Q' && modif_corr != 'q' && modif_corr != 'A' && modif_corr != 'a' && modif_corr != 'D' && modif_corr != 'd');*/
            }
        }while (repetir != 'S' && repetir != 's' && repetir != 'N' && repetir != 'n');

    }while(repetir == 'S' || repetir == 's');
    
    
    
    
    
    if(datos_antiguos == 'N' || datos_antiguos == 'n'){ //En caso de que no se haya elegido recargar los datos antiguos, para guardalos en ficheros
        guardarCarpetas(carpetas, total_carp, INICIO);
        guardarCorreos(correos, total_corr, INICIO, exist_corr);
    }
    liberar(&usuario, carpetas, total_carp, correos, total_corr);
    return (EXIT_SUCCESS);
}


void tipoLinea(texto *user, info_carpetas *carp, info_correos *corr, int *simples, int *dobles, int total_carp, int long_carp, int long_corr, int exist_carp)
{/*Funcion que nos permite seleccionar el tipo de linea que deseamos imprimir*/
    int long_total = 2;     //Variable resultante de todas las demas longitudes. Iniciualizado a 2 porque corresponde a las 2 columnas que no entran en las longitudes
    char elecc_simbol = 'S';//Variable utilizada para decidir si imprimir lineas simples o doble
    int ult_carp = 0;       //Para imprimir el numero restante de carpetas, ya que es necesario conocer la ultima posicion impresa
    int elecc_carp = 0;     //Variable para elegir la carpeta que mostrar
    long_total += long_carp + long_corr + LONG_DER;

    do{
        printf("\n\nQue tipo de linea quieres utilizar, simple o doble? [ S / D ]: ");
        fflush(stdin);
        scanf("%c", &elecc_simbol);
        
        switch (elecc_simbol)
        {
            case 'S':   //1.A Se activa si queremos imprimir una tabla con lineas SIMPLES
            case 's':
                imprimirInfoCarp(carp, total_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, total_carp, carp[elecc_carp].numero_correos, simples, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(total_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, simples, elecc_carp, ult_carp, carp, corr);
                break;
            
            case 'D':   //1.B Se activa si queremos imprimir una tabla con lineas DOBLES
            case 'd':
                imprimirInfoCarp(carp, total_carp, &elecc_carp);
                imprimirEncabezado(long_total, long_carp, long_corr, total_carp, carp[elecc_carp].numero_correos, dobles, elecc_carp, ult_carp, user, carp);
                imprimirCeldasCC(total_carp, carp[elecc_carp].numero_correos, long_total, long_carp, long_corr, dobles, elecc_carp, ult_carp, carp, corr);
                break;
                
            default:    //1.C Se activa si caracter introducido no es para entrar en las otras 2 condiciones
                errorComandoDesconocido();
        }
        
    }while (elecc_simbol != 'S' && elecc_simbol != 's' && elecc_simbol != 'D' && elecc_simbol != 'd');
}


void imprimirInfoCarp(info_carpetas *carp, int total_carp, int *elecc_carp)
{
    printf("\nIntroduce el contenido de la carpeta que quieras ver:\n");
    carpetasExistentes(carp, &total_carp);

    do{
        printf("\nIntroduce el numero de la carpeta: ");
        scanf("%d", elecc_carp);
        
        if(*elecc_carp < 0 || *elecc_carp >= total_carp){ //En caso de que la carpeta seleccionada no exista
            errorAccederCarp(elecc_carp);
        }
    }while(*elecc_carp < 0 || *elecc_carp >= total_carp);
}
