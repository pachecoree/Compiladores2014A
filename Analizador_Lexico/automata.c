#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERR 99
#define ACP -1

//variables globales

int indx = 0;
char token[20]= "";

/*
ASCII VALUES
<letra> = { 65...90 } | { 97...122 }
<digito> = { 48...57 }
<caracter> = { 32...47} | { 58...64} | { 91...96} | { 123...126 }
+ = 43
- = 45
/ = 47
* = 42
_ = 95
. = 46
> = 62
< = 60
= = 61
! = 33
# = 35
" = 34
' = 39
% = 37
*/
//28 palabras reservadas



char palabras_reservadas[29][12] = {
    "produce",
    "ejec",
    "con",
    "afirmar",
    "final",
    "mientras",
    "interrumpe",
    "para",
    "pasa",
    "clase",
    "desde",
    "imprime",
    "continua",
    "global",
    "eleva",
    "def",
    "si",
    "regresa",
    "del",
    "importar",
    "prueba",
    "otsi",
    "en",
    "mientras",
    "otro",
    "es",
    "excepto",
    "lamda"
    "pasa"
    };

char constantes[3][15] = {
    "entero",
    "flotante",
    "largo"
};

char operadores_bool[2][15] = {
    "verdadero",
    "falso"
};

char operadores_logicos[3][5] = {
    "y",
    "o",
    "no"
};

void clasifica_identificador(char iden[]) {
    int i;
    for(i=0; i<29;i++) {
        if( strcmp(iden,palabras_reservadas[i]) == 0) {
            strcpy(token,"RESERVADA");
            return;
        }
    }
    for(i=0; i<3;i++) {
        if( strcmp(iden,constantes[i]) == 0) {
            strcpy(token,"CONSTANTE");
            return;
        }
    }
    for(i=0; i<2;i++) {
        if( strcmp(iden,operadores_bool[i]) == 0) {
            strcpy(token,"BOOLEANO");
            return;
        }
    }
    for(i=0; i<3;i++) {
        if( strcmp(iden,operadores_logicos[i]) == 0) {
            strcpy(token,"LOGICO");
            return;
        }
    }
    strcpy(token,"IDENTIFICADOR");
    return;
}

int columna = 0;
char lexema[40] = "";
char arreglo[800000];

int estados[46][34] = {
1,1,2,4,1,1,1,1,1,1,45,36,37,38,32,34,26,30,28,31,39,42,45,45,45,45,45,45,45,45,45,45,45,46,
1,1,1,1,1,1,1,1,1,1,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,2,2,3,ACP,ACP,ACP,15,25,11,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,9,5,7,18,ACP,13,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,ERR,6,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,ACP,6,21,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
8,ERR,ERR,8,ERR,8,ERR,ERR,8,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
8,ACP,ACP,8,22,8,ACP,ACP,8,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,ERR,10,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,ACP,10,23,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,12,12,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,12,12,ACP,ACP,ACP,ACP,15,25,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,14,14,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,14,14,ACP,ACP,ACP,ACP,18,24,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,17,17,ERR,ERR,ERR,ERR,ERR,ERR,ERR,16,16,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ERR,ERR,17,17,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,17,17,ACP,ACP,ACP,ACP,ACP,25,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,20,20,ERR,ERR,ERR,ERR,ERR,ERR,ERR,19,19,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ERR,ERR,20,20,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,20,20,ACP,ACP,ACP,ACP,ACP,24,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,29,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,33,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,35,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,41,40,40,40,40,40,40,40,40,40,40,40,40,40,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,41,40,40,40,40,40,40,40,40,40,40,40,40,40,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,44,43,43,43,43,43,43,43,43,43,43,43,43,
43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,44,43,43,43,43,43,43,43,43,43,43,43,43,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP
};

int regresa_estado(char c,int estado_actual) {
    int col = 1;
    if ((c>= 97 && c<=122) || (c>= 65 && c<=90)) {
        col = 0;
    }
    if (c>= 49 && c<=57) {
        col = 2;
    }
    switch (c) {
        case 32: col = 33; break;
        case '_': col=1; break;
        case '0': col=3; break;
        case 'l': case 'L': col=4; break;
        case 'b': case 'B': col=5; break;
        case 'o': case 'O': col=6; break;
        case 'x': case 'X': col=7; break;
        case 'e': case 'E': col=8; break;
        case 'j': case 'J': col=9; break;
        case '.': col=10; break;
        case '+': col=11; break;
        case '-': col=12; break;
        case '%': col=13; break;
        case '*': col=14; break;
        case '/': col=15; break;
        case '<': col=16; break;
        case '>': col=17; break;
        case '=': col=18; break;
        case '!': col=19; break;
        case 39: col=20; break;
        case 34: col=21; break;
        case '#': col=22; break;
        case 64: col=23; break;
        case 123: col=24; break;
        case 125: col=25; break;
        case 44: col=26; break;
        case 91: col=27; break;
        case 93: col=28; break;
        case 40: col=29; break;
        case 41: col=30; break;
        case 58: col=31; break;
        case 59: col=32; break;
    }
    if (estado_actual == 5 || estado_actual == 6) {
        if (c >= 49 && c <= 55) {
            col = 6;
        }
    }//oct
    else if (estado_actual == 7 || estado_actual == 8) {
        if ((c >= 49 && c <= 57) || (c >= 97 && c <= 102) || (c >= 65 && c <= 70)) {
            col = 8;
        }
    }//hex
    else if (estado_actual == 9 || estado_actual == 10) {
        if (c == 49) {
            col = 10;
        }
    }//bin
    return estados[estado_actual][col];
}

void agarra_token(int estado) {
    switch (estado) {
        case 1: clasifica_identificador(lexema); break;
        case 2: strcpy(token,"ENTERO"); break;
        case 3: case 21: case 22: case 23: strcpy(token,"LARGO"); break;
        case 6: strcpy(token,"OCTAL"); break;
        case 8: strcpy(token,"HEX"); break;
        case 10: strcpy(token,"BINARIO"); break;
        case 12: case 14: strcpy(token,"DECIMAL"); break; //FLOAT
        case 17: case 20: strcpy(token,"EXPONENTE"); break;
        case 24: case 25: strcpy(token,"IMAGINARIO"); break;
        case 26: case 27: case 29: case 30: strcpy(token,"RELACIONAL"); break;
        case 28: strcpy(token,"ASIGNACION"); break;
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: strcpy(token,"ARITMETICO"); break;
        case 41: case 44: strcpy(token,"LITERAL"); break; //STRING
        case 45: strcpy(token,"DELIMITADOR"); break;
    }
    printf("%s\n",token);
}


void lexico() {
    int estado_actual = 0, estado_anterior = 0;
    int i=0;
    char cadena_auxiliar[50] = "";
    strcpy(lexema,"");
    while (estado_actual != ERR && estado_actual != ACP && arreglo[indx] != EOF) {
        estado_anterior = estado_actual;
        estado_actual = regresa_estado(arreglo[indx],estado_actual);
        printf("Simbolo %c, estado : %i\n",arreglo[indx],estado_actual);
        cadena_auxiliar[i] = arreglo[indx];
        if (arreglo[indx] == 32)
            indx++;
        indx++;
        i++;
    }
    printf("\nEstado: %i\n",estado_actual);
    if (estado_actual == ACP) {
        cadena_auxiliar[i-1] = '\0';
        strcpy(lexema,cadena_auxiliar);
        agarra_token(estado_anterior);
        estado_actual = 0;
        indx--;
    }
    if (arreglo[indx] == EOF) {
        agarra_token(estado_actual);
    }
}
void analisis_lexico() {
    int i=0;
    while(arreglo[i] != EOF) {
        printf("%c",arreglo[i]);
        i++;
    }
    printf("%c",'\n');
    while(arreglo[indx] != EOF) {
        lexico();
    }
    printf("\nultimo token: %s\n\n",token);
}







void main () {
    char caracter = 0;
    int i=0;
    printf("\n\n\n\n");
    FILE *archivo_entrada;
    archivo_entrada = fopen ("lexico.txt","r");
    if (archivo_entrada!=NULL) {
        while (caracter != EOF) {
            caracter = fgetc (archivo_entrada);
            arreglo[i]= caracter;
            i++;
        }
        arreglo[i] = EOF;
        i++;
        fclose (archivo_entrada);
    }
    analisis_lexico();
}
