/**********************************************************************/
/* Objetivo: El programa ordena de formarápida unaestructura de */
/* direcciones que se encuentraalmacenada en unfichero */
/* de acceso directo. */
/* Entrada: Nombre del fichero. */
/* Salida: El fichero ordenado. */
/¡tí*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM.ELEMENT 100 /* Este es un número arbitrario que se debe de
determinar dinámicamente para cada lista */
struct direc {
char nombre [30] ;
char calle [40];
char ciudad [20] ;
char provincia[2] ;
char codigo [10];
} dinfo;
void rapida_disco(FILE *fp, int cont);
void or.disco(FILE *fp, int izq, int der);
void cambiar.campos(FILE *fp, long i, long j);
char *obtener_codigo(FILE *fp, long rec) ;
void main(int argc, char *argv[])
{ FILE *fp;
if(argc!=2) {
    printf("Uso: ordena <nombre_fichero>\n");
exit(l);
if((fp=fopen(argv [1] ,"rb+"))==NULL) í
printf("Imposible abrir archivo para lectura/escritura\n");
exit(l) ;
rapida_disco(fp,NUM_ELEMENT);
fclose(fp);
printf("Lista ordenada\n");
/* Una ordenación rápida para archivos */
void rapida_disco(FILE *fp, int cont)
í
or_disco(fp, 0, cont-1);
J
void or_disco(FILE *fp, int izq, int der)
{ long int i,j;
char x[100] ;
i = izq;
j = der;
strcpy(x, obtener_codigo(fp,(long)(i+j) /2));
/* Obtener código medio */
/* La ordenación se realiza por medio de intercambio */
do {
while(strcmp(obtener_codigo(fp,i),x)<0 && i<der) i++;
while(strcmp(obtener_codigo(fp,j),x)>0 && j>izq) j--;
if(i<=j) {
cambiar_campos(fp,i, j ) ;
j—;
1 while(i<=j);
if(izq<j)
or.disco(fp,izq,(int)j);
if(i<der)
or_disco(fp, (int)i,der);
}
void cambiar_canpos(FILE *fp, long i, long j)
{ char a[sizeof(dinfo)] , b[sizeof(dinfo)];
/* primero lee los registros i y j */
fseek(fp, sizeof(dinfo)*i, 0);
fread(a, sizeof(dinfo), 1, fp);
fseek(fp, sizeof(dinfo)*j , 0);
fread(b, sizeof(dinfo), 1, fp);
/* después los escribe en orden contrario */
fseek(fp, sizeof(dinfo)*j, 0);
fwrite(a, sizeof(dinfo), 1, fp) ;
fseek(fp, sizeof(dinfo)*i, 0);
fread(b, sizeof(dinfo), 1, fp) ;
}
/* Devolver un puntero al código postal */
char *obtener_codigo(FILE *fp, long rec)
í struct direc *p;
p = &dinfo;
fseek(fp, rec*sizeof(dinfo), 0);
fread(p, sizeof(dinfo), 1, fp);
return dinfo.codigo;
} 
