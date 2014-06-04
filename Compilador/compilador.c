#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <conio.h>

#define ACP 99
#define ERR -1

//prototipos
void expr();
void bloque();
void funcs();
void lfunc();

char lexema[40] = "";
char lex[1225] ="";
char token[20];
int indx=0;
char entrada[800000];
int numero_reglon = 0;
int numero_columna = 0;
int caracteres_archivo = 0;
int b_principal = 0;
int error = 0 ;

int indice_tabsim = 0;

struct tabla_simbolos {
    char nombre[256]; //(Nombre de la funcion, variable, constante)
    int tipo; //(1. Entero , 2. Decimal , 3. Literal, 4. Booleano)
    int clase; //(1. Variable , 2. Funcion , 3. Constante)
    int dimensiones[2]; //([0][0] - Variable, [1][0] - Arreglo, [1][1] - Matriz
    int regresa; //(0. Void , 1. ENTERO , 2. Decimal , 3. Literal
}tabsim[10000];


char palabras_reservadas[31][12] = {
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
    "sino",
    "regresa",
    "del",
    "importar",
    "prueba",
    "lee",
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
    "literal"
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

//Tabla de Transciciones
int estados[34][32] = {
/*00*/        1,1,4,9,1,1,10,24,23,22,25,26,18,20,19,17,14,11,33,10,10,10,10,10,10,10,10,10,10,ACP,ACP,ACP,
/*01*/        2,2,2,2,2,2,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*02*/        2,2,2,2,2,2,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*03*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*04*/        ACP,ACP,4,4,ACP,ACP,6,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*05*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*06*/        ERR,ERR,7,7,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
/*07*/        ACP,ACP,7,7,ACP,29,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*08*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*09*/        ACP,ACP,4,4,ACP,ACP,6,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*10*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*11*/        12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
/*12*/        12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
/*13*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*14*/        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
/*15*/        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
/*16*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*17*/        ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,21,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
/*18*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,21,21,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*19*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,21,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*20*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,21,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*21*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*22*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*23*/        ACP,ACP,4,9,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*24*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*25*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,28,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*26*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,27,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*27*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*28*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*29*/        ERR,ERR,31,31,ERR,ERR,ERR,30,30,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,
/*30*/        ACP,ACP,31,31,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*31*/        ACP,ACP,31,31,32,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*32*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
/*33*/        33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,ACP
        };
//



void clasifica_identificador(char iden[]) {
    int i;
    for(i=0; i<31;i++) {
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

int colcar (char c) {
    int columna=ERR;
    if ( c >= 65 && c <= 90) {
        if (c == 67) {
            columna=4;
        }
        else if ( c == 69) {
            columna=5;
        }
        else
            columna = 0;
    }
    else if ( c >= 97 && c <= 122) {
        if (c == 99) {
            columna=4;
        }
        else if ( c == 101) {
            columna=5;
        }
        else
            columna = 0;
    }
    else if ( c >= 49 && c <= 57) {
        columna = 2;
    }
    switch (c) {
        case 95: columna = 1; break;
        case 48: columna = 3; break;
        case 46: columna = 6; break;
        case 43: columna = 7; break;
        case 45: columna = 8; break;
        case 37: columna = 9; break;
        case 42: columna = 10; break;
        case 47: columna = 11; break;
        case 60: columna = 12; break;
        case 62: columna = 13; break;
        case 61: columna = 14; break;
        case 33: columna = 15; break;
        case 39: columna = 16; break;
        case 34: columna = 17; break;
        case 35: columna = 18; break;
        case 64: columna = 19; break;
        case 123: columna = 20; break;
        case 125: columna = 21; break;
        case 44: columna = 22; break;
        case 91: columna = 23; break;
        case 93: columna = 24; break;
        case 40: columna = 25; break;
        case 41: columna = 26; break;
        case 58: columna = 27; break;
        case 59: columna = 28; break;
        case ' ': columna = 29; break;
        case '\t': columna = 30; break;
    }
    return columna;
}

void agarra_token(int estado,char lexema[]) {
    switch (estado) {
        case 1: case 2: case 3: clasifica_identificador(lexema); break;
        case 4: case 5: case 9: strcpy(token,"ENTERO"); break; //INT //ENTERO
        case 7: case 8: case 32: case 30: case 31: strcpy(token,"DECIMAL"); break; //FLOAT //DECIMAL
        case 20: case 21: case 18: strcpy(token,"RELACIONAL"); break;
        case 19: strcpy(token,"ASIGNACION"); break;
        case 22: case 23: case 24: case 25: case 26: case 27: case 28: strcpy(token,"ARITMETICO"); break;
        case 16: case 13: strcpy(token,"LITERAL"); break; //STRING //LITERAL
        case 10: strcpy(token,"DELIMITADOR"); break;
        case 33: strcpy(token,"COMENTARIO"); break; //COMENTARIO
    }
}

void parse() {
    char lexema[50] = "";
    strcpy(token,"");
    strcpy(lex,"");
    int indx_lexema = 0;
    int col=0;
    int estado=0,estado_ant = 0;
    char c = EOF;
    int bandera=0;
        while ( entrada[indx] != EOF && estado != ERR && estado != ACP) {
            c = entrada[indx++];
            numero_columna++;
            //if (c == '\n' && (estado != 16 || estado != 13)) {
            if (c == '\n') {
                numero_reglon++;
                numero_columna=0;
            }
            if (estado == 33) {
                while ( c != '\n') {
                    lexema[indx_lexema++] = c;
                    lexema[indx_lexema] = '\0';
                    bandera=1;
                    c = entrada[indx++];
                }
            }
            while ( estado == 0 && ( c == ' ' || c == '\t')) {
                c = entrada[indx++];
                numero_columna++;
            }
            /*if ((estado == 4 || estado == 7) &&c == ' ') { // quitar en caso de que la constante no acepte espacio entre variable y la 'c'
                c = entrada[indx++];
            }
            else */if ( (c == ' ' || c == '\t'  || c == '\n') && estado > 0 && (estado != 11 && estado != 12 && estado != 14 && estado != 15)) {
                estado = ACP;
            }
            col = colcar(c);
            if ( col >= 0 && estado != ACP && estado != ERR) {
                estado_ant = estado;
                estado = estados[estado][col];
                if ( estado != ACP && estado != ERR) {
                    lexema[indx_lexema++] = c;
                    lexema[indx_lexema] = '\0';
                    bandera=1;
                }
            }
            if ( estado == ACP || estado == ERR) {
                indx--;
                numero_columna--;
            }
            if (estado != ACP && estado != ERR ) estado_ant = estado;
        }
        agarra_token(estado_ant,lexema);
        if (bandera==1 ) {
            strcpy(lex,lexema);
            //printf("Estado %i|Lexema : %s |Token: %s\n",estado_ant,lexema,token);
        }
        if (strcmp(token,"COMENTARIO") == 0) {
            parse();
        }
}


void agregar_tabsim(char nombre[],int tipo,int clase,int dimensiones[],int regresa) {
    strcpy(tabsim[indice_tabsim].nombre,nombre);
    tabsim[indice_tabsim].tipo = tipo;
    tabsim[indice_tabsim].clase = clase;
    tabsim[indice_tabsim].dimensiones[0] = dimensiones[0];
    tabsim[indice_tabsim].dimensiones[1] = dimensiones[1];
    tabsim[indice_tabsim++].regresa = regresa;
}


int cte() {
    if (strcmp(token,"LITERAL") == 0) return 1;
    if (strcmp(token,"ENTERO") == 0) return 1;
    if (strcmp(token,"DECIMAL") ==0) return 1;
    if (strcmp(token,"BOOLEANO")==0) return 1;
    return 0;
}

int get_tipo() {
    if (strcmp(token,"LITERAL") == 0) return 3;
    if (strcmp(token,"ENTERO") == 0) return 1;
    if (strcmp(token,"DECIMAL") ==0) return 2;
    if (strcmp(token,"BOOLEANO")==0) return 4;
}

int arreglo() {
    if ( strcmp(lex,"[") == 0) {
        parse();
        if (cte()) {
            parse();
            while (strcmp(lex,",") == 0) {
                parse();
                if (cte()) {
                    parse();
                }
                else {
                    error++;
                    printf("\nSe esperaba >> 'Constante' << se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
                }
            }
        }
        else if ( strcmp(lex,"[") == 0) {
            parse();
            if (cte()) {
                parse();
            }
            if (strcmp(lex,"]") == 0) {
                parse();
            }
            else { error++;
                printf("\nSe esperaba >> ']' << se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
                return 0;
            }
        }
        if ( strcmp(lex,"]") == 0) {
            return 1;
        }
        else { error++;
            printf("\nSe esperaba >> ']' << se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
            return 0;
        }
    }
    return 0;
}


void vars() {
    while ( strcmp(token,"IDENTIFICADOR") == 0) {
        char nombre[1000][256];
        int tipo;
        int clase = 1;
        int dimensiones[2] = {0,0};
        int regresa = 0;
        int i;
        int indice_iden = 0;
        strcpy(nombre[indice_iden++],lex);
        parse();
        if ( strcmp(lex,"=") == 0) {
            parse();
            while (strcmp(token,"IDENTIFICADOR") == 0) {
                strcpy(nombre[indice_iden++],lex);
                parse();
                if (strcmp(lex,"=") == 0) {
                    parse();
                }
                else { error++;
                    printf("\nError : Se esperaba >> '=' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
                }
            }
            if (cte()) {
                tipo = get_tipo();
                printf("\nDeclaracion de variable : %s",lex);
                parse();
            }
            else if (arreglo()) {
                tipo = get_tipo();
                printf("\nDeclaracion de arreglo");
                parse();
            }
            else { error++;
                printf("\nError : Se esperaba Constante o arreglo");
            }
            if (strcmp(lex,"c") == 0) {
                clase = 3;
                printf(" es constante");
                parse();
            }
            for (i=0; i<indice_iden ; i++) {
                agregar_tabsim(nombre[i],tipo,clase,dimensiones,regresa);
            }
        }
        else { error++;
            printf("\nError : Se esperaba >> '=' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
    }
}

void dimens() {
    do {
        if (strcmp(lex,"[") ==0) {
            parse();
            expr();
            if (strcmp(lex,"]")==0){
                parse();
            }
            else { error++;
                printf("\nError : Se esperaba >> ']' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
            }
        }
   } while(strcmp(lex,"[")==0);
}

void asigna() {
    if (strcmp(token,"IDENTIFICADOR")==0) {
        parse();
        if (strcmp(lex,"(")==0) {
            lfunc();
            return;
        }
        if (strcmp(lex,"[")==0) {
            dimens();
        }
        if (strcmp(lex,"=")==0) {
            parse();
        }
        else { error++;
             printf("\nError : Se esperaba >> '=' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        expr();
    }
}

void lfunc() {
    if (strcmp(lex,"(")==0) {
        parse();
        if (strcmp(lex,")")==0) {
            parse();
            return;
        }
        expr();
        while(strcmp(lex,",")==0) {
            parse();
            expr();
        }
        if (strcmp(lex,")")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> ')' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
    }
}


void termino() {
    if (strcmp(lex,"(")==0) {
        parse();
        expr();
        if (strcmp(lex,")")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> ')' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
    }
    else if (strcmp(token,"BOOLEANO")==0){
        parse();
    }
    else if (cte()) {
        parse();
    }
    else if (strcmp(token,"IDENTIFICADOR")==0) {
        parse();
        if (strcmp(lex,"[")==0) {
            dimens();
        }
        else if (strcmp(lex,"(")==0) {
            lfunc();
        }
    }
    else { error++;
        printf("\nError : Se esperaba constante o booleano o funcion o parentesis y se encontro  >> %s <<",lex);
    }
}

void ope_signo() {
    if (strcmp(lex,"-")==0) {
        parse();
    }
    termino();
}

void ope_pot() {
    do {
        if (strcmp(lex,"**")==0) {
            parse();
        }
        ope_signo();
    } while (strcmp(lex,"**")==0);
}

void ope_mul() {
    do {
        if (strcmp(lex,"*")==0 || strcmp(lex,"/")==0 || strcmp(lex,"%")==0 || strcmp(lex,"//")==0) {
            parse();

        }
        ope_pot();
    } while (strcmp(lex,"*")==0 || strcmp(lex,"/")==0 || strcmp(lex,"%")==0 || strcmp(lex,"//")==0);
}

void ope_sum() {
    do {
        if (strcmp(lex,"+")==0 || strcmp(lex,"-")==0) {
            parse();
        }
        ope_mul();
    } while (strcmp(lex,"+")==0 || strcmp(lex,"-")==0 );
}


void ope_rel() {
    ope_sum();
        if ((strcmp(lex,"<") == 0) || (strcmp(lex,">") == 0) || (strcmp(lex,"<=") == 0) || (strcmp(lex,">=") == 0) ||
            (strcmp(lex,"==") == 0) || (strcmp(lex,"!=") == 0)) {

            parse();
            ope_sum();
        }
}


void ope_no() {
    if (strcmp(lex,"no")==0) {
        parse();
    }
    ope_rel();

}


void ope_y() {
    do {
        if (strcmp(lex,"y")==0){
            parse();
        }
        ope_no();

    } while (strcmp(lex,"y")==0);
}


void expr() {
    do {
        if (strcmp(lex,"o") == 0) {
            parse();
        }
        ope_y();
    } while (strcmp(lex,"o") == 0);
}


void estatuto_si() {
    if (strcmp(lex,"si")==0) {
        printf("\nEstatuto SI");
        parse();
        expr();
        bloque();
        while (strcmp(lex,"otsi")==0) {
            printf("\nEstatuto OTSI");
            parse();
            expr();
            bloque();
        }
        if (strcmp(lex,"sino")==0) {
            printf("\nEstatuto SINO");
            parse();
            bloque();
        }
    }
}

void estatuto_regresa() {
    if (strcmp(lex,"regresa")==0) {
        printf("\nEstatuto REGRESA");
        parse();
        expr();
    }
}


void lexp() {
    if (arreglo()) {
        parse();
        return;
    }
    expr();
    if (strcmp(lex,".")==0) {
        parse();
    }
    else { error++;
        printf("\nError : Se esperaba >> '.' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
    }
    if (strcmp(lex,".")==0) {
        parse();
    }
    else { error++;
        printf("\nError : Se esperaba >> '.' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
    }
    expr();
    printf("\n**%s,%s**\n",token,lex);
    if (strcmp(lex,"pasa")==0) {
        parse();
        if (strcmp(token,"ENTERO")==0 || strcmp(token,"DECIMAL")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> 'Entero' o 'Decimal' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
        }
    }
}



void estatuto_desde() {
    if (strcmp(lex,"desde")==0) {
        parse();
        expr();
        if (strcmp(lex,"en")==0) {
            parse();
        }
        else { error++;
                printf("\nError : Se esperaba >> 'en' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        lexp();
        bloque();
        printf("\nEstatuto DESDE");
    }
}


void estatuto_mientras() {
    if (strcmp(lex,"mientras")==0) {
        parse();
        expr();
        bloque();
    }
}

void estatuto_imprime() {
    if (strcmp(lex,"imprime")==0) {
        parse();
        if (strcmp(lex,"(")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> '(' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        expr();
        while (strcmp(lex,",")==0) {
            parse();
            expr();
        }
        if (strcmp(lex,")")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> ')' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
    }
}

void estatuto_lee() {
    if (strcmp(lex,"lee")==0) {
        parse();
        if (strcmp(lex,"(")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> '(' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        expr();
        if (strcmp(lex,")")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> ')' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
    }
}

void estatuto_interrumpe() {
    if (strcmp(lex,"interrumpe")==0) {
        parse();
    }
}

void estatuto_continua() {
    if (strcmp(lex,"continua")==0) {
        parse();
    }
}

int comando() {
    if (strcmp(lex,"si")==0) {
        estatuto_si();
    }
    else if (strcmp(lex,"desde") == 0) {
        estatuto_desde();
    }
    else if (strcmp(lex,"regresa")==0) {
        estatuto_regresa();
    }
    else if (strcmp(lex,"mientras")==0) {
        estatuto_mientras();
    }
    else if (strcmp(lex,"imprime")==0) {
        estatuto_imprime();
    }
    else if (strcmp(lex,"lee")==0) {
        estatuto_lee();
    }
    else if (strcmp(lex,"interrumpe")==0) {
        estatuto_interrumpe();
    }
    else if (strcmp(lex,"continua")==0) {
        estatuto_continua();
    }
    else if (strcmp(token,"IDENTIFICADOR")==0) {
        asigna();
    }
    else {
        return 0;
    }
    return 1;
}

void estatutos() {
    int comando_regreso = 0;
    do {
        comando_regreso = comando();
    }while (comando_regreso == 1);
}

void bloque() {
    if ( strcmp(lex,":") == 0) {
        parse();
    }
    else { error++;
        printf("\nError : Se esperaba >> ':' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
    }
    estatutos();
    if (strcmp(lex,".") == 0) {
        parse();
    }
    else { error++;
        printf("\nError : Se esperaba >> '.' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
    }
}


void param() {
    if (strcmp(lex,")")==0)
        return;
        if (strcmp(token,"IDENTIFICADOR")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
        }
        if (strcmp(lex,"=")==0) {
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> '=' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        if (cte()) {
            printf("\nParametro %s",lex);
            parse();
        }
        else if (arreglo()) {
            printf("\nParametro %s",lex);
            parse();
        }
        else { error++;
            printf("\nError : Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
        }
        while (strcmp(lex,",")==0) {
            parse();
            if (strcmp(token,"IDENTIFICADOR")==0) {
                parse();
            }
            else { error++;
                printf("\nError : Se esperaba >> 'IDENTIFICADOR' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
            }
            if (strcmp(lex,"=")==0) {
                parse();
            }
            else { error++;
                printf("\nError : Se esperaba >> '=' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
            }
            if (cte()) {
                parse();
            }
            else if (arreglo()) {
                parse();
            }
            else { error++;
                printf("\nError : Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
            }
        }
}


void prgm() {
    parse();
    vars();
    funcs();
    if (b_principal == 0) { error++;
        printf("\nError: Se esperaba funcion principal pero no fue encontrada!" );
    }
    if (strcmp(token,"") != 0 ) { error++;
        printf("\nUnexpected >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
    }
}


void funcs() {
    while (strcmp(lex,"def")==0) {
        parse();
        if (b_principal == 0 && strcmp(lex,"principal")==0) {
            b_principal = 1; // si hay principal
        }
        else if (strcmp(token,"IDENTIFICADOR")==0) {
            //parse();
        }
        else {
            printf("\nError : Se esperaba >> 'IDENTIFICADOR' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",token,numero_reglon-1,numero_columna);
        }
        parse();
        if (strcmp(lex,"(")==0) {
            parse();
        }
        else {
            printf("\nError : Se esperaba >> '(' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        param();
        if (strcmp(lex,")")==0) {
            parse();
        }
        else {
            printf("\nError : Se esperaba >> ')' << pero se encontro >> '%s' << en el renglon '%i',columna '%i'",lex,numero_reglon-1,numero_columna);
        }
        if (cte()) {
            parse();
        }
        vars();
        bloque();
    }
}



void mostrar_variables() {
    int i;
    printf("Variables Declaradas : ");
    for (i=0; i< indice_tabsim; i++) {
        printf("\n\nNombre : %s",tabsim[i].nombre);
        printf("\nTipo : %i",tabsim[i].tipo);
        printf("\nClase : %i",tabsim[i].clase);
        printf("\nDimensiones : [%i][%i]",tabsim[i].dimensiones[0],tabsim[i].dimensiones[1]);
        printf("\nRegresa: %i",tabsim[i].regresa);
    }
}

int main () {
    char caracter = 0;
    int i=0 , j=0;
    char lexema[50];
    FILE *archivo_entrada;
    archivo_entrada = fopen ("lexico.txt","r");
    if (archivo_entrada!=NULL) {
        while (caracter != EOF) {
            caracter = fgetc (archivo_entrada);
            entrada[i]= caracter;
            i++;
        }
        caracteres_archivo = i;
        entrada[i] = EOF;
        fclose (archivo_entrada);
    }
    int renglon = 1;
    printf("[%i]\t",renglon++);
    for (j=0; j<i ; j++) {
        if(entrada[j] == '\n') {
            printf("\n[%i]\t",renglon++);
        }
        else printf("%c",entrada[j]);
    }
    //printf("\n%s\n\n\n",entrada);
    printf("\n\n\n");
    prgm();
    if (error == 0 ) printf("\n\n\nAnalisis Completo\n\n\n ");
    else printf("\n\n\nNo es pudo compilar programa debido a %i errores\n\n\n",error);
    //mostrar_variables();
    return 0;
}
