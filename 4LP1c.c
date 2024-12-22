/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas las funciones que debéis 
 * implementar en ensamblador.
 * Desde este código se realizan las llamadas a las subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO SE DEBE ENTREGAR.
 **/

#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

/**
 * Constantes.
 **/
#define ROWSMATRIX 6
#define COLSMATRIX 7
#define DISCSYMBOLPLAYER1 'X'
#define DISCSYMBOLPLAYER2 'O'


/**
 * Definición de variables globales.
 **/
extern int developer; //Variable declarada en ensamblador que indica el nombre del programador

char charac;      //Carácter leído del teclado y para escribir en pantalla.
int  rowScreen;   //Fila para posicionar el cursor en la pantalla.
int  colScreen;   //Columna para posicionar el cursor en la pantalla

int  row;         //Fila de la matriz mBoard.
int  col;         //Columna de la matriz mBoard.
long indexMat;    //Índice para acceder a la matriz mBoard en ensamblador.

char discSymbol;  //Símbolo del disco del jugador que está jugando.
int  colCursor = 3; //Columna donde está el cursor [0..6]
unsigned char fourINaLINE = 0; //0=false, 1=true
int  state = 1;   // Estado del juego
                  // 0: Se ha pulsado ESC para salir   
                  // 1: Juega el jugador 1.
                  // 2: Juega el jugador 2.
                  // 3: El jugador 1 ha hecho 4 en línea. (en la P2)
                  // 4: El jugador 2 ha hecho 4 en línea. (en la P2)
                  // 5: El tablero está lleno. Empate.
int  newState = 1;

//Matriz 6x7 que contiene el estado del tablero       0   1   2   3   4   5   6
char mBoard[ROWSMATRIX][COLSMATRIX] = { /*0*/ {' ',' ',' ','O',' ',' ',' '},
                                        /*1*/ {'O',' ','O','X',' ',' ',' '},
                                        /*2*/ {'X',' ','O','O',' ',' ',' '},
                                        /*3*/ {'X',' ','O','X',' ','X',' '},
                                        /*4*/ {'O',' ','X','O','X','O',' '},
                                        /*5*/ {'O','X','O','X','X','X',' '} };
int freeRowXcol[COLSMATRIX] =                 { 0, 4, 0, -1, 3, 2, 5 };
int direction[2][8] = { { -1, +1, +1, -1,  0,  0, -1, +1},   //modificación de fila
                        { -1, +1, -1, +1, -1, +1,  0,  0} }; //modificación de columna


/**
 * Definición de las funciones en C.
 **/
void clearScreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();
void printMenuP1_C();
void printBoardP1_C();

void calcIndexP1_C(); 
void updateBoardP1_C(); 
void showDiscPosP1_C();
void moveCursorP1_C();
void insertDiscP1_C();
void checkLineP1_C();
void checkEndP1_C();

void printMessageP1_C();
void playP1_C();

/**
 * Definición de las subrutinas en ensamblador que se llaman desde C.
 **/
void calcIndexP1(); 
void updateBoardP1(); 
void showDiscPosP1();
void moveCursorP1();
void insertDiscP1();
void checkLineP1();
void checkEndP1();

void printMessageP1();
void playP1();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en la fila indicada por la variable (rowScreen) y en 
 * la columna indicada por la variable (colScreen) de la pantalla.
 * 
 * Variables globales utilizadas:   
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP1' para 
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void gotoxyP1_C(){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter guardado en la variable (charac) en la pantalla, 
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que queremos mostrar.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Leer una tecla y guardar el carácter asociado en la variable (charac)
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que leemos del teclado.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'getchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándar (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

   /* ~ICANON para tratar la entrada del teclado carácter a carácter no como una línea completa acabada con /n
      ~ECHO para que no muestre el carácter leído*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. **/
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   charac = (char) getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Mostrar en la pantalla el menú del juego y pedir una opción.
 * Solo acepta una de las opciones correctas del menú ('0'-'8')
 * 
 * Variables globales utilizadas:   
 * (developer):((char;)&developer): variable definida en el código ensamblador.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : Opción elegida del menú, leída del teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void printMenuP1_C(){
	
  clearScreen_C();
  rowScreen = 1;
  colScreen = 1;
  gotoxyP1_C();
  printf("                            \n");
  printf("      Developed by:         \n");
  printf("     ( %s )    \n",(char *)&developer);
  printf(" __________________________ \n");
  printf("|                          |\n");
  printf("|   MENU 4 IN a LINE v1.0  |\n");
  printf("|__________________________|\n");
  printf("|                          |\n");
  printf("|     1.  CalcIndex        |\n");
  printf("|     2.  UpdateBoard      |\n");
  printf("|     3.  ShowDisc&Pos     |\n");
  printf("|     4.  MoveCursor       |\n");
  printf("|     5.  InsertDisc       |\n");
  printf("|     6.  -(new in P2)-    |\n");
  printf("|     7.  CheckEnd         |\n");
  printf("|     8.  Play Game        |\n");
  printf("|     9.  Play Game C      |\n");
  printf("|     0.  Exit             |\n");
  printf("|                          |\n");
  printf("|         OPTION:          |\n");
  printf("|__________________________|\n"); 
   
  charac =' ';
  while ((charac < '0' || charac > '9') && charac!=27) {
    rowScreen = 20;
    colScreen = 19;
    gotoxyP1_C();
    getchP1_C();
  }
   
}


/**
 * Mostrar el tablero de juego en la pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printBoardP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void printBoardP1_C(){
   int i;

   clearScreen_C();
   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();                                        //ScreenRows   
   printf(" _____________________________________ \n"); //01
   printf("|                                     |\n"); //02
   printf("|             4 IN a LINE             |\n"); //03
   printf("|                                     |\n"); //04
   printf("|                                     |\n"); //05
//Screen Colums  08  12  16  20  24  28  32      
   printf("|    +---+---+---+---+---+---+---+    |\n"); //06
   printf("|  0 |   |   |   |   |   |   |   |    |\n"); //07
   printf("|    +---+---+---+---+---+---+---+    |\n"); //08
   printf("|  1 |   |   |   |   |   |   |   |    |\n"); //09
   printf("|    +---+---+---+---+---+---+---+    |\n"); //10
   printf("|  2 |   |   |   |   |   |   |   |    |\n"); //11
   printf("|    +---+---+---+---+---+---+---+    |\n"); //12
   printf("|  3 |   |   |   |   |   |   |   |    |\n"); //13
   printf("|    +---+---+---+---+---+---+---+    |\n"); //14
   printf("|  4 |   |   |   |   |   |   |   |    |\n"); //15
   printf("|    +---+---+---+---+---+---+---+    |\n"); //16
   printf("|  5 |   |   |   |   |   |   |   |    |\n"); //17
   printf("|    +---+---+---+---+---+---+---+    |\n"); //18  
   printf("|      0   1   2   3   4   5   6      |\n"); //19
   printf("|                                     |\n"); //20
   printf("|    (ESC)Exit   (Space)Inser Disc    |\n"); //21
   printf("|     (k) Left     (l)  Right         |\n"); //22
   printf("|_____________________________________|\n"); //23
   printf("|                                     |\n"); //24
   printf("|                                     |\n"); //25
   printf("|_____________________________________|\n"); //26
             
}


/**
 * Calcular el índice para acceder a la matriz (mBoard) en ensamblador.
 * (mBoard[row][col]) en C, es ([mBoard+indexMat]) en ensamblador.
 * donde indexMat = row*COLSMATRIX+col.
 * La matriz (mBoard) es de tipo char(BYTE)1byte.
 * 
 * Variables globales utilizadas:	
 * (row)     : Fila de la matriz mBoard.
 * (col)     : Columna de la matriz mBoard.
 * (indexMat): Índice para acceder a la matriz mBoard en ensamblador.
 * 
 **** Esta función no es necesaria en C, solo en ensamblador.****
 * Hay una subrutina de ensamblador equivalente 'calcIndexP1'.
 */
void calcIndexP1_C(){
	
   indexMat = (long)(row*COLSMATRIX+col);

}


/**
 * Mostrar los valores de la matriz (mBoard)
 * dentro del tablero, en las posiciones correspondientes. 
 * Se debe recorrer toda la matriz (mBoard), de izquierda a derecha y 
 * de arriba a abajo, desde la posición [0][0]=(0) hasta la posición [5][6]=(41),
 * cada posición es de tipo char(BYTE)1byte, y para cada elemento hacer:
 * Posicionar el cursor en el tablero en función de las variables 
 * (rowScreen) fila y (colScreen) columna llamando la función gotoxyP1_C.
 * Las variables (rowScreen) y (colScreen) se inicializarán en 7 y 8, 
 * respectivamente, que es la posición en pantalla de la casilla [0][0].
 * Las filas se incrementan de 2 en 2 y las columnas de 4 en 4.
 * Mostrar los caracteres de cada posición de la matriz (mBoard) llamando
 * la función printchP1_C.
 *  
 * Variables globales utilizadas:	
 * (mBoard): Dirección de la matriz donde guardamos los discos introducidos.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'updateBoardP1'.
 **/
void  updateBoardP1_C(){
   
   int i,j;
   rowScreen = 7;
   for (i=0;i<ROWSMATRIX;i++){
	  colScreen = 8;
      for (j=0;j<COLSMATRIX;j++){
         gotoxyP1_C();
         charac = mBoard[i][j];
         printchP1_C();
         colScreen = colScreen + 4;
      }
      rowScreen = rowScreen + 2;
   }
   
}


/**
 * Muestra en la parte superior del tablero el símbolo del jugador (discSymbol)
 * que está jugando en la columna donde está el cursor (colCursor)
 * y posiciona el cursor en la posición del tablero
 * donde caerá el disco en esa columna.
 * El símbolo del jugador se muestra en la fila (rowScreen=5) y la columna
 * (colScreen=8+colCursor*4).
 * Después posicionar el cursor en la misma columna y 
 * en la fila donde caerá el disco si se presiona espacio.
 * (rowScreen = 7+freeRowXcol[colCursor]*2)
 * La matriz freeRowXcol es de tipo int(DWORD)4bytes.
 * Posicionar el cursor en el tablero en función de las variables 
 * (rowScreen) fila y (colScreen) columna llamando la función gotoxyP1_C.
 * Mostrar el símbolo (discSymbol) llamando la función printchP1_C.
 *  
 * Variables globales utilizadas:	
 * (discSymbol) : Símbolo del jugador que está jugando.
 * (colCursor)  : Columna donde está el cursor.
 * (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'showDiscPosP1'.
 **/
void showDiscPosP1_C(){
	
  rowScreen = 5;
  colScreen = 8+colCursor*4;
  gotoxyP1_C();
  charac = discSymbol;
  printchP1_C();
  rowScreen = 7+freeRowXcol[colCursor]*2;
  //colScreen = 8+colCursor*4;
  gotoxyP1_C();
  
}
 
 
/**
 * Actualizar la columna donde está el cursor (colCursor).
 * Si se ha leído (charac=='k') izquierda o (charac=='l') derecha 
 * actualizar la posición del cursor (colCursor +/- 1)
 * controlando que no salga del tablero [0..(COLSMATRIX-1)]. 
 *  
 * Variables globales utilizadas:	
 * (charac)   : Carácter leído desde el teclado.
 * (colCursor): Columna donde está el cursor.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'moveCursorP1'.
 **/
void moveCursorP1_C(){
	
   if ((charac=='k') && (colCursor>0)){             
      colCursor--;
   } else
   if ((charac=='l') && (colCursor<(COLSMATRIX-1))){
      colCursor++;
   }
   
}

/**
 * Inserta el disco (discSymbol) del jugador en la columna donde está 
 * el cursor (colCursor) y en la primera fila libre de esa columna.
 * La primera fila libre (row) de una columna dentro de la matriz (mBoard) 
 * la tenemos guardada en la columna (colCursor) del vector (freeRowXcol)
 * (freRowXcol[colCursor]).
 * Si podemos introducir el disco (row>=0):
 *   Si el estado del juego es (state==1) el (discSymbol = DISCSYMBOLPLAYER1),
 *   si el estado del juego es (state==2) el (discSymbol = DISCSYMBOLPLAYER2).
 *   Poner el símbolo (discSymbol) en la matriz (mBoard) en la primera fila 
 *   libre (row) y en la columna donde está el cursor (colCursor).
 *   Decrementar la fila libre del vector (freeColXrow) de la columna donde
 *   hemos insertado el disco (colCursor).
 *   Cambiamos de jugador, de jugador 1 a jugador 2 y de jugador 2 a jugador 1
 *   (state = 3 - state).
 * Si no queda espacio en esa columna (row=-1) no insertamos el disco.
 * Devolver estado del juego.
 * 
 * Variables globales utilizadas:	
 * (row)        : Fila de la matriz mBoard.
 * (colCursor)  : Columna donde está el cursor.
 * (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
 * (mBoard)     : Dirección de la matriz donde guardamos los discos introducidos. 
 * (discSymbol) : Símbolo del jugador que está jugando.
 * (state)      : Estado del juego.
 * (newState)   : Estado del juego.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'insertDiscP1'.
 **/
void insertDiscP1_C(){

  row = freeRowXcol[colCursor];
  if(row >= 0) {
    if (state == 1) discSymbol = DISCSYMBOLPLAYER1; else discSymbol = DISCSYMBOLPLAYER2;
	mBoard[row][colCursor] = discSymbol;
	freeRowXcol[colCursor]--;
	newState = 3 - state;
  }
  
}


/**
 * Verifica si el tablero está lleno y no se puede continuar jugando.
 * El tablero está lleno si la primera fila libre de todas las columnas
 * indicado en el vector (freeRoxWcol) son -1.
 * Si se recorre todo el vector (freeRowXcol) y todas las posiciones
 * valen -1, (c==COLSMATRIX) pondremos (state=5) para indicar que
 * el tablero está lleno y no se puede continuar jugando.
 * 
 * Variables globales utilizadas:	
 * (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
 * (state)      : Estado del juego.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'checkEndP1'.
 **/
void checkEndP1_C(){

  int c=0;
  while (freeRowXcol[c] == -1 && c < COLSMATRIX){
    c++;
  }
  if (c == COLSMATRIX) state = 5;
  
}
 

/**
 * Muestra un mensaje en la parte inferior del tablero según el 
 * valor de la variable (state).
 * (state) 0: Se ha pulsado ESC para salir 
 *         1: Juega el jugador 1.
 *         2: Juega el jugador 2.
 *         3: El jugador 1 ha hecho 4 en línea.
 *         4: El jugador 2 ha hecho 4 en línea.
 *         5: El tablero está lleno. Empate.
 * 
 * Variables globales utilizadas:	
 * (state): Estado del juego.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printMessageP1' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void printMessageP1_C(){

   rowScreen = 25;
   colScreen = 4;
   gotoxyP1_C();
   switch(state){
	 case 0:
       printf("       EXIT: (ESC) PRESSED ");
     break;
     case 1: 
       printf("       Play: PLAYER 1 (%c)       ", DISCSYMBOLPLAYER1);
     break;
     case 2:
       printf("       Play: PLAYER 2 (%c)       ", DISCSYMBOLPLAYER2);
     break;
     case 3:
       printf("      PLAYER 1 (%c): WIN!!!      ", DISCSYMBOLPLAYER1);
     break;
     case 4:
       printf("      PLAYER 2 (%c): WIN!!!      ", DISCSYMBOLPLAYER2);
     break;
     case 5:
       printf("  GAME OVER: Grid Full! = DRAW = ");
     break;
   }
   
}


/**
 * Función principal del juego
 * Muestra el tablero de juego y deja hacer las jugadas de los 2 jugadores,
 * alternativamente, hasta que uno de los dos jugadores pone 4 discos en línea
 * o el tablero queda lleno y nadie ha hecho ninguna línea de 4.
 * 
 * Pseudo-código:
 * Mientras estén jugando el jugador 1 (state=1) o el jugador 2 (state = 2) hacer:
 *   Mostrar las líneas del tablero llamando la función printBoardP2_C.
 *   Actualizar el estado del tablero llamando la función updateBoardP2_C.
 *   Mostrar un mensaje según el estado del juego (state) llamando 
 *   la función printMessageP2_C.
 *   Si (state==1) asignar a (discSymbol) el símbolo del jugador 1
 *   (DISCSYMBOLPLAYER1='X'), si no el símbolo del jugador 2
 *   (DISCSYMBOLPLAYER1='O').
 *   Leer una tecla llamando la función getchP2_C.
 *   Si la tecla leída es 'k' o 'l' mover el cursor sin salir del
 *   tablero llamando la función moveCursorP2_C.
 *   Si la tecla leída es ' ' 
 *     Insertar el disco en el tablero (mBoard) en la columna actual del 
 *     cursor (colCursor) llamando la función insertDiscP2_C.
 *     Si se ha introducido el disco, (state != newState)
 *       Verificar si el tablero está lleno 
 *       llamando la función checkEndP2_C.
 *       Si no, si se ha hecho 4 en raya o el tablero está lleno (state <= 2)
 *       actualizar el estado del juego (state = newState).
 *   Si la tecla es ESC(ASCII 27) poner (state=0) para indicarlo.
 * Actualizar el estado del tablero llamando la función updateBoardP2_C.
 * Mostrar un mensaje según el estado del juego (state) llamando 
 * la función printMessageP2_C.
 * Se acaba el juego.
 * 
 * Variables globales utilizadas:
 * (colCursor)  : Columna donde está el cursor.
 * (state)      : Estado del juego.
 * (newState)   : Estado del juego.
 * (discSymbol) : Símbolo del jugador que está jugando.
 * (charac)     : Carácter que leemos del teclado.
 *  
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'playP1',  
 * el paso de parámetros es equivalente.
 **/
void playP1_C(){
  
  colCursor = 3;
  state = 1;
  while (state == 1 || state ==2  ) {
    printBoardP1_C();
    updateBoardP1_C();
    printMessageP1_C();
    if (state == 1) discSymbol = DISCSYMBOLPLAYER1; else discSymbol = DISCSYMBOLPLAYER2;  
    showDiscPosP1_C();
    getchP1_C();   
    if (charac == 'k'  || charac == 'l') {
      moveCursorP1_C();
    }
    if (charac == ' ' ) {
      insertDiscP1_C();
      if(state != newState){ //nuevo disco insertado
        checkEndP1_C();
      }
      if (state <= 2) state = newState;
    }
    if (charac == 27) {
       state = 0;
    }
  }
  updateBoardP1_C();
  printMessageP1_C();
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓN: Podéis probar la funcionalidad de las subrutinas que se deben
 * desarrollar quitando los comentarios de la llamada a la función 
 * equivalente implementada en C que hay bajo cada opción.
 * Para el juego completo hay una opción para la versión en ensamblador y 
 * una opción para el juego en C.
 **/
void main(void){
  char c;
  int op=' ';
  while (op!='0' && op!=27) {
    printMenuP1_C();	  
    op = charac;
    switch(op){
      case 27:
      case '0':
        rowScreen = 22;
        colScreen = 0;
        gotoxyP1_C();
        break;
      case '1':
        clearScreen_C();
        printBoardP1_C();
        row = 4;
        col = 3;
        indexMat = -1;
        //=======================================================
        calcIndexP1();
        ///calcIndexP1_C();
        //=======================================================
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("row(%i)*COLSMATRIX(7)+col(%i)=(%li)",row, col, indexMat);
        getchP1_C();
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("                           ");
        break;
      case '2':
        clearScreen_C();
        printBoardP1_C();
        //=======================================================
        updateBoardP1();
        ///updateBoardP1_C();	    
        //=======================================================
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("         Press any key ");
        getchP1_C();
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("                           ");
        break;
      case '3':
        clearScreen_C();
        printBoardP1_C();
        updateBoardP1_C();
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("   Move cursor left and right ");
        colCursor = 3;
        discSymbol = 'X';
        do {
          getchP1_C();
          c = charac;
          rowScreen = 5;
          colScreen = 4;
          gotoxyP1_C();
          printf("                               ");
          if (c == 'k' && colCursor > 0) colCursor--;
          if (c == 'l' && colCursor < COLSMATRIX-1) colCursor++;
          //=======================================================
          showDiscPosP1();
          ///showDiscPosP1_C();
          //=======================================================
        } while (c != 27);
        break;
      case '4': 	     
        clearScreen_C();
        printBoardP1_C();
        updateBoardP1_C();
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("   Move cursor left and right ");
        colCursor = 3;
        discSymbol = 'X';
        charac = ' ';
        do {
          getchP1_C();
          c = charac;
          rowScreen = 5;
          colScreen = 4;
          gotoxyP1_C();
          printf("                               ");	
          //=======================================================
          moveCursorP1(); 
          ///moveCursorP1_C(); 
          //=======================================================
          showDiscPosP1_C();
        } while (c != 27);
        break;  
      case '5': 	     
        clearScreen_C();
        printBoardP1_C();
        updateBoardP1_C();
        rowScreen = 25;
        colScreen = 4;
        gotoxyP1_C();
        printf("          Insert disc         ");
        colCursor = 3;
        state = 1;
        charac = ' ';
        do {
		  printBoardP1_C();
          updateBoardP1_C();
          rowScreen = 25;
          colScreen = 4;
          gotoxyP1_C();
          printf("          Insert disc         ");
          if (state == 1) discSymbol = DISCSYMBOLPLAYER1; else discSymbol = DISCSYMBOLPLAYER2;
          showDiscPosP1_C();
          getchP1_C();
          if (charac == 'k' || charac == 'l' ) moveCursorP1_C();
          if (charac == ' '){
            //=======================================================
            insertDiscP1();
            ///insertDiscP1_C();		
            //=======================================================
          }
        } while (charac != 27); 
        break;
      case '6': 	     
        clearScreen_C();
        printBoardP1_C();
        rowScreen = 25;
        colScreen = 2;
        gotoxyP1_C();
        printf(" This option will be developed in P2 ");
        getchP1_C();
        break;
      case '7':
        char mBoard2[ROWSMATRIX][COLSMATRIX] = 
                                  { /*0*/ {'O','O','X',' ','X','X','X'},
                                    /*1*/ {'O','X','O','X','O','O','O'},
                                    /*2*/ {'X','X','O','O','O','X','O'},
                                    /*3*/ {'X','O','O','X','X','X','O'},
                                    /*4*/ {'O','X','X','O','X','O','X'},
                                    /*5*/ {'O','X','O','X','X','X','O'} };
        int freeRowXcol2[COLSMATRIX]  =   {-1, -1, -1,  0, -1, -1, -1 };
        colCursor = 3;
        state = 2; //state = 1; //fourINaLINE 'X'
        if (state == 1) discSymbol = DISCSYMBOLPLAYER1; else discSymbol = DISCSYMBOLPLAYER2;
        clearScreen_C();
        printBoardP1_C();
        int i,j,k;
        for (i=0;i<ROWSMATRIX;i++) {
		  for (j=0;j<COLSMATRIX;j++) {
            charac = mBoard[i][j];
            mBoard[i][j]=mBoard2[i][j];
            mBoard2[i][j]=charac;
          }
        }
        for (j=0;j<COLSMATRIX;j++) {
          k=freeRowXcol[j];
          freeRowXcol[j]=freeRowXcol2[j];
          freeRowXcol2[j]=k;
        }
        insertDiscP1_C();
        //=======================================================
        checkEndP1();
        ///checkEndP1_C();
        //=======================================================
        updateBoardP1_C();
        for (i=0;i<ROWSMATRIX;i++) {
		  for (j=0;j<COLSMATRIX;j++) {
            charac = mBoard2[i][j];
            mBoard2[i][j]=mBoard[i][j];
            mBoard[i][j]=charac;
          }
        }
        for (j=0;j<COLSMATRIX;j++) {
          k=freeRowXcol2[j];
            freeRowXcol2[j]=freeRowXcol[j];
            freeRowXcol[j]=k;
        }
        printMessageP1_C();
        rowScreen = 27;
        colScreen = 14;
        gotoxyP1_C();
        printf("Press any key ");
        getchP1_C();
        break;
      case '8': 	//Juego completo en ensamblador.
        clearScreen_C();
        //=======================================================
        playP1();
        //=======================================================
        rowScreen = 27;
        colScreen = 14;
        gotoxyP1_C();
        printf("Press any key");
        getchP1_C();	  
        break;
      case '9': 	//Juego completo en C.
        clearScreen_C();
        //=======================================================
        playP1_C();
        //=======================================================
        rowScreen = 27;
        colScreen = 14;
        gotoxyP1_C();
        printf("Press any key");
        getchP1_C();	  
        break;
     }
  }
   
}