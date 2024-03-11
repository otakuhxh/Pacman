//Por Eddy Zamora, María Belén Garcia y Tiffany Jordán
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

// Definiciones de colores usando códigos ANSI
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_F     "\x1b[41m"
#define VERDE_F        "\x1b[42m"
#define    AMARILLO_F  "\x1b[43m"
#define    AZUL_F      "\x1b[44m"
#define MAGENTA_T  "\x1b[35m"
#define MAGENTA_F  "\x1b[45m"
#define CYAN_T     "\x1b[36m" //T
#define CYAN_F     "\x1b[46m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"

void gotoxy(int x,int y)
{
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y= y;
    SetConsoleCursorPosition(hcon,dwPos);
}

FILE *archivo;

struct NodoJugador
{
    char nombre[50];
    int puntaje;
};

struct Nodo
{
    struct NodoJugador *jugador;
    struct Nodo *siguiente;
    struct Nodo *anterior;
};
struct Nodo *cabeza=NULL;

//Lo llama por primera vez para guardar
struct NodoJugador *crearNodoJugador(int puntaje)
{
    struct NodoJugador *nuevoJugador=malloc(sizeof(struct NodoJugador));
    gotoxy(40,17);
    printf("Ingrese su nombre: ");
    scanf("%50s", &nuevoJugador->nombre);
    nuevoJugador->puntaje = puntaje;
    return nuevoJugador;
}
//interno
struct Nodo *crearNodo(struct NodoJugador *jugador)
{
    struct Nodo *nuevoNodo = malloc(sizeof(struct Nodo));
    nuevoNodo->jugador = jugador;
    nuevoNodo->siguiente = NULL;
    nuevoNodo->anterior =NULL;
    return nuevoNodo;
}

//Segundo en llamarse
void ingresoJugador(struct NodoJugador *jugador)
{
    struct Nodo *nuevoNodo = crearNodo(jugador);
    if(cabeza==NULL)
    {
        cabeza=nuevoNodo;
    }
    else
    {
        struct Nodo *nodoAux = cabeza;
        while(nodoAux->siguiente != NULL)
        {
            nodoAux = nodoAux->siguiente;
        }
        //Al encontrar vacio
        nodoAux->siguiente = nuevoNodo; //escribe el jugador
        nuevoNodo->anterior = nodoAux; //el nuevo nodo se crea un enlace con el anterior nodoaux
    }

}

void intercambiarNodo(struct Nodo *a, struct Nodo *b)
{
    struct NodoJugador *temp = a->jugador;
    a->jugador = b->jugador;
    b->jugador = temp;
}

// Función para ordenar la lista enlazada doble por puntaje
void ordenarLista()
{
    int intercambiado;
    struct Nodo *puntero;
    struct Nodo *ultimo = NULL;

    // Manejo de lista vacía o con un solo elemento
    if (cabeza == NULL || cabeza->siguiente == NULL)
    {
        return;
    }

    do
    {
        intercambiado = 0;
        puntero = cabeza;

        while (puntero->siguiente != ultimo) //s detendria cuando llegase a null null
        {
            if (puntero->jugador->puntaje < puntero->siguiente->jugador->puntaje)
            {
                intercambiarNodo(puntero, puntero->siguiente);
                intercambiado = 1;
            }
            puntero = puntero->siguiente;
        }
        ultimo = puntero;
    }
    while (intercambiado); //si no se realizan cambios sale del bucle
}

//Tercero en ejecutarse
void guardarGanadores()
{
    archivo = fopen("archivo.dat", "w");
    if(!archivo)
    {
        gotoxy(40,18);
        printf("Error al abrir el archivo\n");
        return;
    }

    if(cabeza==NULL)
    {
        gotoxy(40,18);
        printf("No hay ganadores aun");
    }
    else
    {
        ordenarLista();
        struct Nodo *nodoAux = cabeza;
        while(nodoAux != NULL) //no ejecutar en base al siguiente si no al actual
        {
            fprintf(archivo,"%d puntos | Nombre: %s|\n",nodoAux->jugador->puntaje,nodoAux->jugador->nombre);
            nodoAux = nodoAux->siguiente;
        }
    }
    fclose(archivo);
}

void leerGanadores()
{
    int cont=10; //posicion para imprimirse
    archivo = fopen("archivo.dat", "r");
    char linea[100];
    while(fgets(linea, sizeof(linea), archivo) != NULL)
    {
        gotoxy(40,cont);
        printf("%s", linea);
        cont++;
    }
    fclose(archivo);
    getch();
}

void eliminarArchivo()
{
    archivo = fopen("archivo.dat", "r");
    fclose(archivo);
    remove("archivo.dat");
}
void cargarGanadores()
{
    {
        archivo = fopen("archivo.dat", "r");
        if (!archivo)
        {
            return;
        }

        int puntaje;
        char nombre[50];

        while (fscanf(archivo, "%d puntos | Nombre: %s|", &puntaje, nombre) == 2) // porque devuelve los dos datos leidos con exito
        {
            // Crear un nuevo nodo jugador
            struct NodoJugador *jugador = malloc(sizeof(struct NodoJugador));
            strcpy(jugador->nombre, nombre); //copia el dato dentro de la variable
            jugador->puntaje = puntaje;

            // Crear un nuevo nodo
            struct Nodo *nuevoNodo = crearNodo(jugador); //con lo datos del jugador

            // Insertar el nodo al final de la lista
            if (cabeza == NULL)
            {
                cabeza = nuevoNodo;
            }
            else
            {
                struct Nodo *nodoAux = cabeza;
                while (nodoAux->siguiente != NULL)
                {
                    nodoAux = nodoAux->siguiente;
                }
                //crea los enlaces
                nodoAux->siguiente = nuevoNodo;
                nuevoNodo->anterior = nodoAux;
            }
        }

        fclose(archivo);
    }
}
char escenario[31][28];
char    ESI=218,    //ESQUINA SUPERIOR IZQUIEDA
        ESD=191,    //ESQUINA SUPERIOR DERECHA
        EII=192,    //ESQUINA INFERIOR IZQUIERDA
        EID=217,    //ESQUINA INFERIOR DERECHA
        BH=196,     //BORDE HORIZONTAL
        BV=179,    //BORDE VERTICAL
        BF=176,  //BORDE FANTASMA
        COIN='*', //MONEDAS
        PODER=254,   //BOLITA ESPECIAL
        PM='C'; // PACMAN
        F1='F'; // FANTASMA 1, esta para los cuatro fantasmas

int validar()
{
    char opt='0';
    if (_kbhit()) // PRUEBA BEEP
    {
        opt = _getch();
        return opt;
    }
    else
    {
        return 0;
    }
}

int PacManMusica()
{
    //Do: 262, Re: 294, Mi: 330, Fa: 350, Sol: 392, La: 440
    char opt='0';
    while(0==0)
    {
        Beep(262,300);//do
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(523,300);//do
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(392,300);//sol
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(330,300);//mi
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(523,300);//do
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(392,300);//sol
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(330,300);//mi
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }

        Beep(277,300);//do#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(554,300);//do#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(415,300);//sol#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(349,300);//fa
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(554,300);//do#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(415,300);//sol#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(349,300);//fa
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }

        Beep(262,300);//do
        if(opt!=0)
        {
            return opt;
        }
        Beep(523,300);//do
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(392,300);//sol
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(330,300);//mi
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(523,300);//do
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(392,300);//sol
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(330,300);//mi
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }

        Beep(349,300);//fa
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(370,300);//fa#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(392,300);//sol
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(415,300);//sol#
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(440,300);//la
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(494,300);//si
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
        Beep(523,300);//DO
        opt=validar();
        if(opt!=0)
        {
            return opt;
        }
    }
}

void marco()
{
    int posX=20, posY=0,i;
    for (i = posX; i <= 93; i++)
    {
        gotoxy(i, 0);
        printf("%c", 219);
        gotoxy(i, 33);
        printf("%c", 219);
    }
    for (i = posY; i < 34; i++)
    {
        gotoxy(20, i);
        printf("%c", 219);
        gotoxy(93, i);
        printf("%c", 219);
    }
}

void limpiarPantalla()
{
    int x=21, y=1,i,j;
    for(i=y; i<33; i++)
    {
        for( j=x; j<=92; j++)
        {
            gotoxy(j,i);
            printf(" ");
        }
    }
}


void letraPacMan()
{
    int pcX=30, pcY=1,y,x,i;
    //rayas verticales
    for (y=0; y<8; y++)
    {
        gotoxy(30,pcY);
        printf(YELLOW "%c",219);
        gotoxy(38,pcY);
        printf("%c",219);
        gotoxy(42,pcY);
        printf("%c",219);
        gotoxy(47,pcY);
        printf("%c",219);
        gotoxy(58,pcY);
        printf("%c",219);
        gotoxy(64,pcY);
        printf("%c",219);
        gotoxy(70,pcY);
        printf("%c",219);
        gotoxy(74,pcY);
        printf("%c",219);
        gotoxy(80,pcY);
        printf("%c",219);
        gotoxy(84,pcY);
        printf("%c",219);
        pcY++;
    }
    //rayas horizontales
    for( x=0; x<4; x++)
    {
        gotoxy(pcX,1);
        printf("%c",219);
        gotoxy(pcX,4);
        printf("%c",219);
        gotoxy(pcX+8,1);
        printf("%c",219);
        gotoxy(pcX+8,4);
        printf("%c",219);
        gotoxy(pcX+17,1);
        printf("%c",219);
        gotoxy(pcX+17,8);
        printf("%c",219);
        gotoxy(pcX+22,4);
        printf("%c",219);
        gotoxy(pcX+40,1);
        printf("%c",219);
        gotoxy(pcX+40,4);
        printf("%c",219);
        pcX++;
    }
 //rayita de la p
    for( i=1; i<=4; i++)
    {
        gotoxy(34,i);
        printf("%c",219);
    }
//rayas en x diagonales
//-i /
//+i |raya del otro lado
    for( i=0; i<4; i++)
    {
        gotoxy(i+58,i+1);
        printf("%c",219);
        gotoxy(61+i,4-i);
        printf("%c",219);
        gotoxy(80+i,2+i);
        printf("%c",219);
    }
}

void dibujoFantasmitas()
{
    int j,i;
    for( j=25; j<33; j++)
    {
        gotoxy(31,j);
        printf(RED "%c",219);
        gotoxy(32,j);
        printf("%c",219);
        gotoxy(34,j-3);
        printf("%c",219);
        gotoxy(37,j-3);
        printf("%c",219);
        gotoxy(39,j);
        printf("%c",219);
        gotoxy(40,j);
        printf("%c",219);

        gotoxy(45,j);
        printf(MAGENTA_T "%c",219);
        gotoxy(46,j);
        printf("%c",219);
        gotoxy(48,j-3);
        printf("%c",219);
        gotoxy(51,j-3);
        printf("%c",219);
        gotoxy(53,j);
        printf("%c",219);
        gotoxy(54,j);
        printf("%c",219);

        gotoxy(59,j);
        printf(CYAN_T "%c",219);
        gotoxy(60,j);
        printf("%c",219);
        gotoxy(62,j-3);
        printf("%c",219);
        gotoxy(65,j-3);
        printf("%c",219);
        gotoxy(67,j);
        printf("%c",219);
        gotoxy(68,j);
        printf("%c",219);

        gotoxy(73,j);
        printf(GREEN "%c",219);
        gotoxy(74,j);
        printf("%c",219);
        gotoxy(76,j-3);
        printf("%c",219);
        gotoxy(79,j-3);
        printf("%c",219);
        gotoxy(81,j);
        printf("%c",219);
        gotoxy(82,j);
        printf("%c",219);
    }

    for( j=26; j<33; j++)
    {
        gotoxy(33,j-3);
        printf(RED "%c",219);
        gotoxy(38,j-3);
        printf("%c",219);

        gotoxy(47,j-3);
        printf(MAGENTA_T "%c",219);
        gotoxy(52,j-3);
        printf("%c",219);

        gotoxy(61,j-3);
        printf(CYAN_T "%c",219);
        gotoxy(66,j-3);
        printf("%c",219);

        gotoxy(75,j-3);
        printf(GREEN "%c",219);
        gotoxy(80,j-3);
        printf("%c",219);

    }

    for( j=22; j<33; j++)
    {
        gotoxy(35,j);
        printf(RED "%c",219);
        gotoxy(36,j);
        printf("%c",219);

        gotoxy(49,j);
        printf(MAGENTA_T "%c",219);
        gotoxy(50,j);
        printf("%c",219);

        gotoxy(63,j);
        printf(CYAN_T "%c",219);
        gotoxy(64,j);
        printf("%c",219);

        gotoxy(77,j);
        printf(GREEN "%c",219);
        gotoxy(78,j);
        printf("%c",219);
    }

    for( i=34; i<37; i++)
    {
        for ( j=25; j<27; j++)
        {
            gotoxy(i,j); //color de ojos
            printf(RESET "%c",219);
            gotoxy(i+4,j);
            printf("%c",219);

            gotoxy(i+14,j);
            printf("%c",219);
            gotoxy(i+18,j);
            printf("%c",219);

            gotoxy(i+28,j);
            printf("%c",219);
            gotoxy(i+32,j);
            printf("%c",219);

            gotoxy(i+42,j);
            printf("%c",219);
            gotoxy(i+46,j);
            printf("%c",219);
        }
    }

    gotoxy(36,26);
    printf(NEGRO_T "%c",219);
    gotoxy(40,26);
    printf("%c",219);
    gotoxy(50,26);
    printf("%c",219);
    gotoxy(54,26);
    printf("%c",219);
    gotoxy(64,26);
    printf("%c",219);
    gotoxy(68,26);
    printf("%c",219);
    gotoxy(78,26);
    printf("%c",219);
    gotoxy(82,26);
    printf("%c",219);

}

void barraDeCarga()
{
    int xSup=21, ySup=16, xInf=92, yInf=18,i;
    for ( i=xSup; i<=xInf; i++ )
    {
        gotoxy(i,ySup);
        printf("%c",196);
        gotoxy(i,yInf);
        printf("%c",196);
    }

    for( i=ySup; i<=yInf; i++)
    {
        gotoxy(xSup,i);
        printf("%c",179);
        gotoxy(xInf,i);
        printf("%c",179);
    }
    gotoxy(xSup,ySup); //marco superior
    printf("%c",218);
    gotoxy(xInf,yInf); //marco inferior
    printf("%c",217);
    gotoxy(xInf, ySup);
    printf("%c", 191);
    gotoxy(xSup, yInf);
    printf("%c", 192);

    for( i=22; i<=91; i++)
    {
        gotoxy(i,17); //barrita
        printf(YELLOW "%c",219);
        Sleep(35);
    }

}

void llenar_Escenario() //matriz vaciada
{
    int i,j;
    for(i=0; i<31; i++)
    {
        for(j=0; j<28; j++)
        {
            escenario[i][j]=' ';
        }
    }
}

void Crear_Bordes()
{
    int nj=0, ni=0;
    //ESQUINAS DE LA PARTE EXTERIOR DEL MAPA
    escenario[0][0]=ESI;
    escenario[0][14]=ESI;
    escenario[9][27-5]=ESI;
    escenario[15][27-5]=ESI;
    escenario[19][0]=ESI;
    escenario[25][0]=ESI;
    escenario[24][25]=ESI;


    escenario[0][13]=ESD;
    escenario[0][27]=ESD;
    escenario[9][5]=ESD;
    escenario[15][5]=ESD;
    escenario[19][27]=ESD;
    escenario[24][2]=ESD;
    escenario[25][27]=ESD;


    escenario[4][13]=EII;
    escenario[9][0]=EII;
    escenario[13][27-5]=EII;
    escenario[19][27-5]=EII;
    escenario[24][0]=EII;
    escenario[30][0]=EII;
    escenario[25][25]=EII;

    escenario[4][14]=EID;
    escenario[9][27]=EID;
    escenario[13][5]=EID;
    escenario[19][5]=EID;
    escenario[30][27]=EID;
    escenario[25][2]=EID;
    escenario[24][27]=EID;

    //ESQUINAS DE OBSTACULOS----------------------------
    escenario[2][2]=ESI;
    escenario[2][7]=ESI;
    escenario[2][16]=ESI;
    escenario[2][22]=ESI;
    escenario[6][2]=ESI;
    escenario[6][7]=ESI;
    escenario[6][10]=ESI;
    escenario[6][19]=ESI;
    escenario[6][22]=ESI;
    escenario[7][14]=ESI;
    escenario[9][16]=ESI;
    escenario[10][8]=ESI;
    escenario[12][10]=ESI;
    escenario[15][7]=ESI;
    escenario[15][19]=ESI;
    escenario[18][10]=ESI;
    escenario[19][14]=ESI;
    escenario[21][7]=ESI;
    escenario[21][16]=ESI;
    escenario[21][2]=ESI;
    escenario[21][22]=ESI;
    escenario[22][23]=ESI;
    escenario[24][10]=ESI;
    escenario[25][14]=ESI;
    escenario[24][7]=ESI;
    escenario[24][19]=ESI;
    escenario[27][2]=ESI;
    escenario[27][16]=ESI;


    escenario[2][5]=ESD;
    escenario[2][11]=ESD;
    escenario[2][20]=ESD;
    escenario[2][25]=ESD;
    escenario[6][5]=ESD;
    escenario[6][8]=ESD;
    escenario[6][17]=ESD;
    escenario[6][20]=ESD;
    escenario[6][25]=ESD;
    escenario[7][13]=ESD;
    escenario[9][11]=ESD;
    escenario[10][19]=ESD;
    escenario[12][17]=ESD;
    escenario[15][8]=ESD;
    escenario[15][20]=ESD;
    escenario[18][17]=ESD;
    escenario[19][13]=ESD;
    escenario[21][11]=ESD;
    escenario[21][20]=ESD;
    escenario[21][5]=ESD;
    escenario[21][25]=ESD;
    escenario[22][4]=ESD;
    escenario[24][17]=ESD;
    escenario[25][13]=ESD;
    escenario[24][8]=ESD;
    escenario[24][20]=ESD;
    escenario[27][11]=ESD;
    escenario[27][25]=ESD;



    escenario[4][2]=EII;
    escenario[4][7]=EII;
    escenario[4][16]=EII;
    escenario[4][22]=EII;
    escenario[7][2]=EII;
    escenario[7][10]=EII;
    escenario[7][22]=EII;
    escenario[9][8]=EII;
    escenario[10][13]=EII;
    escenario[10][16]=EII;
    escenario[13][7]=EII;
    escenario[13][19]=EII;
    escenario[16][10]=EII;
    escenario[19][7]=EII;
    escenario[19][19]=EII;
    escenario[19][10]=EII;
    escenario[22][13]=EII;
    escenario[22][7]=EII;
    escenario[22][16]=EII;
    escenario[22][2]=EII;
    escenario[25][4]=EII;
    escenario[25][22]=EII;
    escenario[25][10]=EII;
    escenario[27][8]=EII;
    escenario[27][20]=EII;
    escenario[28][13]=EII;
    escenario[28][2]=EII;
    escenario[28][16]=EII;


    escenario[4][5]=EID;
    escenario[4][11]=EID;
    escenario[4][20]=EID;
    escenario[4][25]=EID;
    escenario[7][5]=EID;
    escenario[7][17]=EID;
    escenario[7][25]=EID;
    escenario[9][19]=EID;
    escenario[10][11]=EID;
    escenario[10][14]=EID;
    escenario[13][8]=EID;
    escenario[13][20]=EID;
    escenario[16][17]=EID;
    escenario[19][8]=EID;
    escenario[19][20]=EID;
    escenario[19][17]=EID;
    escenario[22][14]=EID;
    escenario[22][11]=EID;
    escenario[22][20]=EID;
    escenario[22][25]=EID;
    escenario[25][5]=EID;
    escenario[25][23]=EID;
    escenario[25][17]=EID;
    escenario[27][7]=EID;
    escenario[27][19]=EID;
    escenario[28][14]=EID;
    escenario[28][11]=EID;
    escenario[28][25]=EID;

    int i,j;
    for( i=0; i<31; i++)
    {
        nj=0;
        ni=0;
        for( j=0; j<28; j++)
        {
            if(escenario[i][j]==ESI) //BORDE HORIZONTAL PARA LA ESQUINA SUPERIOR IZQUIERDA
            {
                nj=j+1;
                while(escenario[i][nj]!=ESD && escenario[i][nj]!=EID && escenario[i][nj]!=EII && nj<28)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                {
                    if(i==12 && nj == 13)
                    {
                        escenario[i][nj]=BF;
                    }
                    else
                    {
                        if(i==12 && nj == 14)
                        {
                            escenario[i][nj]=BF;
                        }
                        else
                        {
                            escenario[i][nj]=BH;
                        }
                    }
                    nj=nj+1;
                }
            }
            else
            {
                if(escenario[i][j]==EII)//BORDE HORIZONTAL PARA LA ESQUINA INFERIOR IZQUIERDA
                {
                    nj=j+1;
                    while(escenario[i][nj]!=ESD && escenario[i][nj]!=EID && escenario[i][nj]!=ESI && nj<28)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                    {
                        escenario[i][nj]=BH;
                        nj=nj+1;
                    }
                }
                else
                {
                    if(i==13 && j==0)//BORDE HORIZONTAL DE LA MITAD DEL MAPA
                    {
                        nj=j;
                        while(escenario[i][nj]!=ESD && escenario[i][nj]!=EID && escenario[i][nj]!=ESI && nj<28)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                        {
                            escenario[i][nj]=BH;
                            nj=nj+1;
                        }
                    }
                    if(i==15 && j==0)//2DO BORDE HORIZONTAL DE LA MITAD DEL MAPA
                    {
                        nj=j;
                        while(escenario[i][nj]!=ESD && escenario[i][nj]!=EID && escenario[i][nj]!=ESI && nj<28)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                        {
                            escenario[i][nj]=BH;
                            nj=nj+1;
                        }
                    }
                }

            }

            if(escenario[i][j]==ESI)//BORDE VERTICAL DE LA ESQUINA SUPERIOR IZQUIERDA
            {
                ni=i+1;
                while(escenario[ni][j]!=EII && escenario[ni][j]!=EID)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                {
                    escenario[ni][j]=BV;
                    ni=ni+1;
                }
            }
            else
            {
                if(escenario[i][j]==ESD)//BORDE VERTICAL DE LA ESQUINA SUPERIOR DERECHA
                {
                    ni=i+1;
                    while(escenario[ni][j]!=EII && escenario[ni][j]!=EID)//SIGUE CREANDO BORDE MIENTRAS EL ESPACIO DE LA MATRIZ SEA DIFERENTE
                    {
                        escenario[ni][j]=BV;
                        ni=ni+1;
                    }
                }
            }
        }
    }
}


void colocarMonedas()
{
    int i,j;
    for( i=0; i<31; i++)
    {
        for( j=0; j<28; j++)
        {

            if(escenario[i][j]!=BH && escenario[i][j]!=BV && escenario[i][j]!=ESI && escenario[i][j]!=ESD && escenario[i][j]!=EII && escenario[i][j]!=EID)
            {
                if(i==3 && j!=1 && j!=6 && j!=12 && j!=15 && j!=21 && j!=26)
                {
                }
                else
                {
                    if(i>=10 && i<=18 && j!=6 && j!=9 && j!=10 && j!= 11 && j!=12 && j!=13 && j!=14 && j!=15 && j!=16 && j!=17 && j!=18 && j!=21)
                    {
                    }
                    else
                    {
                        if(i>=10 && i<=19 && j>=9 && j<=18)
                        {

                        }
                        else
                        {
                            escenario[i][j]=COIN;
                        }
                    }
                }
            }
        }
    }
    escenario[3][1]=PODER;
    escenario[3][26]=PODER;
    escenario[23][1]=PODER;
    escenario[23][26]=PODER;
}

void Mostrar_Bordes()
{
    int x=30, y=1, ni=0, nj=0,i,j;
    for( i=0; i<31; i++)
    {
        x=30;
        for( j=0; j<28; j++)
        {
            if(escenario[i][j]==BH || escenario[i][j]==ESI || escenario[i][j]==EII)
            {
                if(escenario[i][j] == COIN || escenario[i][j] == PODER)
                {
                    gotoxy(x,y);
                    printf(YELLOW "%c", escenario[i][j]);
                }
                else
                {
                    gotoxy(x,y);
                    printf(BLUE "%c", escenario[i][j]);
                }

                if(j!=27)
                {
                    if(i == 12 && j==12)
                    {
                    }
                    else
                    {
                        gotoxy(x+1,y);
                        printf(BLUE "%c", BH);
                    }
                }
            }
            else
            {
                if(escenario[i][j] == COIN || escenario[i][j] == PODER)
                {
                    gotoxy(x,y);
                    printf(YELLOW "%c", escenario[i][j]);
                }
                else
                {
                    gotoxy(x,y);
                    printf(BLUE "%c", escenario[i][j]);
                }
                //gotoxy(x,y); printf("%c", escenario[i][j]);
            }
            x=x+2;
        }
        y=y+1;
    }
    printf(""RESET);
}

int contar_monedas(int coin, int c, int f)
{
    if(escenario[f][c]==COIN || escenario[f][c] == PODER)
    {
        coin = coin +100;
    }
    return coin;
}

int terminar_juego()
{
    int n=0,i,j;
    for( i=0; i<31; i++)
    {
        for( j=0; j<28; j++)
        {
            if(escenario[i][j]==COIN)
            {
                n=n+1;
            }
        }
    }
    return n;
}

//int tiempo=10;
char Mpacman=' ', teclaant, teclaNew;
int x=58, y=24, d=23, c=14, count=0, game=1;
int contador=0;

int vida=3;
//Fantasma 1 - rojo
char Fantasma1=' ', teclaanterior, teclaNueva;
int x2=58, y2=12, d2=11, c2=14;
int poder_activo=0;
int cuenta_tiempo=100;

//Fanstama3-verde
char Fantasma3=' ', teclaanterior3, teclaNueva3;
int x3=58, y3=12, d3=11, c3=14;

//Fantasma4 - morado
char Fantasma4=' ', teclaanterior4, teclaNueva4;
int x4=58, y4=12, d4=11, c4=14;

//Fanstasma5 - azul
char Fantasma5=' ', teclaanterior5, teclaNueva5;
int x5=58, y5=12, d5=11, c5=14;



void movimiento_fantasma_verde()
{
    int contador2=1, prueba=1;
    if (poder_activo==0)
    {
        gotoxy(x3,y3);
        printf(GREEN "%c", F1);
    }
    if (poder_activo==1)
    {
        gotoxy(x3,y3);
        printf(CYAN_T "%c", F1);
        //cuenta_tiempo--;
    }

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    while(prueba==1)
    {
        if(game==0)
        {
            contador=1;
            return 0;
        }
        teclaanterior3=Fantasma3;
        if (contador2==1)
        {
            Fantasma3=rand()%4;
            switch (Fantasma3)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d3-1][c3]==COIN || escenario[d3-1][c3]==' ' || escenario[d3-1][c3] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    y3=y3-1;
                    d3=d3-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }

                    teclaNueva3=Fantasma3;
                    contador2=0;

                    //verde_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva3=teclaanterior3;
                    Fantasma3=teclaanterior3;

                    //verde_llamada_fantasmas();

                    movimiento_fantasma_verde();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();


                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d3+1][c3]==COIN || escenario[d3+1][c3]==' ' || escenario[d3+1][c3] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    y3=y3+1;
                    d3=d3+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva3=Fantasma3;
                    contador2=0;

                    //verde_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva3=teclaanterior3;
                    Fantasma3=teclaanterior3;

                    //verde_llamada_fantasmas();
                    movimiento_fantasma_verde();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d3][c3-1]== COIN || escenario[d3][c3-1]==' ' || escenario[d3][c3-1] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);

                    x3=x3-2;
                    c3=c3-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva3=Fantasma3;
                    contador2=0;

                    //verde_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva3=teclaanterior3;
                    Fantasma3=teclaanterior3;

                    //verde_llamada_fantasmas();

                    movimiento_fantasma_verde();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d3][c3+1]== COIN || escenario[d3][c3+1]==' ' || escenario[d3][c3+1] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    x3=x3+2;
                    c3=c3+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva3=Fantasma3;
                    contador2=0;

                    //verde_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva3=teclaanterior3;
                    Fantasma3=teclaanterior3;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();

                    //verde_llamada_fantasmas();

                    movimiento_fantasma_verde();
                    /*
                                        validacion();
                                        if (vida<=0)
                                        {
                                            return 0;
                                        }
                                        movimiento_pacman();*/
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }
        else
        {
            switch (teclaNueva3)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d3-1][c3]==COIN || escenario[d3-1][c3]==' ' || escenario[d3-1][c3] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    y3=y3-1;
                    d3=d3-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //verde_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d3+1][c3]==COIN || escenario[d3+1][c3]==' ' || escenario[d3+1][c3] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);

                    y3=y3+1;
                    d3=d3+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //verde_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d3][c3-1]== COIN || escenario[d3][c3-1]==' ' || escenario[d3][c3-1] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    x3=x3-2;
                    c3=c3-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //verde_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d3][c3+1]== COIN || escenario[d3][c3+1]==' ' || escenario[d3][c3+1] == PODER)
                {
                    gotoxy(x3,y3);
                    printf(" ");
                    gotoxy(x3,y3);
                    printf(YELLOW"%c"RESET, escenario[d3][c3]);
                    x3=x3+2;
                    c3=c3+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x3,y3);
                        printf(GREEN "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x3,y3);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //verde_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }

        if(x3==84)
        {
            gotoxy(x3,y3);
            printf(" ");

            gotoxy(x3,y3);
            printf(YELLOW"%c"RESET, escenario[d3][c3]);


            x3=32;
            c3=1;
            if (poder_activo==0)
            {
                gotoxy(x3,y3);
                printf(GREEN "%c", F1);
            }
            if (poder_activo==1)
            {
                gotoxy(x3,y3);
                printf(CYAN_T "%c", F1);
            }
        }
        else
        {
            if(x3==30)
            {
                gotoxy(x3,y3);
                printf(" ");

                gotoxy(x3,y3);
                printf(YELLOW"%c"RESET, escenario[d3][c3]);

                x3=82;
                c3=26;
                if (poder_activo==0)
                {
                    gotoxy(x3,y3);
                    printf(GREEN "%c", F1);
                }
                if (poder_activo==1)
                {
                    gotoxy(x3,y3);
                    printf(CYAN_T "%c", F1);
                }
            }
        }

    }

}



void movimiento_fantasma_morado()
{
    int contador2=1, prueba=1;
    if (poder_activo==0)
    {
        gotoxy(x4,y4);
        printf(MAGENTA_T"%c", F1);
    }
    if (poder_activo==1)
    {
        gotoxy(x4,y4);
        printf(CYAN_T "%c", F1);
        //cuenta_tiempo--;
    }

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    while(prueba==1)
    {
        if(game==0)
        {
            contador=1;
            return 0;
        }
        teclaanterior4=Fantasma4;
        if (contador2==1)
        {
            Fantasma4=rand()%4;
            switch (Fantasma4)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d4-1][c4]==COIN || escenario[d4-1][c4]==' ' || escenario[d4-1][c4] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    y4=y4-1;
                    d4=d4-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }

                    teclaNueva4=Fantasma4;
                    contador2=0;

                    //morado_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva4=teclaanterior4;
                    Fantasma4=teclaanterior4;

                    //morado_llamada_fantasmas();

                    movimiento_fantasma_morado();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();


                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d4+1][c4]==COIN || escenario[d4+1][c4]==' ' || escenario[d4+1][c4] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    y4=y4+1;
                    d4=d4+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva4=Fantasma4;
                    contador2=0;

                    //morado_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva4=teclaanterior4;
                    Fantasma4=teclaanterior4;

                    //morado_llamada_fantasmas();
                    movimiento_fantasma_morado();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d4][c4-1]== COIN || escenario[d4][c4-1]==' ' || escenario[d4][c4-1] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);

                    x4=x4-2;
                    c4=c4-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva4=Fantasma4;
                    contador2=0;

                    //morado_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva4=teclaanterior4;
                    Fantasma4=teclaanterior4;

                    //morado_llamada_fantasmas();

                    //movimiento_fantasma_morado();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d4][c4+1]== COIN || escenario[d4][c4+1]==' ' || escenario[d4][c4+1] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    x4=x4+2;
                    c4=c4+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva4=Fantasma4;
                    contador2=0;

                    //morado_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva4=teclaanterior4;
                    Fantasma4=teclaanterior4;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();

                    //morado_llamada_fantasmas();

                    movimiento_fantasma_morado();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }
        else
        {
            switch (teclaNueva4)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d4-1][c4]==COIN || escenario[d4-1][c4]==' ' || escenario[d4-1][c4] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    y4=y4-1;
                    d4=d4-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //morado_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d4+1][c4]==COIN || escenario[d4+1][c4]==' ' || escenario[d4+1][c4] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);

                    y4=y4+1;
                    d4=d4+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //morado_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d4][c4-1]== COIN || escenario[d4][c4-1]==' ' || escenario[d4][c4-1] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    x4=x4-2;
                    c4=c4-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //morado_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d4][c4+1]== COIN || escenario[d4][c4+1]==' ' || escenario[d4][c4+1] == PODER)
                {
                    gotoxy(x4,y4);
                    printf(" ");
                    gotoxy(x4,y4);
                    printf(YELLOW"%c"RESET, escenario[d4][c4]);
                    x4=x4+2;
                    c4=c4+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x4,y4);
                        printf(MAGENTA_T"%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x4,y4);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //morado_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }

        if(x4==84)
        {
            gotoxy(x4,y4);
            printf(" ");

            gotoxy(x4,y4);
            printf(YELLOW"%c"RESET, escenario[d4][c4]);

            x4=32;
            c4=1;
            if (poder_activo==0)
            {
                gotoxy(x4,y4);
                printf(MAGENTA_T"%c", F1);
            }
            if (poder_activo==1)
            {
                gotoxy(x4,y4);
                printf(CYAN_T "%c", F1);
            }
        }
        else
        {
            if(x4==30)
            {
                gotoxy(x4,y4);
                printf(" ");

                gotoxy(x4,y4);
                printf(YELLOW"%c"RESET, escenario[d4][c4]);

                x4=82;
                c4=26;
                if (poder_activo==0)
                {
                    gotoxy(x4,y4);
                    printf(MAGENTA_T"%c", F1);
                }
                if (poder_activo==1)
                {
                    gotoxy(x4,y4);
                    printf(CYAN_T "%c", F1);
                }
            }
        }

    }

}


void movimiento_fantasma_azul()
{
    int contador2=1, prueba=1;
    if (poder_activo==0)
    {
        gotoxy(x5,y5);
        printf(CYAN_T "%c", F1);
    }
    if (poder_activo==1)
    {
        gotoxy(x5,y5);
        printf(CYAN_T "%c", F1);
        //cuenta_tiempo--;
    }

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    while(prueba==1)
    {
        if(game==0)
        {
            contador=1;
            return 0;
        }
        teclaanterior5=Fantasma5;
        if (contador2==1)
        {
            Fantasma5=rand()%4;
            switch (Fantasma5)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d5-1][c5]==COIN || escenario[d5-1][c5]==' ' || escenario[d5-1][c5] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    y5=y5-1;
                    d5=d5-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }

                    teclaNueva5=Fantasma5;
                    contador2=0;

                    //azul_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva5=teclaanterior5;
                    Fantasma5=teclaanterior5;

                    //azul_llamada_fantasmas();

                    movimiento_fantasma_azul();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();


                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d5+1][c5]==COIN || escenario[d5+1][c5]==' ' || escenario[d5+1][c5] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    y5=y5+1;
                    d5=d5+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva5=Fantasma5;
                    contador2=0;

                    //azul_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva5=teclaanterior5;
                    Fantasma5=teclaanterior5;

                    //azul_llamada_fantasmas();
                    movimiento_fantasma_azul();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d5][c5-1]== COIN || escenario[d5][c5-1]==' ' || escenario[d5][c5-1] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);

                    x5=x5-2;
                    c5=c5-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva5=Fantasma5;
                    contador2=0;

                    //azul_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva5=teclaanterior5;
                    Fantasma5=teclaanterior5;

                    //azul_llamada_fantasmas();

                    movimiento_fantasma_azul();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d5][c5+1]== COIN || escenario[d5][c5+1]==' ' || escenario[d5][c5+1] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    x5=x5+2;
                    c5=c5+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva5=Fantasma5;
                    contador2=0;

                    //azul_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva5=teclaanterior5;
                    Fantasma5=teclaanterior5;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();

                    //azul_llamada_fantasmas();

                    movimiento_fantasma_azul();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }
        else
        {
            switch (teclaNueva5)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d5-1][c5]==COIN || escenario[d5-1][c5]==' ' || escenario[d5-1][c5] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    y5=y5-1;
                    d5=d5-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //azul_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d5+1][c5]==COIN || escenario[d5+1][c5]==' ' || escenario[d5+1][c5] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);

                    y5=y5+1;
                    d5=d5+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //azul_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d5][c5-1]== COIN || escenario[d5][c5-1]==' ' || escenario[d5][c5-1] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    x5=x5-2;
                    c5=c5-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //azul_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d5][c5+1]== COIN || escenario[d5][c5+1]==' ' || escenario[d5][c5+1] == PODER)
                {
                    gotoxy(x5,y5);
                    printf(" ");
                    gotoxy(x5,y5);
                    printf(YELLOW"%c"RESET, escenario[d5][c5]);
                    x5=x5+2;
                    c5=c5+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x5,y5);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    Sleep(150);

                    //azul_llamada_fantasmas();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }

        if(x5==84)
        {
            gotoxy(x5,y5);
            printf(" ");

            gotoxy(x5,y5);
            printf(YELLOW"%c"RESET, escenario[d5][c5]);


            x5=32;
            c5=1;
            if (poder_activo==0)
            {
                gotoxy(x5,y5);
                printf(CYAN_T "%c", F1);
            }
            if (poder_activo==1)
            {
                gotoxy(x5,y5);
                printf(CYAN_T "%c", F1);
            }
        }
        else
        {
            if(x5==30)
            {
                gotoxy(x5,y5);
                printf(" ");

                gotoxy(x5,y5);
                printf(YELLOW"%c"RESET, escenario[d5][c5]);

                x5=82;
                c5=26;
                if (poder_activo==0)
                {
                    gotoxy(x5,y5);
                    printf(CYAN_T "%c", F1);
                }
                if (poder_activo==1)
                {
                    gotoxy(x5,y5);
                    printf(CYAN_T "%c", F1);
                }
            }
        }

    }

}

void rojo_llamada_fantasmas()
{
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);

    movimiento_fantasma_morado();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);

    movimiento_fantasma_azul();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);

    movimiento_fantasma_verde();


    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
}

void morado_llamada_fantasmas()
{

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_azul();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_verde();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);

    movimiento_fantasma_rojo();
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
}
void verde_llamada_fantasmas()
{
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_rojo();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //Sleep(1);
    //sleep(1);
    movimiento_fantasma_azul();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);

    movimiento_fantasma_morado();
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
}
void azul_llamada_fantasmas()
{
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_verde();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_rojo();

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
    movimiento_fantasma_morado();
    validacion();
    if (vida<=0)
    {
        return 0;
    }
    //sleep(1);
}
void validacion()
{

    if (cuenta_tiempo<=0)
    {
        //tiempo de color termino
        cuenta_tiempo=100;
        poder_activo=0;
    }
    //si fantasma se come a pacman entonces pacman vida -1 y fantasma vuelve a su posicion inicial
    if(x==x2 && y==y2 && poder_activo==0 ||x==x3 && y==y3 && poder_activo==0 || x==x4 && y==y4 && poder_activo==0 || x==x5 && y==y5 && poder_activo==0)
    {
        gotoxy(x,y);
        printf(" ");
        x=58, y=24, d=23, c=14;
        gotoxy(x,y);
        printf(" ");
        //gotoxy(x,y);
        //printf(" ");
        vida=vida-1;
        //gotoxy(0,0);
        if (vida==2)
        {
            if(count > 0)
            {
                count=count -100;
            }
            //gotoxy(x,y);
            //printf(" ");
            //Sleep(500);
        }
        if (vida==1)
        {
            if(count > 0)
            {
                count=count -100;
            }
            //gotoxy(x,y);
            //printf(" ");
            //Sleep(500);
        }
        if (vida<=0)
        {
            if(count > 0)
            {
                vida=0;
                count=count -100;
            }
            gotoxy(0,0);
            printf(BLANCO_T "Vidas: %d", vida);
            //gotoxy(x,y);
            //printf(" ");
            //Sleep(500);
            return 0;
        }
        gotoxy(0,0);
        printf(BLANCO_T "Vidas: %d", vida);
    }
    //si poder se pone de color los fantasmas por 10 segundos y sse los puede comer
    //si fantasma es comido vuelva a la posicion inicial
    //si fantasma topa ccn pacman pacman pierde vida
    //si come se activa variable y esa se valida

    if (cuenta_tiempo<=0)
    {
        //tiempo de color termino
        cuenta_tiempo=150;
        poder_activo=0;
    }
    //si se come el cuadrado de poder le da color y se los puede comer
    switch (Mpacman)
    {
    case 'W':
    case 'w':
    {
        if(escenario[d-1][c] == PODER)
        {
            poder_activo=1;
        }
        break;
    }

    case 'S':
    case 's':
    {
        if(escenario[d+1][c] == PODER)
        {
            poder_activo=1;
        }
        break;
    }
    case 'a':
    case 'A':
    {
        if(escenario[d][c-1] == PODER)
        {
            poder_activo=1;
        }
        break;
    }
    case 'd':
    case 'D':
    {
        if(escenario[d][c+1] == PODER)
        {
            poder_activo=1;
        }
        break;
    }

    } //despues de terminaado tiempo vuelve a poder_activo=0
    if(x==x2 && y==y2 && poder_activo==1 || x==x3 && y==y3 && poder_activo==1 ||x==x4 && y==y4 && poder_activo==1||x==x5 && y==y5 && poder_activo==1)
    {
        count=count+100;
        gotoxy(x,y);
        printf(" ");
        gotoxy(x,y);
        printf(GREEN "%c", PM);
        gotoxy(x,y);
        printf(" ");
        gotoxy(x,y);
        printf(YELLOW "%c", PM);
        if (x==x2 && y==y2)
        {
            //fantasma1
            x2=58, y2=12, d2=11, c2=14;
        }
        if (x==x3 && y==y3)
        {
            //fantasma2
            x3=58, y3=12, d3=11, c3=14;
        }
        if (x==x4 && y==y4)
        {
            //fantasma3
            x4=58, y4=12, d4=11, c4=14;
        }
        if (x==x5 && y==y5)
        {
            //fantasma4
            x5=58, y5=12, d5=11, c5=14;
        }

    }
}


void movimiento_pacman()
{
    gotoxy(x,y);
    printf( YELLOW "%c", PM);
    game=1;
    while(game!=0)
    {
        game=terminar_juego();
        if(game==0)
        {
            contador=1;
            return 0;
        }
        validacion();
        if (vida<=0)
        {
            return 0;
        }
        gotoxy(0,5);
        printf("%d", game);
        gotoxy(22,24);
        printf( BLANCO_T "%d" RESET, count);
        teclaant=Mpacman;

        if (_kbhit()) // ENTRA AL IF CUANDO SE PRESIONA UNA TECLA
        {
            Mpacman = _getch();
            switch (Mpacman)
            {
            case 'W':
            case 'w':
            {
                // MOVIMIENTO W
                if(escenario[d-1][c]==COIN || escenario[d-1][c]==' ' || escenario[d-1][c] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    y=y-1;
                    d=d-1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    teclaNew=Mpacman;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_rojo();
                    rojo_llamada_fantasmas();
                }
                else
                {
                    teclaNew=teclaant;
                    Mpacman=teclaant;
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                break;
            }

            case 'S':
            case 's':
            {
                // MOVIMIENTO S
                if(escenario[d+1][c]==COIN || escenario[d+1][c]==' ' || escenario[d+1][c] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ", PM);
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    y=y+1;
                    d=d+1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    teclaNew=Mpacman;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_morado();
                    morado_llamada_fantasmas();
                }
                else
                {
                    teclaNew=teclaant;
                    Mpacman=teclaant;
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                break;
            }
            case 'a':
            case 'A':
            {
                if(escenario[d][c-1]== COIN || escenario[d][c-1]==' ' || escenario[d][c-1] == PODER)
                {
                    gotoxy(x,y);
                    printf("  ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    x=x-2;
                    c=c-1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    teclaNew=Mpacman;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_verde();
                    verde_llamada_fantasmas();
                }
                else
                {
                    teclaNew=teclaant;
                    Mpacman=teclaant;
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                break;
            }
            case 'd':
            case 'D':
            {
                if(escenario[d][c+1]== COIN || escenario[d][c+1]==' ' || escenario[d][c+1] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    x=x+2;
                    c=c+1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    teclaNew=Mpacman;
                    /*validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }*/
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_azul();
                    azul_llamada_fantasmas();
                }
                else
                {
                    teclaNew=teclaant;
                    Mpacman=teclaant;
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                break;
            }
            case 'f':
            {
                exit(0);
                break;
            }
            }
        }
        else
        {
            switch (teclaNew)
            {
            case 'W':
            case 'w':
            {
                // MOVIMIENTO W
                if(escenario[d-1][c]==COIN || escenario[d-1][c]==' ' || escenario[d-1][c] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    y=y-1;
                    d=d-1;
                    gotoxy(x,y);
                    printf( YELLOW "%c", PM);
                    Sleep(150);

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_rojo();
                    rojo_llamada_fantasmas();
                }
                else
                {
                    return 0;
                }
                break;
            }

            case 'S':
            case 's':
            {
                // MOVIMIENTO S
                if(escenario[d+1][c]==COIN || escenario[d+1][c]==' ' || escenario[d+1][c] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ", PM);
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    y=y+1;
                    d=d+1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    Sleep(150);

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_morado();
                    morado_llamada_fantasmas();
                }
                else
                {
                    return 0;
                }
                break;
            }
            case 'a':
            case 'A':
            {
                if(escenario[d][c-1]== COIN || escenario[d][c-1]==' ' || escenario[d][c-1] == PODER)
                {
                    gotoxy(x,y);
                    printf("  ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    x=x-2;
                    c=c-1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    Sleep(150);

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_verde();
                    verde_llamada_fantasmas();
                    //verde_llamada_fantasmas();
                }
                else
                {
                    return 0;
                }
                break;
            }
            case 'd':
            case 'D':
            {
                if(escenario[d][c+1]== COIN || escenario[d][c+1]==' ' || escenario[d][c+1] == PODER)
                {
                    gotoxy(x,y);
                    printf(" ");
                    count = contar_monedas(count, c, d);
                    escenario[d][c]=' ';
                    x=x+2;
                    c=c+1;
                    gotoxy(x,y);
                    printf(YELLOW "%c", PM);
                    Sleep(150);

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_fantasma_azul();
                    azul_llamada_fantasmas();
                    //azul_llamada_fantasmas();
                }
                else
                {
                    return 0;
                }
                break;
            }
            case 'f':
            {
                exit(0);
                break;
            }
            }

        }

        if(x==84)
        {
            gotoxy(x,y);
            printf(" ");
            x=32;
            c=1;
            gotoxy(x,y);
            printf("%c", PM);
        }
        else
        {
            if(x==30)
            {
                gotoxy(x,y);
                printf(" ");
                x=82;
                c=26;
                gotoxy(x,y);
                printf("%c", PM);
            }
        }
    }
}

int main()
{
#ifdef _WIN32
    system("cls");
#endif
    system("mode con: cols=115 lines=34");
    //demo();
    cargarGanadores();
    //Para hacer acomodar la pantalla antes de iniciar el juego
    int cualquiera;
    printf("INGRESA ALGO: ");
    scanf("%d", &cualquiera);

    llenar_Escenario();
    Crear_Bordes();
    char opt;

    srand (time(NULL));
    marco();

    do
    {
        limpiarPantalla();
        letraPacMan();
        dibujoFantasmitas();
        gotoxy(46,12);
        printf(RESET "1. Empezar juego");
        gotoxy(46,14);
        printf("2. Ver ganadores");
        gotoxy(46,16);
        printf("3. Salir");
        gotoxy(46,18);
        printf("Escoja una opcion: ");
        opt=PacManMusica();
        limpiarPantalla();
        switch(opt)
        {
        case '1':
        {
            gotoxy(0,0);
            printf("                    ");
            gotoxy(0,0);
            printf("Vidas: %d", vida);
            barraDeCarga();
            limpiarPantalla();
            colocarMonedas();
            Mostrar_Bordes();
            //pacman
            vida=3;
            gotoxy(0,0);
            printf("Vidas: %d", vida);
            count=0; //monedas
            x=58;
            y=24;
            d=23;
            c=14;
            //Fanstama3-verde
            Fantasma3=' ', teclaanterior3, teclaNueva3;
            x3=58, y3=12, d3=11, c3=14;

            //Fantasma4 - morado
            Fantasma4=' ', teclaanterior4, teclaNueva4;
            x4=58, y4=12, d4=11, c4=14;

            //Fanstasma5 - azul
            Fantasma5=' ', teclaanterior5, teclaNueva5;
            x5=58, y5=12, d5=11, c5=14;

            Mpacman=' ';
            teclaNew=' ';
            contador=0;
            while (contador==0)
            {
                if (vida>0)
                {
                    movimiento_pacman();
                }
                if (vida<=0)
                {
                    contador=1;
                }
                if (vida>0)
                {
                    movimiento_fantasma_rojo();
                }
                if (vida<=0)
                {
                    contador=1;
                }
                if (vida>0)
                {
                    movimiento_fantasma_azul();
                }
                if (vida<=0)
                {
                    contador=1;
                }
                if (vida>0)
                {
                    movimiento_fantasma_verde();
                }
                if (vida<=0)
                {
                    contador=1;
                }
                if (vida>0)
                {
                    movimiento_fantasma_morado();
                }
                if (vida<=0)
                {
                    contador=1;
                }
            }
            limpiarPantalla();
            if (vida<=0)
            {
                gotoxy(40,16);
                printf("PERDISTE!!!");
            }
            if (vida>0)
            {
                gotoxy(40,16);
                printf("GANASTE!!!");
            }
            struct NodoJugador *jugador = crearNodoJugador(count);
            ingresoJugador(jugador);
            getch();
            guardarGanadores();

        }
        break;

        case '2':
        {
            leerGanadores();
        }
        break;

        case '3':
        {
            eliminarArchivo();
            gotoxy(40,17);
            printf("Gracias por jugar PacMan!!!");
        }
        break;
        }
    }
    while(opt != '3');

    gotoxy(0,40);
    return 0;
}


void movimiento_fantasma_rojo()
{

    int contador2=1, prueba=1;
    if (poder_activo==0)
    {
        gotoxy(x2,y2);
        printf(RED "%c", F1);
    }
    if (poder_activo==1)
    {
        gotoxy(x2,y2);
        printf(CYAN_T "%c", F1);
    }

    validacion();
    if (vida<=0)
    {
        return 0;
    }
    while(prueba==1)
    {
        if(game==0)
        {
            contador=1;
            return 0;
        }
        teclaanterior=Fantasma1;
        if (contador2==1)
        {
            Fantasma1=rand()%4;
            switch (Fantasma1)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d2-1][c2]==COIN || escenario[d2-1][c2]==' ' || escenario[d2-1][c2] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    y2=y2-1;
                    d2=d2-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva=Fantasma1;
                    contador2=0;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva=teclaanterior;
                    Fantasma1=teclaanterior;

                    movimiento_fantasma_rojo();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();


                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d2+1][c2]==COIN || escenario[d2+1][c2]==' ' || escenario[d2+1][c2] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    y2=y2+1;
                    d2=d2+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva=Fantasma1;
                    contador2=0;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva=teclaanterior;
                    Fantasma1=teclaanterior;

                    movimiento_fantasma_rojo();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d2][c2-1]== COIN || escenario[d2][c2-1]==' ' || escenario[d2][c2-1] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);

                    x2=x2-2;
                    c2=c2-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva=Fantasma1;
                    contador2=0;

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva=teclaanterior;
                    Fantasma1=teclaanterior;

                    movimiento_fantasma_rojo();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d2][c2+1]== COIN || escenario[d2][c2+1]==' ' || escenario[d2][c2+1] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    x2=x2+2;
                    c2=c2+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        cuenta_tiempo--;
                    }
                    teclaNueva=Fantasma1;
                    contador2=0;
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                }
                else
                {
                    teclaNueva=teclaanterior;
                    Fantasma1=teclaanterior;

                    movimiento_fantasma_rojo();

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }

                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }
        else
        {
            switch (teclaNueva)
            {
            case 0:
            {
                // MOVIMIENTO W
                if(escenario[d2-1][c2]==COIN || escenario[d2-1][c2]==' ' || escenario[d2-1][c2] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    y2=y2-1;
                    d2=d2-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                        Sleep(150);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        Sleep(150);
                        cuenta_tiempo--;
                    }

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }

            case 1:
            {
                // MOVIMIENTO S
                if(escenario[d2+1][c2]==COIN || escenario[d2+1][c2]==' ' || escenario[d2+1][c2] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);

                    y2=y2+1;
                    d2=d2+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                        Sleep(150);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        Sleep(150);
                        cuenta_tiempo--;
                    }
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 2:
            {
                if(escenario[d2][c2-1]== COIN || escenario[d2][c2-1]==' ' || escenario[d2][c2-1] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    x2=x2-2;
                    c2=c2-1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                        Sleep(150);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        Sleep(150);
                        cuenta_tiempo--;
                    }
                    //Sleep(150);

                    //rojo_llamada_fantasmas();
                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 3:
            {
                if(escenario[d2][c2+1]== COIN || escenario[d2][c2+1]==' ' || escenario[d2][c2+1] == PODER)
                {
                    gotoxy(x2,y2);
                    printf(" ");
                    gotoxy(x2,y2);
                    printf(YELLOW"%c"RESET, escenario[d2][c2]);
                    x2=x2+2;
                    c2=c2+1;
                    if (poder_activo==0)
                    {
                        gotoxy(x2,y2);
                        printf(RED "%c", F1);
                        Sleep(150);
                    }
                    if (poder_activo==1)
                    {
                        gotoxy(x2,y2);
                        printf(CYAN_T "%c", F1);
                        Sleep(150);
                        cuenta_tiempo--;
                    }

                    validacion();
                    if (vida<=0)
                    {
                        return 0;
                    }
                    movimiento_pacman();
                }
                else
                {
                    prueba=0;
                    return 0;
                }
                break;
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }

        if(x2==84)
        {
            gotoxy(x2,y2);
            printf(" ");

            gotoxy(x2,y2);
            printf(YELLOW"%c"RESET, escenario[d2][c2]);


            x2=32;
            c2=1;
            if (poder_activo==0)
            {
                gotoxy(x2,y2);
                printf(RED "%c", F1);
            }
            if (poder_activo==1)
            {
                gotoxy(x2,y2);
                printf(CYAN_T "%c", F1);
            }
        }
        else
        {
            if(x2==30)
            {
                gotoxy(x2,y2);
                printf(" ");

                gotoxy(x2,y2);
                printf(YELLOW"%c"RESET, escenario[d2][c2]);

                x2=82;
                c2=26;
                if (poder_activo==0)
                {
                    gotoxy(x2,y2);
                    printf(RED "%c", F1);
                }
                if (poder_activo==1)
                {
                    gotoxy(x2,y2);
                    printf(CYAN_T "%c", F1);
                }
            }
        }

    }

}

