section .note.GNU-stack noalloc noexec nowrite progbits
section .data               
;Cambiar Nombre y Apellido por vuestros datos.
developer db "KEVIN CABALLERO",0

;Constante que también está definida en C.
ROWSMATRIX equ 6
COLSMATRIX equ 7
DISCSYMBOLPLAYER1 equ 'X'
DISCSYMBOLPLAYER2 equ 'O'

section .text            
;Variables definidas en ensamblador.
global developer                        

;Subrutinas de ensamblador que se llaman desde C.
global calcIndexP1, updateBoardP1, showDiscPosP1, moveCursorP1, insertDiscP1
global checkEndP1, playP1

;Variables globales definidas en C.
extern charac, rowScreen, colScreen, row, col, indexMat, discSymbol
extern colCursor, state, newState, mBoard, freeRowXcol, direction

;Funciones de C que se llaman desde ensamblador.
extern gotoxyP1_C, printchP1_C, getchP1_C
extern printBoardP1_C, printMessageP1_C

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATENCIÓN: Recordad que en ensamblador las variables y los parámetros 
;;   de tipo 'char' deben asignarse a registros de tipo  
;;   BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;;   los de tipo 'short' deben asignarse a registros de tipo 
;;   WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;;   los de tipo 'int' deben asignarse a registros de tipo 
;;   DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;;   los de tipo 'long' deben asignarse a registros de tipo 
;;   QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Las subrutinas en ensamblador que debéis implementar son:
;;   calcIndexP1, updateBoardP1, showDiscPosP1
;;   moveCursorP1, insertDiscP1, checkEndP1
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Posicionar el cursor en la fila indicada por la variable (rowScreen) y 
; en la columna indicada por la variable (colScreen) de la pantalla,
; llamando a la función gotoxyP1_C.
; 
; Variables globales utilizadas:   
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call gotoxyP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Mostrar un carácter guardado en la variable (charac) en la pantalla, 
; en la posición donde está el cursor, llamando a la función printchP1_C.
; 
; Variables globales utilizadas:   
; (charac): Carácter que queremos mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printchP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Leer una tecla y guardar el carácter asociado en la variable (charac)
; sin mostrarlo en pantalla, llamando a la función getchP1_C. 
; 
; Variables globales utilizadas:   
; (charac): Carácter que leemos del teclado.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call getchP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; **** Esta función no es necesaria en C, solo en ensamblador.****
; Calcular el índice para acceder a la matriz (mBoard) en ensamblador.
; (mBoard[row][col]) en C, es ([mBoard+indexMat]) en ensamblador.
; donde indexMat = row*COLSMATRIX+col.
; La matriz (mBoard) es de tipo char(BYTE)1byte.
; 
; Variables globales utilizadas:	
; (row)     : Fila de la matriz mBoard.
; (col)     : Columna de la matriz mBoard.
; (indexMat): Índice para acceder a la matriz mBoard en ensamblador.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
calcIndexP1:
	push rbp
	mov rbp, rsp
	;guardar el estado de los registros que se modifican en esta 
	;subrutina y que no se utilizan para devolver valores.
	
	push rax
	
	; calculo del indice: se carga la fila, se multiplica por el numero 
	; de columnas y se suma la columna al resultado.
	mov al, [row]
	imul rax, COLSMATRIX
	add rax, [col]

	;almacenar el resultado en indexMat
	mov [indexMat], rax

	pop rax

	calcIndexP1_end:
	;restaurar el estado de los registros que se han guardado en la pila.  

	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar los valores de la matriz (mBoard)
; dentro del tablero, en las posiciones correspondientes. 
; Se debe recorrer toda la matriz (mBoard), de izquierda a derecha y 
; de arriba a abajo, desde la posición [0][0]=(0) hasta la posición [5][6]=(41),
; cada posición es de tipo char(BYTE)1byte, y para cada elemento hacer:
; Posicionar el cursor en el tablero en función de las variables 
; (rowScreen) fila y (colScreen) columna llamando a la subrutina gotoxyP1.
; Las variables (rowScreen) y (colScreen) se inicializarán en 7 y 8, 
; respectivamente, que es la posición en pantalla de la casilla [0][0].
; Las filas se incrementan de 2 en 2 y las columnas de 4 en 4.
; Mostrar los caracteres de cada posición de la matriz (mBoard) llamando
; a la subrutina printchP1.
;  
; Variables globales utilizadas:	
; (mBoard): Dirección de la matriz donde guardamos los discos introducidos.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
updateBoardP1:
	push rbp
	mov  rbp, rsp
	;guardar el estado de los registros que se modifican en esta 
	;subrutina y que no se utilizan para devolver valores.

	push rax
	push rbx
	push rcx
	push rdx

	; inicializar las posiciones en pantalla
	mov BYTE [rowScreen], 7
	mov BYTE [colScreen], 8

	mov rbx, mBoard
	mov rcx, 0

	; se recorren las filas comprobando si es el limite y cuando se han
	; recorrido todas las filas se sale
	fila_loop:
	cmp rcx, ROWSMATRIX
	jge updateBoardP1_end

	mov rdx, 0

	; se recorren las columnas comprobando si es el limite y cuando se han
	; recorrido todas las filas se sale
	columna_loop:
	cmp rdx, COLSMATRIX
	jge next_row

	mov rax, rcx
	imul rax, COLSMATRIX
	add rax, rdx

	; posicionar el cursor en la pantalla y mostrar el disco correspondiente
	mov rsi, [rowScreen]
	mov rdi, [colScreen]
	call gotoxyP1

	;cargar y mostrar la ficha
	mov al, [rbx + rax]
	mov [charac], al
	call printchP1

	add BYTE [colScreen], 4
	inc rdx
	jmp columna_loop

	next_row:
	mov BYTE [colScreen], 8
	add BYTE [rowScreen], 2
	inc rcx
	jmp fila_loop

	updateBoardP1_end:
	;restaurar el estado de los registros que se han guardado en la pila.
	pop rdx
	pop rcx
	pop rbx
	pop rax
	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Muestra en la parte superior del tablero el símbolo del jugador (discSymbol)
; que está jugando en la columna donde está el cursor (colCursor)
; y posiciona el cursor en la posición del tablero
; donde caerá el disco en esa columna.
; El símbolo del jugador se muestra en la fila (rowScreen=5) y la columna
; (colScreen=8+colCursor*4).
; Después posicionar el cursor en la misma columna y 
; en la fila donde caerá el disco si se presiona espacio.
; (rowScreen = 7+freeRowXcol[colCursor]*2)
; La matriz freeRowXcol es de tipo int(DWORD)4bytes.
; Posicionar el cursor en el tablero en función de las variables 
; (rowScreen) fila y (colScreen) columna llamando a la subrutina gotoxyP1.
; Mostrar el símbolo (discSymbol) llamando a la subrutina printchP1.
;  
; Variables globales utilizadas:	
; (discSymbol) : Símbolo del jugador que está jugando.
; (colCursor)  : Columna donde está el cursor.
; (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showDiscPosP1:
	push rbp
	mov rbp, rsp
	;guardar el estado de los registros que se modifican en esta 
	;subrutina y que no se utilizan para devolver valores.

	push rax
	push rbx
	push rcx
	push rdx

	; conseguir la columna actual del cursor
	mov rax, [colCursor]

	; calcular colScreen = 8 + colCursor * 4 (posicion columna en pantalla)
	mov rcx, 4
	imul rax, rcx
	add rax, 8
	mov BYTE [colScreen], al

	; posicionar siempre en la fila 5 para mostrar el simbolo del jugador
	mov BYTE [rowScreen], 5

	; mostrar el simbolo del jugador en la fila 5
	mov rsi, [rowScreen] 
	mov rdi, [colScreen]
	call gotoxyP1
	mov al, [discSymbol]
	mov [charac], al
	call printchP1

	; posicionar el cursor en el cuadrado donde caerá el disco
	movzx rbx, BYTE [colCursor] ;movzx al ser un valor pequeno y se extiende con 0
	mov edx, [freeRowXcol + rbx * 4]
	imul edx, 2
	add edx, 7
	mov BYTE [rowScreen], dl

	; posicionar cursor en el tablero
	mov rsi, [rowScreen]
	mov rdi, [colScreen]
	call gotoxyP1

	showDiscPosP1_end:
	;restaurar el estado de los registros que se han guardado en la pila.
	pop rdx
	pop rcx
	pop rbx
	pop rax
	mov rsp, rbp
	pop rbp
	ret
	
 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Actualizar la columna donde está el cursor (colCursor).
; Si se ha leído (charac=='k') izquierda o (charac=='l') derecha 
; actualizar la posición del cursor (colCursor +/- 1)
; controlando que no se salga del tablero [0..(COLSMATRIX-1)]. 
;  
; Variables globales utilizadas:	
; (charac)   : Carácter leído del teclado.
; (colCursor): Columna donde está el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
moveCursorP1:
	push rbp
	mov  rbp, rsp
	;guardar el estado de los registros que se modifican en esta 
	;subrutina y que no se utilizan para devolver valores.

	push rax

	mov al, [charac]
	
	; comprobar si se ha presionado la tecla 'k' para mover a la izquierda
	cmp al, 'k'
	jne check_right ; Si no es 'k', verificar si es 'l'

	; mover a la izquierda
	mov al, [colCursor]
	dec al
	cmp al, 0
	jl moveCursorP1_end
	mov [colCursor], al
	jmp moveCursorP1_end

	; comprobar si se ha presionado la tecla 'l' para mover a la derecha
	check_right:
	cmp al, 'l'
	jne moveCursorP1_end ; si no es 'l', salir

	; mover a la derecha
	mov al, [colCursor]
	inc al
	cmp al, COLSMATRIX
	jge moveCursorP1_end
	mov [colCursor], al

	moveCursorP1_end:
	;restaurar el estado de los registros que se han guardado en la pila.   
	pop rax

	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Inserta el disco (discSymbol) del jugador en la columna donde está 
; el cursor (colCursor) y en la primera fila libre de esa columna.
; La primera fila libre (row) de una columna dentro de la matriz (mBoard) 
; la tenemos guardada en la columna (colCursor) del vector (freeRowXcol)
; (freRowXcol[colCursor]).
; Si podemos introducir el disco (row>=0):
;   Si el estado del juego es (state==1) el (discSymbol = discSymbolPLAYER1),
;   si el estado del juego es (state==2) el (discSymbol = discSymbolPLAYER2).
;   Colocar el símbolo (discSymbol) en la matriz (mBoard) en la primera fila 
;   libre (row) y en la columna donde está el cursor (colCursor).
;   Decrementar la fila libre del vector (freeColXrow) de la columna donde
;   hemos insertado el disco (colCursor).
;   Cambiar de jugador, de jugador 1 a jugador 2 y de jugador 2 a jugador 1
;   (state = 3 - state).
; Si no queda espacio en esa columna (row=-1) no insertamos el disco.
; Devolver estado del juego.
; 
; Variables globales utilizadas:	
; (row)        : Fila de la matriz mBoard.
; (colCursor)  : Columna donde está el cursor.
; (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
; (mBoard)     : Dirección de la matriz donde guardamos los discos introducidos. 
; (discSymbol) : Símbolo del jugador que está jugando.
; (state)      : Estado del juego.
; (newState)   : Estado del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
insertDiscP1:
	push rbp
	mov  rbp, rsp
	;guardar el estado de los registros que se modifican en esta 
	;subrutina y que no se utilizan para devolver valores.

	push rax
	push rbx
	push rcx

	; calcularel indice de la fila libre
	mov eax, DWORD[colCursor]
	shl eax, 2
	mov ecx, DWORD[freeRowXcol+rax]
	mov DWORD[row], ecx

	; se comprueba si esta ocupada
	cmp ecx, 0
	jl insertDiscP1_end
	; determinar el simbolo que se va a insertar
	cmp DWORD[state], 1
	je p1Disc
	mov al, DISCSYMBOLPLAYER2
	jmp setDisc
		
	p1Disc:
	mov al, DISCSYMBOLPLAYER1
		
	; insertar el simbolo en la matriz del tablero
	setDisc:
	mov ebx, DWORD[colCursor]
	imul ecx, ecx, COLSMATRIX
	add ecx, ebx
	mov BYTE[mBoard+ecx], al
	; actualizar la fila libre de la columna
	mov eax, DWORD[colCursor]
	shl eax, 2
	dec DWORD[freeRowXcol+eax]
	; cambiar el estado del jugador
	mov eax, 3
	sub eax, DWORD[state]
	mov DWORD[newState], eax

	insertDiscP1_end:
	;restaurar el estado de los registros que se han guardado en la pila.
	pop rcx
	pop rbx
	pop rax
	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verificar si el tablero está lleno y no se puede continuar jugando.
; El tablero está lleno si la primera fila libre de todas las columnas
; indicada en el vector (freeRowXcol) son -1.
; Si se recorre todo el vector (freeRowXcol) y todas las posiciones
; valen -1, (c==COLSMdATRIX) pondremos (state=5) para indicar que
; el tablero está lleno y no se puede continuar jugando.
; 
; Variables globales utilizadas:	
; (freeRowXcol): Dirección de la matriz que indica la primera fila libre de cada columna del tablero.
; (state)      : Estado del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkEndP1:
	push rbp
	mov  rbp, rsp

	push rax
	push rbx
	push rcx

	mov rcx, 0 ;indice de columna
	mov rbx, 0 ;contador columnas llenas

	check_column:
	cmp rcx, COLSMATRIX
	jge check_full

	;ovbtener la primera fila libre de la columna y verificar si esta llena
	;si no esta llena sale, si esta llena incrementa el contador y repite
	mov eax, [freeRowXcol + rcx * 4]
	cmp eax, -1
	jne checkEndP1_end

	inc rbx
	inc rcx
	jmp check_column

	;comparar el contador con el total, si todas estan llenas marcar como lleno
	check_full:
	cmp rbx, COLSMATRIX
	jne checkEndP1_end

	mov DWORD [state], 5

	checkEndP1_end:
   ;restaurar el estado de los registros que se han guardado en la pila.
	pop rcx
	pop rbx
	pop rax
		
	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar el tablero de juego en la pantalla. Las líneas del tablero.
; 
; Variables globales utilizadas:	
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
; 
; Parámetros de entrada: 
; Ninguno.
; 
; Parámetros de salida: 
; Ninguno.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printBoardP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printBoardP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
 
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar un mensaje en la parte inferior del tablero según el 
; valor de la variable (state).
; (state) 0: Se ha presionado ESC para salir 
;         1: Juega el jugador 1.
;         2: Juega el jugador 2.
;         3: El jugador 1 ha hecho 4 en línea.
;         4: El jugador 2 ha hecho 4 en línea.
;         5: El tablero está lleno. Empate.
; 
; Variables globales utilizadas:	
; (state): Estado del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printMessageP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printMessageP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Función principal del juego
; Mostrar el tablero de juego y permitir hacer las jugadas de los 2 jugadores,
; alternativamente, hasta que uno de los dos jugadores ponga 4 discos en línea
; o el tablero quede lleno y nadie haya hecho ninguna línea de 4.
; 
; Pseudo-código:
; Mientras estén jugando el jugador 1 (state=1) o el jugador 2 (state = 2) hacer:
;   Mostrar las líneas del tablero llamando a la subrutina printBoardP2.
;   Actualizar el estado del tablero llamando a la subrutina updateBoardP2.
;   Mostrar un mensaje según el estado del juego (state) llamando 
;   a la subrutina printMessageP2.
;   Si (state==1) asignar a (discSymbol) el símbolo del jugador 1
;   (DISCSYMBOLPLAYER1='X'), si no, el símbolo del jugador 2
;   (DISCSYMBOLPLAYER2='O').
;   Leer una tecla llamando a la subrutina getchP2.
;   Si la tecla leída es 'k' o 'l' mover el cursor sin salir del
;   tablero llamando a la subrutina moveCursorP2.
;   Si la tecla leída es ' ' 
;     Insertar el disco en el tablero (mBoard) en la columna actual del 
;     cursor (colCursor) llamando a la subrutina insertDiscP2.
;     Si se ha introducido el disco, (state != newState)
;       Verificar si el tablero está lleno 
;       llamando a la subrutina checkEndP2.
;       Si no, si se ha hecho 4 en raya o el tablero está lleno (state <= 2)
;       actualizar el estado del juego (state = newState).
;   Si la tecla es ESC (ASCII 27) poner (state=0) para indicarlo.
; Actualizar el estado del tablero llamando a la subrutina updateBoardP2.
; Mostrar un mensaje según el estado del juego (state) llamando 
; a la subrutina printMessageP2.
; Termina el juego.
; 
; Variables globales utilizadas:
; (colCursor)  : Columna donde está el cursor.
; (state)      : Estado del juego.
; (newState)   : Estado del juego.
; (discSymbol) : Símbolo del jugador que está jugando.
; (charac)     : Carácter que leemos del teclado.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
playP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.
   push rbx
   push r12

   mov DWORD[colCursor], 3    ;colCursor = 3;
   mov DWORD[state], 1        ;state = 1;
   playP1_while:
   cmp DWORD[state], 1        ;while (state == 1 
   je playP1_loop
     cmp DWORD[state], 2      ;|| state ==2  ) {
     jne playP1_endwhile
     playP1_loop:
       call printBoardP1      ;printBoardP1_C();
       call updateBoardP1     ;updateBoardP1_C();
       call printMessageP1    ;printMessageP1_C();
       cmp DWORD[state], 1
       jne playP1_else1       ;if (state == 1) 
         mov BYTE[discSymbol], DISCSYMBOLPLAYER1;discSymbol = DISCSYMBOLPLAYER1; 
         jmp playP1_endif1
         playP1_else1:        ;else 
         mov BYTE[discSymbol], DISCSYMBOLPLAYER2;discSymbol = DISCSYMBOLPLAYER2;
         playP1_endif1:
         call showDiscPosP1   ;showDiscPosP1_C();
         call getchP1         ;getchP1_C();
         mov bl, BYTE[charac]
         cmp bl, 'k'          ;if (charac == 'k'  || charac == 'l') {
         je  playP1_move
           cmp bl, 'l'
           jne playP1_endmove
           playP1_move:
             call moveCursorP1;colCursor = moveCursorP1_C();
         playP1_endmove:      ;}
         cmp bl, ' '          ;if (charac == ' ' ) {
         jne playP1_endinsert
           call insertDiscP1       ;insertDiscP1_C();
           mov r12d, DWORD[newState]
           cmp DWORD[state], r12d         ;if(state != newState){ //new disc inserted
           je  playP1_notinserted
             call checkEndP1       ;checkEndP1_C();
           playP1_notinserted:     ;}
         cmp DWORD[state], 2       ;if (state <= 2) 
         jg  playP1_newstate
           mov DWORD[state], r12d  ;state = newState;
         playP1_newstate:
       playP1_endinsert:      ;}
       cmp bl, 27             ;if (charac == 27) {
       jne playP1_noesc
         mov DWORD[state], 0  ;state = 0;
       playP1_noesc:          ;}
     jmp playP1_while 
   playP1_endwhile:           ;}
   call updateBoardP1         ;updateBoardP1_C();
   call printMessageP1        ;printMessageP1_C();  

   playP1_end: 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r12
   pop rbx

   mov rsp, rbp
   pop rbp
   ret