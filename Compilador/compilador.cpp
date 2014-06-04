#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <cstring>
#include <map>

using namespace std;

const int ACP = 99;
const int ERR = -1;
//prototipos
void expr();
void bloque();
void funcs();
void lfunc();
ofstream Salida;
string expr_asigna="";
string lfunc(string nombre_func);
string expresion = "";
string lexema = "";
string lex="";
string token = "";
int indx=0;
string contexto = "";
int contexto_regresa = 0;
char entrada[800000];
int inicio_palabra_ant = 0;
int numero_reglon = 0;
int numero_columna = 0;
int caracteres_archivo = 0;
int b_principal = 0;
int b_ciclo = 0;
int b_teclado = 0; //para entrada de telcado
string e_teclado = "";
int error = 0 ;
int cont_E = 1; //contador de etiqueta
int cont_L = 1; // contador de linea codigo
stack<string> c_rompe_ciclo;
stack<string> c_n;

struct tabsim {
    string valor;
    string existe;
    string tipo;
    string dato;
    int dimension1;
    int dimension2;
    int linea; //Linea de declaracion
};

struct def_funcion {
    string retorno;
    int dimension1;
    int linea;
};

struct codigo {
    string ins;
    string dir1;
    string dir2;
}cod[10000];

map<string, tabsim> TABSIM; //mapa de simbolos
map<string, def_funcion> TABDEF; //mapa de funciones

//Elementos de un arreglo
int dimension1_arreglo = 0;
int dimension2_arreglo = 0;
string dato_arreglo = "";
string valor_arreglo = "";
string tipo_arreglo = "";

tabsim tabsim_element_creator() {
    tabsim element;
    element.valor="";
    element.existe="verdad";
    element.tipo = "";
    element.dato = "";
    element.dimension1 = 0;
    element.dimension2 = 0;
    element.linea = 0;
    return element;
}

string palabras_reservadas[] = {
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

string constantes[] = {
    "entero",
    "flotante",
    "literal"
};

string operadores_bool[] = {
    "verdadero",
    "falso"
};

string operadores_logicos[] = {
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
/*23*/        ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,
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
stack<string> pila_tipos;
const string cTipo[] = {  "E=E", "B=B", "D=D", "L=L", "D=E",
                          "E+E", "E+D", "D+E", "D+D", "L+L",
                          "E-E", "E-D", "D-E", "D-D",
                          "E*E", "E*D", "D*E", "D*D",
                          "E**E","D**E",
                          "E/E", "E/D", "D/E", "D/D",
                          "E//E","E//D","D//E",
                          "E%E", "-E", "-D",
                          "ByB", "BoB", "BnoB",
                          "E>E", "R>E", "E>D", "D>D",
                          "E<E", "D<E", "E<D", "D<D",
                          "E>=E", "D>=E", "E>=D", "D>=D",
                          "E<=E", "D<=E", "E<=D", "D<=D",
                          "E!=E", "D!=E", "E!=D", "D!=D", "L!=L",
                          "L==L", "D==E", "E==D", "D==D", "E==E"
};

const string tipoR[] = { "E", "B", "D", "L", "D",//asignacion
                        "E", "D", "D", "D", "L",//suma
                        "E", "D", "D", "D",//resta
                        "E", "D", "D", "D",//multiplicacion
                        "E","D",//potencia
                        "D", "D", "D", "D",//division
                        "E","D","D",//doble division
                        "E", "E", "D",//modulo,signo
                        "B", "B", "B",//logicos
                        "B", "B", "B", "B",//relacionales
                        "B", "B", "B", "B",
                        "B", "B", "B", "B",
                        "B", "B", "B", "B",
                        "B", "B", "B", "B", "B",
                        "B", "B", "B", "B", "B"
};

int buscaTipo(string cadt) {
    for(int i=0; i < 59; i++)
        if(cTipo[i]==cadt) return i;
    return -1;
}

bool en_TABSIM(string value) {
    string boleano = TABSIM[value].existe;
    if (boleano == "verdad") { // existe en map
        return true;
    }
    return false; // no Existe en map
}

bool en_TABDEF(string value) {
    string boleano = TABDEF[value].retorno;
    if (boleano != "") { // existe en map
        return true;
    }
    return false; // no Existe en map

}

string regresa_identificador(string identificador) {
    string cadena = identificador.substr(identificador.find("$")+1);
    return cadena;
    //la funcion identificador(int, int, int) no esta declarada
}

string regresa_funcion (string identificador) {
    if (identificador.find("%") == string::npos) return identificador+"()";
    string cadena = identificador.substr(0,identificador.find("%"));
    string parametros = "(";
    string aux = identificador.substr(identificador.find("%"),identificador.find("%")+1);
    int i=1;
    while (aux.length() > i) {
        char _aux = aux.at(i);
        if (_aux == 'E') parametros = parametros + "ENTERO,";
        if (_aux == 'B') parametros = parametros + "BOOLEANO,";
        if (_aux == 'D') parametros = parametros + "DECIMAL,";
        if (_aux == 'L') parametros = parametros + "LITERAL,";
        i+=2;
    }
    parametros.at(parametros.length()-1) = ')';
    cadena = cadena + parametros;
    return cadena;
}

void agregar_codigo(string ins,string dir1,string dir2) {
    cod[cont_L].ins = ins;
    cod[cont_L].dir1 = dir1;
    cod[cont_L++].dir2 = dir2;
}

void agregar_funcion(def_funcion elemento, string nombre ) {
    if (!en_TABDEF(nombre)) {
        TABDEF[nombre] = elemento;
    }
    else {
        error++;
        cout << "Error Linea: "<< numero_reglon <<" Funcion '"<< regresa_funcion(nombre) << "' ya fue declarada previamente en la linea : "<< TABDEF[nombre].linea << endl;
    }
}

void agregar_identificador(tabsim elemento, string nombre) {
    if (!en_TABSIM(nombre)) {
        TABSIM[nombre] = elemento;
    }
    else {
        error++;
        string iden = nombre.substr(nombre.find("$")+1);
        cout << "Error Linea: "<< numero_reglon <<" Identificador '"<< regresa_identificador(iden) << "' ya fue declarado previamente en la linea : "<< TABSIM[nombre].linea << endl;
    }
}

void clasifica_identificador(string iden) {
    int i;
    for(i=0; i<31;i++) {
        if( iden == palabras_reservadas[i]) {
            token = "RESERVADA";
            return;
        }
    }
    for(i=0; i<3;i++) {
        if( iden == constantes[i]) {
            token = "CONSTANTE";
            return;
        }
    }
    for(i=0; i<2;i++) {
        if( iden == operadores_bool[i]) {
            token = "BOOLEANO";
            return;
        }
    }
    for(i=0; i<3;i++) {
        if( iden == operadores_logicos[i]) {
            token = "LOGICO";
            return;
        }
    }
    token = "IDENTIFICADOR";
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

void agarra_token(int estado,string lexema) {
    switch (estado) {
        case 1: case 2: case 3: clasifica_identificador(lexema); break;
        case 4: case 5: case 9: token = "ENTERO"; break; //INT //ENTERO
        case 7: case 8: case 32: case 30: case 31: token = "DECIMAL"; break; //FLOAT //DECIMAL
        case 20: case 21: case 18: token = "RELACIONAL"; break;
        case 19: token = "ASIGNACION"; break;
        case 22: case 23: case 24: case 25: case 26: case 27: case 28: token = "ARITMETICO"; break;
        case 16: case 13: token = "LITERAL"; break; //STRING //LITERAL
        case 10: token = "DELIMITADOR"; break;
        case 33: token = "COMENTARIO"; break; //COMENTARIO
    }
}

void parse() {
    string lexema = "";
    token = "";
    lex = "";
    int indx_lexema = 0;
    int col=0;
    int estado=0,estado_ant = 0;
    char c;
    int bandera=0;
    inicio_palabra_ant = indx;
        while ( entrada[indx] != EOF && estado != ERR && estado != ACP) {
            c = entrada[indx++];
            if (estado == 0 && c == '\n') numero_reglon++;
            if (estado == 33) {
                while ( c != '\n') {
                    lexema+= c;
                    indx_lexema++;
                    bandera=1;
                    c = entrada[indx++];
                }
            }
            while ( estado == 0 && ( c == ' ' || c == '\t')) {
                c = entrada[indx++];
            }
            /*if ((estado == 4 || estado == 7) && c == ' ') { // quitar en caso de que la constante no acepte espacio entre variable y la 'c'
                c = entrada[indx++];
            }
            else */if ( (c == ' ' || c == '\t'  || c == '\n') && estado > 0 && (estado != 11 && estado != 12 && estado != 14 && estado != 15)) {
                //if (c == '\n') numero_reglon++;
                estado = ACP;
            }
            col = colcar(c);
            if ( col >= 0 && estado != ACP && estado != ERR) {
                estado_ant = estado;
                estado = estados[estado][col];
                if ( estado != ACP && estado != ERR) {
                    lexema+= c;
                    indx_lexema++;
                    bandera=1;
                }
            }
            if ( estado == ACP || estado == ERR) {
                indx--;
            }
            if (estado != ACP && estado != ERR ) estado_ant = estado;
        }
        agarra_token(estado_ant,lexema);
        if (bandera==1 ) {
            lex = lexema;
        }
        if (token == "COMENTARIO") {
            parse();
        }
        //cout << lex << endl;
}


int cte() {
    if (token == "LITERAL") return 1;
    if (token =="ENTERO") return 1;
    if (token =="DECIMAL") return 1;
    if (token =="BOOLEANO") return 1;
    return 0;
}

string int_to_string(int value) {
    string str;
    stringstream out;
    out << value;
    str = out.str();
    return str;
}

int arreglo() { //pendiente
    if ( lex == "[") {
        dimension1_arreglo = 1000;//1;
        dimension2_arreglo = 0;
        valor_arreglo = "";
        dato_arreglo = "";
        tipo_arreglo = "";
        parse();
        if (cte()) {
            string tipoarreglo = token;
            tipoarreglo = tipoarreglo.at(0);
            dato_arreglo = token;
            tipo_arreglo = "VARIABLE";
            valor_arreglo = lex;
            parse();
            while (lex == ",") {
                dimension1_arreglo=1000;//1;
                parse();
                if (cte()) {
                    string tipo_elemento = token;
                    tipo_elemento = tipo_elemento.at(0);
                    if (tipoarreglo != tipo_elemento) {
                        error++;
                        cout << "Conflicto en tipo de Elemento '" << dimension1_arreglo << "' se esperaba >" << tipoarreglo << "< se encontro >" << tipo_elemento << "<" << endl;
                    }
                    parse();
                }
                else {
                    error++;
                    cout <<"Se esperaba >> 'Constante' << se encontro >> '"<< token <<"' << en el renglon '" << numero_reglon << endl;
                }
            }
        }
        else if (lex == "[") {
            dimension2_arreglo = 1000;//1;
            parse();
            if (cte()) {
                valor_arreglo = lex;
                dato_arreglo = token;
                tipo_arreglo = "VARIABLE";
                parse();
            }
            if ( lex == "]") {
                parse();
            }
            else {
                error++;
                cout <<"Se esperaba >> ']' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
                return 0;
            }
        }
        if (lex == "]") {
            return 1;
        }
        else {
            error++;
            cout <<"Se esperaba >> ']' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
            return 0;
        }
    }
    return 0;
}


void vars(string prefijo) {
    prefijo+= '$';
    while ( token == "IDENTIFICADOR") {
        tabsim element = tabsim_element_creator();
        string arreglo_iden[10000];
        int id_iden = 0 , i = 0;
        string iden = lex;
        arreglo_iden[id_iden++] = prefijo+lex;
        element.linea = numero_reglon;
        parse();
        if ( lex == "=") {
            parse();
            stack<string> idens;
            idens.push(prefijo+iden);
            while (token == "IDENTIFICADOR") {
                idens.push(prefijo+lex);
                arreglo_iden[id_iden++] = prefijo+lex;
                parse();
                if (lex == "=") {
                    parse();
                }
                else { error++;
                    cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<< numero_reglon << endl;
                }
            }
            if (cte()) {
                element.valor = lex;
                element.dato = token;
                element.tipo = "VARIABLE";
                parse();
                while(! idens.empty()) {
                    string aux = idens.top();
                    agregar_codigo("LIT",element.valor,"0");
                    agregar_codigo("STO","0",aux);
                    idens.pop();
                }
            }
            else if (arreglo()) {
                element.tipo = tipo_arreglo;
                element.valor = valor_arreglo;
                element.dato = dato_arreglo;
                element.dimension1 = dimension1_arreglo;
                element.dimension2 = dimension2_arreglo;
                parse();
            }
            else { error++;
                cout <<"Error : Se esperaba Constante o arreglo" << endl;
            }
            if (lex == "c") {
                element.tipo = "CONSTANTE";
                parse();
            }
        }
        else { error++;
            cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        for (i=0;i<id_iden; i++) {
            agregar_identificador(element,arreglo_iden[i]);
        }
    }
}

void vars() {
    while ( token == "IDENTIFICADOR") {
        tabsim element = tabsim_element_creator();
        string arreglo_iden[10000];
        string iden = lex;
        int id_iden = 0 , i = 0;
        arreglo_iden[id_iden++] = "$"+lex;
        element.linea = numero_reglon;
        parse();
        if ( lex == "=") {
            parse();
            stack<string> idens;
            idens.push("$"+iden);
            while (token == "IDENTIFICADOR") {
                idens.push("$"+lex);
                arreglo_iden[id_iden++] = "$"+lex;
                parse();
                if (lex == "=") {
                    parse();
                }
                else { error++;
                    cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<< numero_reglon << endl;
                }
            }
            if (cte()) {
                element.valor = lex;
                element.dato = token;
                element.tipo = "VARIABLE";
                parse();
                while(! idens.empty()) {
                    string aux = idens.top();
                    agregar_codigo("LIT",element.valor,"0");
                    agregar_codigo("STO","0",aux);
                    idens.pop();
                }
            }
            else if (arreglo()) {
                element.tipo = tipo_arreglo;
                element.valor = valor_arreglo;
                element.dato = dato_arreglo;
                element.dimension1 = dimension1_arreglo;
                element.dimension2 = dimension2_arreglo;
                parse();
            }
            else { error++;
                cout <<"Error : Se esperaba Constante o arreglo" << endl;
            }
            if (lex == "c") {
                element.tipo = "CONSTANTE";
                parse();
            }
        }
        else { error++;
            cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        for (i=0;i<id_iden; i++) {
            agregar_identificador(element,arreglo_iden[i]);
        }
    }
}


void dimens() {
    do {
        if (lex == "[") {
            parse();
            expr();
            string Tipo = pila_tipos.top();
            if (Tipo != "E") {
                error++;
                cout << "Conflicto en tipos, se esperaba Entero se recibio >" <<Tipo << "< en la linea " << numero_reglon << endl;
            }
            if (lex == "]"){
                parse();
            }
            else { error++;
                cout <<"Se esperaba >> ']' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
            }
        }
   } while(lex == "[");
}

string dimens(string iden) { //pendiente
    int dimension = 0;
    string iden_f = contexto+iden;
    string iden_r = "";
    do {
        if (lex == "[") {
            dimension++;
            parse();
            expr();
            string Tipo = pila_tipos.top();
            if (Tipo != "E") {
                error++;
                cout << "Conflicto en tipos, se esperaba Entero se recibio >" <<Tipo << "< en la linea " << numero_reglon << endl;
            }
            if ( en_TABSIM(iden)) {
                iden_r = iden;
            }
            else if (en_TABSIM(iden_f)) {
                iden_r = iden_f;
            }
            else
                cout << "Identificador " << regresa_identificador(iden) << " no ha sido declarado" << endl;
            if (lex == "]"){
            }
            else { error++;
                cout <<"Se esperaba >> ']' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
            }
            parse();
        }
   } while(lex == "[");
    if (dimension == 1) {
        if (TABSIM[iden_r].dimension1 == 0) {
            error++;
                cout << "Identificador " << regresa_identificador(iden) << " no tiene dimensiones de arreglo" << endl;
        }
    }
    if (dimension == 2) {
        if (TABSIM[iden_r].dimension2 == 0) {
            error++;
                cout << "Identificador " << regresa_identificador(iden) << " no contiene dimensiones de matriz" << endl;
        }
    }
    else if (dimension > 2) {
        error++;
        cout << regresa_identificador(iden) << " Excede dimensiones permitidas" << endl;
   }
   if (en_TABSIM(iden_r)) {
       if (dimension==1) {
            if (TABSIM[iden].dimension2 > 0) {
                error++;
                cout << "A " << regresa_identificador(iden) << " le faltan dimensiones" << endl;
            }
       }
   }
   return iden_r;
}

void asigna() {
    if (token == "IDENTIFICADOR") {
        int i_aux = inicio_palabra_ant;
        string iden=lex;
        string iden_asigna="";
        string tipo_asigna = "I";
        string iden_originial = lex;
        string var_global = "$"+lex;
        string nombre_ide = contexto+"$"+lex;
        parse();
        if (lex == "[") {
            iden_asigna = dimens(var_global);
        }
        else if (lex == "(") {
            indx = i_aux;
            parse();
            expr();
            string dato = pila_tipos.top(); pila_tipos.pop();
            if (dato != "I") {
                error++;
                cout << "Error , Funcion regresa valor en la linea : " << numero_reglon << endl;
            }
            return;
        }
        else if (en_TABSIM(nombre_ide)) {
            if (TABSIM[nombre_ide].dimension1 > 0 || TABSIM[nombre_ide].dimension2 > 0) {
                error++;
                cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
            }
            if (TABSIM[nombre_ide].tipo == "CONSTANTE") {
                error++;
                cout << regresa_identificador(nombre_ide) << " es constante y no se puede modificar" << endl;
            }
            string dato = TABSIM[nombre_ide].dato;
            dato = dato.at(0);
            pila_tipos.push(dato);
            tipo_asigna = dato;
            iden_asigna=nombre_ide;
        }
        else if (en_TABSIM(var_global)) {
            if (TABSIM[var_global].dimension1 > 0 || TABSIM[var_global].dimension2 > 0) {
                error++;
                cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
            }
            if (TABSIM[var_global].tipo == "CONSTANTE") {
                error++;
                cout << regresa_identificador(nombre_ide) << " es constante y no se puede modificar" << endl;
            }
            string dato = TABSIM[var_global].dato;
            dato = dato.at(0);
            pila_tipos.push(dato);
            tipo_asigna = dato;
            iden_asigna = var_global;
        }
        else {
            error++;
            pila_tipos.push("I");
            cout << "Identificador " << regresa_identificador(var_global) << " no ha sido declarado" << endl;
        }
        if (lex != "=") {
            error++;
            cout << "Se esperaba >=< y se encontro >" << lex << "< en la linea : " << numero_reglon << endl;
        }
            tipo_asigna = pila_tipos.top(); pila_tipos.pop();
            parse();
            expr();
            string Tipo = pila_tipos.top();
            pila_tipos.pop();
            Tipo = tipo_asigna+"="+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto en tipos de asignacion >"<< Tipo
                    << "< en la linea " << numero_reglon << endl;
            }
            agregar_codigo("STO", "0", iden_asigna);
    }
}

void lfunc() {
    if (lex == "(") {
        expresion+=lex;
        parse();
        if (lex == ")") {
            expresion+=lex;
            parse();
            return;
        }
        expr();
        while(lex == ",") {
            expresion+=lex;
            parse();
            expr();
        }
        if (lex == ")") {
            expresion+=lex;
            parse();
        }
        else { error++;
            cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
    }
}

string lfunc(string nombre_func) {
    string tipo_parametro;
    if (lex == "(") {
        expresion+=lex;
        parse();
        if (lex == ")") {
            expresion+=lex;
            parse();
            return nombre_func;
        }
        expr();
        tipo_parametro = pila_tipos.top();
        nombre_func+= "%" + tipo_parametro;
        while(lex == ",") {
            expresion+=lex;
            parse();
            expr();
            tipo_parametro = pila_tipos.top();
            nombre_func+= "%" + tipo_parametro;
        }
        if (lex == ")") {
            expresion+=lex;
            parse();
        }
        else { error++;
            cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
    }
    return nombre_func;
}

void termino() {
    if (lex == "(") {
        expresion+=lex;
        parse();
        expr();
        if (lex == ")") {
            expresion+=lex;
            parse();
        }
        else { error++;
            cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
    }
    /*else if (token == "BOOLEANO"){
        expresion+=lex;
        parse();
    }*/
    else if (cte()) {
        expresion+=lex;
        if(token == "BOOLEANO") {
            pila_tipos.push("B");
            if (lex == "verdadero") agregar_codigo("LIT", "V", "0");
            else if (lex=="falso") agregar_codigo("LIT", "F", "0");
        }
        else if( token == "ENTERO") { pila_tipos.push("E"); agregar_codigo("LIT", lex, "0");  }
        else if( token == "DECIMAL") { pila_tipos.push("D"); agregar_codigo("LIT", lex, "0");}
        else if( token == "LITERAL") {
                pila_tipos.push("L");
                if (lex == "\"n\"" || lex == "'n'") agregar_codigo("LIT","\"\"","0");
                else agregar_codigo("LIT", lex, "0");
        }
        parse();
    }
    else if (token == "IDENTIFICADOR") {
        expresion+=lex;
        string iden_originial = lex;
        string var_global = "$"+lex;
        string nombre_ide = contexto+"$"+lex;
        string nombre_func = lex;
        parse();
        if (lex == "[") {
            nombre_ide = dimens(var_global);
            agregar_codigo("LOD",nombre_ide,"0");
        }
        else if (lex == "(") {
            tabsim elemento = tabsim_element_creator();
            elemento.dimension1=0; elemento.dimension2=0;
            elemento.dato = "I"; elemento.tipo="I";
            string _E = "_E" + int_to_string(cont_E++);
            agregar_codigo("LOD",_E,"0");
            nombre_func = lfunc(nombre_func);
            if (en_TABDEF(nombre_func)) {
                string dato = TABDEF[nombre_func].retorno;
                dato = dato.at(0);
                pila_tipos.push(dato);
                agregar_codigo("CAL",nombre_func,"0");
                elemento.dimension1 = cont_L;
                agregar_identificador(elemento,_E);
                if (dato != "I")
                    agregar_codigo("LOD",nombre_func,"0");
            }
            else {
                error++;
                cout << "Funcion " << regresa_funcion(nombre_func) << " no ha sido declarada en la linea : "<<numero_reglon << endl;
                pila_tipos.push("I");
            }
        }
        else if (en_TABSIM(nombre_ide)) {
            if (TABSIM[nombre_ide].dimension1 > 0 || TABSIM[nombre_ide].dimension2 > 0) {
                error++;
                cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
            }
            string dato = TABSIM[nombre_ide].dato;
            dato = dato.at(0);
            pila_tipos.push(dato);
            agregar_codigo("LOD", nombre_ide, "0");
        }
        else if (en_TABSIM(var_global)) {
            if (TABSIM[var_global].dimension1 > 0 || TABSIM[var_global].dimension2 > 0) {
                error++;
                cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
            }
            string dato = TABSIM[var_global].dato;
            dato = dato.at(0);
            pila_tipos.push(dato);
            agregar_codigo("LOD", var_global, "0");
        }
        else {
            error++;
            cout << "Identificador " << regresa_identificador(iden_originial) << " no ha sido declarado" << endl;
            pila_tipos.push("I");
        }
    }
    else {
        if (contexto_regresa != 1) {
            error++;
            cout <<"Error : Se esperaba constante o booleano o funcion o parentesis y se encontro " << lex << endl;
        }
        pila_tipos.push("I");
    }
}

void ope_signo() {
    string op = "";
    if (lex == "-") {
        op = lex;
        expresion+=lex;
        parse();
    }
    termino();
    if( op == "-" ) {
        string Tipo = pila_tipos.top();
        pila_tipos.pop();
        Tipo = op+Tipo;
        int i;
        if( (i = buscaTipo(Tipo) ) >= 0)
            pila_tipos.push(tipoR[i]);
        else {
            error++;
            pila_tipos.push("I");
            cout << "Conflicto en tipos en la operacion, se esperaba >" <<op << "< y se encontro >" << Tipo
            << "< en la linea " << numero_reglon << endl;
        }
        if (op == "-") agregar_codigo("OPR","0","8");
    }
}

void ope_pot() {
    string op;
    do {
        if (lex == "**") {
            expresion+=lex;
            op = lex;
            parse();
        }
        ope_signo();
        if( op == "**") {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto en tipos en la operacion, se esperaba >" <<op << "< y se encontro >" << Tipo
                << "< en la linea " << numero_reglon << endl;
            }
        }
        if (op == "**") agregar_codigo("OPR","0","7");
    } while (lex == "**");
}

void ope_mul() {
    string op;
    do {
        if (lex == "*" || lex == "/" || lex == "%" || lex == "//") {
            expresion+=lex;
            op = lex;
            parse();

        }
        ope_pot();
        if( op == "*" || op == "/" || op == "%" || op == "//") {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto en tipos en la operacion, se esperaba >" <<op << "< y se encontro >" << Tipo
                << "< en la linea " << numero_reglon << endl;
            }
        }
        if (op == "*") agregar_codigo("OPR","0","4");
        if (op == "/") agregar_codigo("OPR","0","5");
        if (op == "%") agregar_codigo("OPR","0","6");
    } while (lex == "*" || lex == "/" || lex == "%" || lex == "//");
}

void ope_sum() {
    string op="";
    bool b=false;
    do {
        if ((lex == "+" || lex == "-")and b) {
            expresion+=lex;
            op = lex;
            parse();
        }
        ope_mul();
        if( op == "+" || op == "-") {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto en tipos en la operacion, se esperaba >" <<op << "< y se encontro >" << Tipo
                << "< en la linea " << numero_reglon << endl;
            }
        }
        if( op == "+" ) agregar_codigo("OPR", "0", "2");
        else if (op == "-" ) agregar_codigo("OPR", "0", "3");
    if (lex == "+" || lex == "-") b = true;
    } while (lex == "+" || lex == "-");
}


void ope_rel() {
    string op;
    ope_sum();
    if (lex == "<" || lex == ">" || lex == "<=" || lex == ">=" ||
        lex == "==" || lex == "!=") {
        op = lex;
        expresion+=lex;
        parse();
        ope_sum();
        if( op == "<" || op == ">" || op == "<=" || op == ">=" ||
            op == "!=" || op == "==") {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto en tipos en la condicion, se esperaba >" <<op << "< y se encontro >" << Tipo
                << "< en la linea " << numero_reglon << endl;
            }
        }
    }
    if (op == "<") agregar_codigo("OPR","0","9");
    else if (op == ">") agregar_codigo("OPR","0","10");
    else if (op == "<=") agregar_codigo("OPR","0","11");
    else if (op == ">=") agregar_codigo("OPR","0","12");
    else if (op == "!=") agregar_codigo("OPR","0","13");
    else if (op == "==") agregar_codigo("OPR","0","14");
}


void ope_no() {
    string op;
    if (lex == "no") {
        expresion+=lex;
        op = lex;
        parse();
    }
    ope_rel();
    if( op == "no" ) {
        string Tipo = pila_tipos.top();
        pila_tipos.pop();
        Tipo = op+Tipo;
        int i;
        if( (i = buscaTipo(Tipo) ) >= 0)
            pila_tipos.push(tipoR[i]);
        else {
            error++;
            pila_tipos.push("I");
            cout << "Conflicto en tipos en la condicion, se esperaba >" <<op << "< y se encontro >" << Tipo
            << "< en la linea " << numero_reglon << endl;
        }
    }
    if (op == "no") agregar_codigo("OPR","0","17");
}


void ope_y() {
    string op;
    do {
        if (lex == "y"){
            expresion+=lex;
            op = lex;
            parse();
        }
        ope_no();
        if( op == "y") {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto de tipos en la condicion, se esperaba >"<<op<<"< y se encontro " << Tipo
                     << " en la linea " << numero_reglon << endl;
            }
        }
        if (op == "y") agregar_codigo("OPR","0","16");
    } while (lex == "y");
}


void expr() {
    string op;
    do {
        if (lex == "o") {
            expresion+=lex;
            op = lex;
            parse();
        }
        ope_y();
        if( op == "o" ) {
            string Tipo = pila_tipos.top(), Tipo1;
            pila_tipos.pop();
            Tipo1=pila_tipos.top();
            pila_tipos.pop();
            Tipo = Tipo1+op+Tipo;
            int i;
            if( (i = buscaTipo(Tipo) ) >= 0)
                pila_tipos.push(tipoR[i]);
            else {
                error++;
                pila_tipos.push("I");
                cout << "Conflicto de tipos en la condicion, se esperaba >" << op <<"< y se encontro " << Tipo
                     << " en la linea " << numero_reglon << endl;
            }
         }
         if (op == "o") agregar_codigo("OPR","0","15");
    } while (lex == "o");
}


void estatuto_si() {
    if (lex == "si") {
        parse();
        expr();
        stack<tabsim> es_si_e;
        stack<string> es_si_n;
        string _Efinal = "_E" + int_to_string(cont_E++);
        string _E = "_E" + int_to_string(cont_E++);
        tabsim element_x = tabsim_element_creator();
        element_x.dimension2 = 0;
        element_x.dato = "I"; element_x.tipo = "I";
        element_x.dimension1 = 0;
        es_si_e.push(element_x);
        es_si_n.push(_E);
        agregar_codigo("JMC","F",_E);
        string Tipo = pila_tipos.top();
        if (Tipo != "B") {
            error++;
            cout << "Conflicto de tipos en la condicion, se esperaba >B< y se encontro " << Tipo
                << " en la linea " << numero_reglon << endl;
        }
        bloque();
        agregar_codigo("JMP","0",_Efinal);
        while (lex == "otsi") {
            element_x = es_si_e.top(); es_si_e.pop();
            _E = es_si_n.top(); es_si_n.pop();
            element_x.dimension1 = cont_L;
            agregar_identificador(element_x,_E);
            parse();
            expr();
            element_x = tabsim_element_creator();
            _E = "_E" + int_to_string(cont_E++);
            element_x.dimension2 = 0;
            element_x.dimension1 = 0;
            element_x.tipo = "I"; element_x.dato = "I";
            es_si_e.push(element_x);
            es_si_n.push(_E);
            agregar_codigo("JMC","F",_E);
            string Tipo = pila_tipos.top();
            if (Tipo != "B") {
                error++;
                cout << "Conflicto de tipos en la condicion, se esperaba >B< y se encontro " << Tipo
                     << " en la linea " << numero_reglon << endl;
            }
            bloque();
            agregar_codigo("JMP","0",_Efinal);
        }
        if (lex == "sino") {
            element_x = es_si_e.top(); es_si_e.pop();
            _E = es_si_n.top(); es_si_n.pop();
            element_x.dimension1 = cont_L;
            agregar_identificador(element_x,_E);
            parse();
            bloque();
        }
        else {
            element_x = es_si_e.top(); es_si_e.pop();
            _E = es_si_n.top(); es_si_n.pop();
            element_x.dimension1 = cont_L;
            agregar_identificador(element_x,_E);
        }
        tabsim ele_final = tabsim_element_creator();
        ele_final.dimension1 = cont_L; ele_final.dimension2=0;
        ele_final.dato = "I"; ele_final.tipo = "I";
        agregar_identificador(ele_final,_Efinal);
    }
}

void estatuto_regresa() {
    if (lex == "regresa") {
        contexto_regresa = 1;
        parse();
        expresion = "";
        expr();
        string valor_regreso = TABDEF[contexto].retorno;
        valor_regreso = valor_regreso.at(0);
        string Tipo = pila_tipos.top();
        if (Tipo != valor_regreso) {
            if (!(Tipo == "E" && valor_regreso == "D")) {
                error++;
                cout << "Conflicto en tipos de retorno, se esperaba >" <<valor_regreso << "< y se encontro >" << Tipo
                     << "< en la linea " << numero_reglon << endl;
            }
        }
        if (Tipo != "I") agregar_codigo("STO","0",contexto);
        agregar_codigo("OPR","0","1");
    }
}

struct s_desde{
    string n;
    string d1;
    string d2;
};
stack<string> p_desde;
stack<s_desde> ps_desde;
int b_pasa = 0;

string lexp(string _E) {
    if (arreglo()) {
        parse();
        return "";
    }
    int i_desde;
    string iden = p_desde.top(); p_desde.pop();
    string tipo_desde;
    string _Ex = "_E" + int_to_string(cont_E++);
    expr();
    string Tipo = pila_tipos.top();
    if (Tipo == "E") {
        tipo_desde=Tipo;
    }
    else if (Tipo == "D") {
        tipo_desde=Tipo;
    }
    else {
        error++;
        cout <<"Conflicto en tipo,se esperaba >E< o >D< y se encontro " << Tipo << " en la linea " << numero_reglon<< endl;
        tipo_desde= "E";
    }
    agregar_codigo("STO","0",iden);
    tabsim element = tabsim_element_creator();
    element.dimension1=cont_L; element.dimension2=0;
    element.tipo = "I"; element.dato ="I";
    agregar_identificador(element,_E);
    agregar_codigo("LOD",iden,"0");
    if (lex != ".") {
        error++;
        cout <<"Se esperaba >> '.' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
    }
    parse();
    if (lex != ".") {
        error++;
        cout <<"Se esperaba >> '.' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
    }
    parse();
    expr();
    i_desde = cont_L;
    agregar_codigo("OPR","0","11"); //9
    agregar_codigo("JMC","F",_Ex);
    Tipo = pila_tipos.top();
    if (Tipo != tipo_desde) {
        error++;
        cout << "Conflicto en tipos se esperaba '" <<tipo_desde<<"' se encontro '"<<Tipo<<"' en " << numero_reglon << endl;
    }
    if (lex == "pasa") {
        parse();
        b_pasa++;
        s_desde e_desde;
        e_desde.n = "STO"; e_desde.d1 = "0"; e_desde.d2="$_EAUX";
        ps_desde.push(e_desde);//sacamos el valor a sumar/restar
        e_desde.n = "LOD"; e_desde.d1 = iden; e_desde.d2="0";
        ps_desde.push(e_desde); //metemos primero variable
        e_desde.n = "LOD"; e_desde.d1 = "$_EAUX"; e_desde.d2="0";
        ps_desde.push(e_desde); //metemos valor a sumar/restar
        //agregar_codigo("LOD",iden,"0");
        if (lex == "-") {
            cod[i_desde].dir2 = "12"; //10
            e_desde.n = "OPR"; e_desde.d1 = "0"; e_desde.d2="3";
            ps_desde.push(e_desde);
            parse();
        }
        else {
            e_desde.n = "OPR"; e_desde.d1 = "0"; e_desde.d2="2";
            ps_desde.push(e_desde);
        }
        expr();
        Tipo = pila_tipos.top();
        if ( Tipo != "E") {
            error++;
            cout <<"Se esperaba >> 'ENTERO' << se encontro >> '"<< Tipo <<"' << en el renglon '" <<numero_reglon << endl;
        }
        e_desde.n = "STO"; e_desde.d1 = "0"; e_desde.d2=iden;
        ps_desde.push(e_desde);;
        //agregar_codigo("STO","0",iden);
    }
    return _Ex;
}



void estatuto_desde() {
    if (lex == "desde") {
        b_ciclo++;
        parse();
        if (token == "IDENTIFICADOR") {
            string var_global = "$"+lex;
            string nombre_ide = contexto+"$"+lex;
            parse();
            if (lex == "[") {
                nombre_ide = dimens(var_global);
                p_desde.push(nombre_ide);
            }
            else if (lex == "(") {
                error++;
                cout << "No se puede usar Funcion en el Desde en la linea " << numero_reglon << endl;
                parse();
            }
            else if (en_TABSIM(nombre_ide)) {
                if (TABSIM[nombre_ide].dimension1 > 0 || TABSIM[nombre_ide].dimension2 > 0) {
                    error++;
                    cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
                }
                else p_desde.push(nombre_ide);
                //else agregar_codigo("OPR",nombre_ide,"19");
            }
            else if (en_TABSIM(var_global)) {
                if (TABSIM[var_global].dimension1 > 0 || TABSIM[var_global].dimension2 > 0) {
                    error++;
                    cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
                }
                else p_desde.push(nombre_ide);
                //else agregar_codigo("OPR",var_global,"19");
            }
            else {
                error++;
                cout << "Identificador " << regresa_identificador(var_global) << " no ha sido declarado" << endl;
            }
        }
        else {
            error++;
            cout <<"Se esperaba >> 'IDENTIFICADOR' << se encontro >> '"<< token <<"' << en el renglon '" <<numero_reglon << endl;
        }
        if (lex != "en"){
            error++;
            cout <<"Se esperaba >> 'en' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
        if (lex == "[") {
            stack<string> valores;
            stack<string> token_valores;
            parse();
            int contador_desde = 0;
            string tipo_desde;
            valores.push(lex); //expr();
            token_valores.push(token);
            tipo_desde = token;
            parse();
            contador_desde++;
            while (lex == ",") {
                parse();
                valores.push(lex);//expr();
                token_valores.push(token);
                parse();
                contador_desde++;
            }
            if ( lex != "]") {
                error++;
                cout << "Se esperaba >]< y se encontro >" << lex << "< en la linea : " << numero_reglon << endl;
            }
            while (!valores.empty()) {
                //string valor = valores.top();
                if (tipo_desde != token_valores.top()) {
                    error++;
                    cout << "Conflicto en tipos se esperaba >"<< tipo_desde << "< y se encontro >"<< token_valores.top() <<"< en la linea : " << numero_reglon << endl;
                }
                agregar_codigo("LIT",valores.top(),"0");
                token_valores.pop();
                valores.pop();
            }
            parse();
            string ide = p_desde.top(); p_desde.pop();
            agregar_codigo("LIT","0","0");
            string _E = "_E" + int_to_string(cont_E++);
            tabsim element = tabsim_element_creator();
            element.dimension1 = cont_L; element.dimension2 = 0;
            element.tipo = "I"; element.dato = "I";
            agregar_identificador(element,_E);
            agregar_codigo("LIT","1","0");
            agregar_codigo("OPR","0","2");
            agregar_codigo("STO","0","$_EAUX");
            agregar_codigo("STO","0",ide);
            agregar_codigo("LOD","$_EAUX","0");
            agregar_codigo("LOD","$_EAUX","0");
            bloque();
            agregar_codigo("LIT",int_to_string(contador_desde),"0");
            agregar_codigo("OPR","0","14");
            agregar_codigo("JMC","F",_E);
            agregar_codigo("STO","0","$_EAUX");
            return;
        }
        string _E = "_E" + int_to_string(cont_E++);
        string _Ex = lexp(_E);
        bloque();
        //revisar la cola para ver si contiene continua
        if (!c_rompe_ciclo.empty()) {
            if (c_rompe_ciclo.top() == "continua") {
                string c_i = c_n.top(); c_n.pop();
                tabsim elemento = tabsim_element_creator();
                elemento.dimension1 = cont_L; elemento.dimension2 = 0;
                elemento.tipo = "I"; elemento.dato = "I";
                agregar_identificador(elemento,c_i);
                c_rompe_ciclo.pop();
            }
        }
        if (b_pasa > 0) {
            s_desde e_desde5,e_desde4,e_desde3,e_desde2,e_desde1;
            e_desde5 = ps_desde.top(); ps_desde.pop();
            e_desde4 = ps_desde.top(); ps_desde.pop();
            e_desde3 = ps_desde.top(); ps_desde.pop();
            e_desde2 = ps_desde.top(); ps_desde.pop();
            e_desde1 = ps_desde.top(); ps_desde.pop();
            agregar_codigo(e_desde1.n,e_desde1.d1,e_desde1.d2);
            agregar_codigo(e_desde2.n,e_desde2.d1,e_desde2.d2);
            agregar_codigo(e_desde3.n,e_desde3.d1,e_desde3.d2);
            agregar_codigo(e_desde4.n,e_desde4.d1,e_desde4.d2);
            agregar_codigo(e_desde5.n,e_desde5.d1,e_desde5.d2);
            b_pasa--;
        }
        agregar_codigo("JMP","0",_E);
        tabsim element = tabsim_element_creator();
        element.dimension1=cont_L; element.dimension2=0;
        element.tipo="I"; element.dato="I";
        agregar_identificador(element,_Ex);
        if (!c_rompe_ciclo.empty()) {
            if (c_rompe_ciclo.top() == "interrumpe") {
                string c_i = c_n.top(); c_n.pop();
                tabsim elemento = tabsim_element_creator();
                elemento.dimension1 = cont_L; elemento.dimension2 = 0;
                elemento.tipo = "I"; elemento.dato = "I";
                agregar_identificador(elemento,c_i);
                c_rompe_ciclo.pop();
            }
        }
        b_ciclo--;
    }
}


void estatuto_mientras() {
    if (lex == "mientras") { //revisar mientras para ver donde se ponen instrucciones de rompe_ciclos
        string _E1 = "_E" + int_to_string(cont_E++);
        tabsim ele_E1 = tabsim_element_creator();
        ele_E1.dimension1 = cont_L; ele_E1.dimension2 = 0;
        ele_E1.dato = "I"; ele_E1.tipo = "I";
        agregar_identificador(ele_E1,_E1);
        b_ciclo++;
        parse();
        expr();
        string Tipo = pila_tipos.top();
        if (Tipo != "B") {
            error++;
            cout << "Conflicto de tipos en la condicion, se esperaba >B< y se encontro " << Tipo
                << " en la linea " << numero_reglon << endl;
        }
        string _E2 = "_E" + int_to_string(cont_E++);
        tabsim ele_E2 = tabsim_element_creator();
        ele_E2.dimension1 = 0; ele_E2.dimension2 = 0;
        ele_E2.dato = "I"; ele_E2.tipo = "I";
        agregar_codigo("JMC","F",_E2);
        bloque();
        tabsim elemento = tabsim_element_creator();
        elemento.dimension1 = cont_L; elemento.dimension2 = 0;
        elemento.tipo = "I"; elemento.dato = "I";
        agregar_codigo("JMP","0",_E1);
        ele_E2.dimension1 = cont_L;
        agregar_identificador(ele_E2,_E2);
        if (!c_rompe_ciclo.empty()) {
            string c_i = c_n.top(); c_n.pop();
            if (c_rompe_ciclo.top() == "interrumpe"){
                elemento.dimension1 = cont_L; elemento.dimension2 = 0;
                elemento.tipo = "I"; elemento.dato = "I";
            }
            agregar_identificador(elemento,c_i);
            c_rompe_ciclo.pop();
        }
        b_ciclo--;
    }
}

void estatuto_imprime() {
    if (lex == "imprime") {
        parse();
        if (lex != "(") {
            error++;
            cout <<"Se esperaba >> '(' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
        expresion ="";
        expr();
        if (pila_tipos.top() == "I") {
            error++;
            cout << "Error en la linea "<< numero_reglon << " expresion " << expresion<<" no regresa valor valido" << endl;
        }
        pila_tipos.pop();
        if (expresion == "\"n\"" || expresion == "'n'") agregar_codigo("OPR","0","21");
        else agregar_codigo("OPR", "0", "20");
        while (lex == ",") {
            parse();
            expresion="";
            expr();
            if (pila_tipos.top() == "I") {
                error++;
                cout << "Error en la linea "<< numero_reglon << " expresion " << expresion<<" no regresa valor valido" << endl;
            }
            pila_tipos.pop();
            if (expresion == "\"n\"" || expresion == "'n'") agregar_codigo("OPR","0","21");
            else agregar_codigo("OPR", "0", "20");
        }
        if (lex != ")") {
            error++;
            cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
    }
}

void estatuto_lee() {
    if (lex == "lee") {
        parse();
        if (lex != "(") {
            error++;
            cout <<"Se esperaba >> '(' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
        if (token == "IDENTIFICADOR") {
            string var_global = "$"+lex;
            string nombre_ide = contexto+"$"+lex;
            parse();
            if (lex == "[") {
                nombre_ide = dimens(var_global);
                agregar_codigo("OPR",nombre_ide,"19");
            }
            else if (lex == "(") {
                error++;
                cout << "No se puede usar Funcion en el Lee en la linea " << numero_reglon << endl;
                parse();
            }
            else if (en_TABSIM(nombre_ide)) {
                if (TABSIM[nombre_ide].dimension1 > 0 || TABSIM[nombre_ide].dimension2 > 0) {
                    error++;
                    cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
                }
                else agregar_codigo("OPR",nombre_ide,"19");
            }
            else if (en_TABSIM(var_global)) {
                if (TABSIM[var_global].dimension1 > 0 || TABSIM[var_global].dimension2 > 0) {
                    error++;
                    cout << "A " << regresa_identificador(nombre_ide) << " le faltan dimensiones" << endl;
                }
                else agregar_codigo("OPR",var_global,"19");
            }
            else {
                error++;
                cout << "Identificador " << regresa_identificador(var_global) << " no ha sido declarado" << endl;
            }
        }
        else {
            error++;
            cout << "Se esperaba Identificador en la linea : " << numero_reglon << " y se encontro " << token <<endl;
        }
        if (lex == ")") {}
        else { error++;
            cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
    }
}

void estatuto_interrumpe() {
    if (lex == "interrumpe") {
        if (b_ciclo == 0) {
            error++;
            cout << "Error: Interrumpe debe estar dentro de un ciclo en la linea : " << numero_reglon << endl;
        }
        string _E = "_E" + int_to_string(cont_E++);
        c_rompe_ciclo.push(lex);
        c_n.push(_E);
        agregar_codigo("JMP","0",_E);
        parse();
    }
}

void estatuto_continua() {
    if (lex == "continua") {
        if (b_ciclo == 0) {
            error++;
            cout << "Error: Continua debe estar dentro de un ciclo en la linea : " << numero_reglon << endl;
        }
        string _E = "_E" + int_to_string(cont_E++);
        c_rompe_ciclo.push(lex);
        c_n.push(_E);
        agregar_codigo("JMP","0",_E);
        parse();
    }
}

int comando() {
    if (lex == "si") {
        estatuto_si();
    }
    else if (lex == "desde") {
        estatuto_desde();
    }
    else if (lex == "regresa") {
        estatuto_regresa();
    }
    else if (lex == "mientras") {
        estatuto_mientras();
    }
    else if (lex == "imprime") {
        estatuto_imprime();
    }
    else if (lex == "lee") {
        estatuto_lee();
    }
    else if (lex == "interrumpe") {
        estatuto_interrumpe();
    }
    else if (lex == "continua") {
        estatuto_continua();
    }
    else if (token == "IDENTIFICADOR") {
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
    if (lex != ":") {
        error++;
        cout <<"Se esperaba >> ':' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
    }
    if (token == "DELIMITADOR") parse();
    estatutos();
    if (lex != ".") {
        error++;
        cout <<"Se esperaba >> '.' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
    }
    parse();
}

char parametro_id () {
    if (token == "ENTERO") {
        return 'E';
    }
    if (token == "BOOLEANO") {
        return 'B';
    }
    if (token == "LITERAL") {
        return 'L';
    }
    if (token == "DECIMAL") {
        return 'D';
    }
}


string param(string prefijo) {
        if (lex == ")") {
            return prefijo;
        }
        tabsim estructura_parametros[10000];
        string nombre_parametros[10000];
        int ind_ep = 0;
        string nombre_funcion = prefijo;
        //nombre_funcion+= f;
        tabsim element = tabsim_element_creator();
        element.linea = numero_reglon;
        string nombre = "";
        if (token == "IDENTIFICADOR") {
            nombre = lex;
        }
        else { error++;
            cout <<"Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << se encontro >> '"<< token <<"' << en el renglon '"<<numero_reglon << endl;
            nombre="";
        }
        parse();
        if (lex == "=") {}
        else { error++;
            cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
        }
        parse();
        if (cte()) {
            element.valor = lex;
            element.dato = token;
            element.tipo = "PARAMETRO";
            nombre_parametros[ind_ep] = nombre;
            estructura_parametros[ind_ep++] = element;
            char pre_par = parametro_id();
            nombre_funcion = nombre_funcion + '%' + pre_par;
            //agregar_identificador(element,nombre);
            parse();
        }
        else if (arreglo()) {
            parse();
        }
        else { error++;
            cout <<"Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << se encontro >> '"<< token <<"' << en el renglon '"<<numero_reglon << endl;
        }
        while (lex == ",") {
            element = tabsim_element_creator();
            element.linea = numero_reglon;
            nombre = "";
            parse();
            if (token == "IDENTIFICADOR") {
                nombre = lex;
            }
            else { error++;
                cout <<"Se esperaba >> 'IDENTIFICADOR' << se encontro >> '"<< token <<"' << en el renglon '"<<numero_reglon << endl;
            }
            parse();
            if (lex == "=") {}
            else { error++;
                cout <<"Se esperaba >> '=' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
            }
            parse();
            if (cte()) {
                element.valor = lex;
                element.dato = token;
                element.tipo = "PARAMETRO";
                nombre_parametros[ind_ep] = nombre;
                estructura_parametros[ind_ep++] = element;
                char pre_par = parametro_id();
                nombre_funcion = nombre_funcion + '%' + pre_par;
                //agregar_identificador(element,nombre);
                parse();
            }
            else if (arreglo()) {
                parse();
            }
            else { error++;
                cout <<"Se esperaba >> 'IDENTIFICADOR' o 'ARREGLO' << se encontro >> '"<< token <<"' << en el renglon '"<<numero_reglon << endl;
            }
        }
        int i = 0;
        for ( ; i< ind_ep; i++ ) {
            string value = nombre_funcion + "$" + nombre_parametros[i];
            agregar_identificador(estructura_parametros[i],value);
        }
        for (i=ind_ep-1 ; i>=0; i-- ) {
            string value = nombre_funcion + "$" + nombre_parametros[i];
            agregar_codigo("STO","0",value);
        }
        return nombre_funcion;
}


void prgm() {
    parse();
    vars();
    agregar_codigo("JMP","0","_E0");
    funcs();
    if (b_principal == 0) { error++;
        cout <<"Se esperaba funcion principal pero no fue encontrada! "<< endl;
    }
    if (token != "") { error++;
        cout <<"Unexpected "<< lex <<" << en el renglon '"<<numero_reglon << endl;
    }
}


void funcs() {
    while (lex == "def") {
        contexto = "";
        contexto_regresa = 0;
        def_funcion elemento_funcion;
        elemento_funcion.linea = numero_reglon;
        elemento_funcion.dimension1 = cont_L;
        string prefijo = "";
        parse();
        if (lex == "principal") {
            if (b_principal == 1) {
                error++;
                cout << "Funcion principal ya ha sido declarada " << endl;
            }
            prefijo = lex;
            b_principal = 1; // si hay principal
            parse();
            if (lex == "(") {
                parse();
            }
            else {
                error++;
                cout <<"Se esperaba >> '(' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
            }
            if (lex == ")") {
                parse();
            }
            else {
                error++;
                cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' <<(principal no debe recibir parametros) en el renglon '"<<numero_reglon << endl;
            }
            elemento_funcion.retorno = "I"; //vacio
        }
        else if (token == "IDENTIFICADOR") {
            prefijo = lex;
                    parse();
                    if (lex == "(") {
                        parse();
                    }
                    else {
                        error++;
                        cout <<"Se esperaba >> '(' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
                    }
                    prefijo=param(prefijo);
                    if (lex == ")") {
                        parse();
                    }
                    else {
                        error++;
                        cout <<"Se esperaba >> ')' << se encontro >> '"<< lex <<"' << en el renglon '"<<numero_reglon << endl;
                    }
                    if (cte()) {
                        elemento_funcion.retorno = token;
                        parse();
                    }
                    else elemento_funcion.retorno = "I"; //vacio
        }
        else {
            error++;
            cout <<"Se esperaba >> 'IDENTIFICADOR1' << se encontro >> '"<< token <<"' << en el renglon '"<<numero_reglon << endl;
        }
        agregar_funcion(elemento_funcion,prefijo);
        vars(prefijo);
        contexto = prefijo;
        bloque();
        if (contexto_regresa == 0) {
            if (TABDEF[contexto].retorno != "I") {
                error++;
                cout << "Funcion " << regresa_identificador(contexto) << " debe regresar " << TABDEF[contexto].retorno <<endl;
            }
        }
        if (contexto != "principal") {
            agregar_codigo("OPR","0","1");
        }
    }
}


bool crear_archivo(string archivo) {
    Salida.open(archivo);
    if( Salida == NULL ) return false;
    return true;
}

void cierra_archivo() {
    Salida.flush();
    Salida.close();
}

string regresa_tipo(string tipo) {
    if (tipo == "ENTERO") return "E";
    if (tipo == "DECIMAL") return "R";
    if (tipo == "LITERAL") return "A";
    if (tipo == "BOOLEANO") return "L";
    else return "I";
}

void copiar_codigo() {
    int i=1;
    for ( ; i<cont_L ; i++) {
        Salida << i << " " << cod[i].ins << " " << cod[i].dir1 << "," << cod[i].dir2 << endl;
    }
}

void copiar_variables() {
    for( map<string, tabsim>::iterator ii=TABSIM.begin(); ii!=TABSIM.end(); ++ii) {
        if ((*ii).second.existe == "verdad") {
            string n,c,t,d1,d2;
            n = (*ii).first;
            c = (*ii).second.tipo;
            c = c.at(0);
            t = regresa_tipo((*ii).second.dato);
            d1 = int_to_string((*ii).second.dimension1);
            d2 = int_to_string((*ii).second.dimension2);
            Salida<< n << ","
                  << c << ","
                  << t << ","
                  << d1 << ","
                  << d2 << ",#,"
                  << endl;
        }
    }
    for( map<string, def_funcion>::iterator ii=TABDEF.begin(); ii!=TABDEF.end(); ++ii) {
        if ((*ii).second.retorno != "") {
            string n,c,t,d1,d2;
            if ((*ii).first == "principal") n = "_E0";
            else n = (*ii).first;
            c = "F";
            t = regresa_tipo((*ii).second.retorno);
            d1 = int_to_string((*ii).second.dimension1);
            d2 = "0";
            Salida<< n << ","
                  << c << ","
                  << t << ","
                  << d1 << ","
                  << d2 << ",#,"
                  << endl;
        }
    }
    Salida << "_P,F,I,1,0,#," << endl;
    Salida << "$_EAUX,V,E,0,0,#," << endl;
    Salida << "@" << endl;
    agregar_codigo("OPR","0","0");
    copiar_codigo();
}


int main () {
    char caracter = 0;
    int i=0 , j=0;
    FILE *archivo_entrada;
    string archivo;
    string archivo_udg;
    string archivo_eje;
    //archivo_entrada = fopen ("lexico.txt","r");
    cout << "Archivo a Compilar(sin extension) :  ";
    cin >> archivo;
    archivo_udg = archivo + ".udg";
    archivo_eje = archivo + ".eje";
    archivo_entrada = fopen (archivo_udg.c_str(),"r");
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
    else {
        cout << "Error al Abrir el archivo o no se encontro..." << endl;
        return 0;
    }
    int renglon = 1;
    cout<<"["<<renglon++<<"]\t";
    for (j=0; j<i-1 ; j++) {
        if(entrada[j] == '\n') {
            cout << "\n["<<renglon++<<"]\t";
        }
        else cout << entrada[j];
    }
    cout << "\n\n" << endl;
    prgm();
    if (error == 0 ) {
        ofstream Salida;
        cout << "\n\n\nAnalisis Completo\n\n"<< endl;
        if (crear_archivo(archivo_eje)) {
            copiar_variables();
            cierra_archivo();
            string prog = "inter " + archivo;// +" d";
            system(prog.c_str());
        }
        else {
            cout << "No se pudo abrir/crear el archivo ejecutable..."<< endl;
        }
    }
    else cout << "\n\n\nNo se pudo compilar programa debido a "<<error<<" errores\n\n"<< endl;

    return 0;
}

