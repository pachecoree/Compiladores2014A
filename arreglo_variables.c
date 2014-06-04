#include <stdio.h>
#include <stdlib.h>


typedef struct arreglo {
    void *variable;
    char tipo[20];
    struct arreglo *sig;
}arreglo_e;

int menu () {
    int opcion = 0;
    system("cls");
    printf("Tipo a declarar\n\n");
    printf("1-Caracter\n2-Cadena\n3-Entero\n4-Decimal\n5-Mostrar Arreglo\n6-Salir : ");
    scanf("%i",&opcion);
    return opcion;
}




void main () {
    arreglo_e *arre;
    arre = NULL;
    int opcion;
    do {
        opcion = menu();
        arreglo_e *indice;
        indice = arre;
        switch(opcion) {
            case 1:
                char c;
                printf("\n\nIngresa caracter");
                fflush(stdin);
                scanf("%c",&c);
                arreglo_e *aux =(arreglo_e*)malloc(sizeof(arreglo_e));
                strcpy(aux->tipo,"caracter");
                aux->variable = c;
                aux->sig = NULL;
                if (indice == NULL) {
                    indice = aux;
                }
                else {
                    while (indice -> sig != NULL) {
                        indice = indice->sig;
                    }
                    indice -> sig = aux;
                }
                break;
            case 2:
                char cad[299];
                printf("\n\nIngresa cadena");
                fflush(stdin);
                gets(cad);
                arreglo_e *aux =(arreglo_e*)malloc(sizeof(arreglo_e));
                strcpy(aux->tipo,"cadena");
                aux->variable = cad;
                aux->sig = NULL;
                if (indice == NULL) {
                    indice = aux;
                }
                else {
                    while (indice -> sig != NULL) {
                        indice = indice->sig;
                    }
                    indice -> sig = aux;
                }
                break;
            case 3:
                int num;
                printf("\n\nIngresa cadena");
                scanf("%i",&num);
                arreglo_e *aux =(arreglo_e*)malloc(sizeof(arreglo_e));
                strcpy(aux->tipo,"entero");
                aux->variable = num;
                aux->sig = NULL;
                if (indice == NULL) {
                    indice = aux;
                }
                else {
                    while (indice -> sig != NULL) {
                        indice = indice->sig;
                    }
                    indice -> sig = aux;
                }
                break;
            case 4:
                float num;
                printf("\n\nIngresa cadena");
                scanf("%i",&num);
                arreglo_e *aux =(arreglo_e*)malloc(sizeof(arreglo_e));
                strcpy(aux->tipo,"decimal");
                aux->variable = num;
                aux->sig = NULL;
                if (indice == NULL) {
                    indice = aux;
                }
                else {
                    while (indice -> sig != NULL) {
                        indice = indice->sig;
                    }
                    indice -> sig = aux;
                }
                break;
            case 5:
                while (indice != NULL) {
                    printf("\nTipo : %s\n",indice -> tipo);
                    if (strcmp(indice->tipo,"caracter")==0) {
                        printf("%c",indice -> variable);
                    }
                    else if (strcmp(indice->tipo,"cadena")==0) {
                        printf("%s",indice -> variable);
                    }
                    else if (strcmp(indice->tipo,"entero")==0) {
                        printf("%i",indice -> variable);
                    }
                    else if (strcmp(indice->tipo,"decimal")==0) {
                        printf("%.2f",indice -> variable);
                    }
                    else
                        printf("\nError!\n");
                    indice = indice->sig;
                }
                getch();
                break;
        }
    }while (opcion != 6);
}
