#pragma once

#include "basiclcd.h"
#include "graphics.h"
#include <string.h>

#define SPACEY 20
#define SPACEX 20
#define	SIZECARACT 60
#define FILAS 2
#define COLS 16
#define W_PIXELS	COLS*SIZECARACT			// tama�o del display
#define H_PIXELS	FILAS*SIZECARACT+SPACEY+20


extern ALLEGRO_EVENT_QUEUE* event_queue;

enum ERRORES		// Distintos tipos de errores
{
	NO_ERROR = -2,
	ERROR = -1
};

class lcd : public basicLCD
{
public:
	/*=====================================================
	* Name: basicLCD
	* Entra: -
	* Resulta: Constructor de la clase. Inicializa el LCD y deja
	* todo listo comenzar a utilizarlo.
	*
	* cadd =1 (cursor address) (ver NOTA 1)
	*=====================================================*/
	lcd();

	/*=====================================================
	* Name: ~basicLCD
	* Entra: -
	4/22
	* Resulta: Destructor de la clase. Libera cualquier recurso
	* que se hubiera tomado de forma de evitar
	* "resources leak".
	*=====================================================*/
	~lcd();
	/*=====================================================
	* Name: lcdInitOk
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre �true� si el display se inicializ�
	* correctamente (el constructor no tuvo errores) o �false
	* en caso contrario.
	*=====================================================*/
	bool lcdInitOk();

	/*=====================================================
	* Name: lcdGetError
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre un lcdError&
	*=====================================================*/
	// FT_STATUS lcdGetError() ;
	/*=====================================================
	* Name: lcdClear
	* Entra: -
	* Resulta: Borra el display y el cursor va a HOME
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdClear();
	/*=====================================================
	* Name: lcdClearToEOL
	* Entra: -
	* Resulta: Borra el display desde la posici�n actual
	* del cursor hasta el final de la l�nea.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdClearToEOL();
	/*=====================================================
	* Name: operator<<()
	* Entra: Un car�cter
	* Resulta: Pone el car�cter en la posici�n actual
	* del cursor del display y avanza el cursor a la pr�xima
	* posici�n respetando el gap (si el car�cter no es imprimible
	* lo ignora)
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �a� << �b� << �c�;
	*=====================================================*/
	basicLCD& operator<<(const  char c);

	/*=====================================================
	* Name: operator<<()
	* Entra: Una cadena de caracteres NULL terminated
	* Resulta: imprime la cadena de caracteres en la posici�n actual
	* del cursor y avanza el cursor al final de la cadena respetando
	* el gap (si alg�n car�cter no es imprimible lo ignora). Si recibe una
	* cadena de m�s de 32 caracteres, muestra los �ltimos 32 en el display.
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �Hola� << � � << �Mundo�;
	*=====================================================*/
	basicLCD& operator<<(const  char* c);
	/*=====================================================
	* Name: lcdMoveCursorUp
	*
	* Entra: -
	* Resulta: Pasa el cursor a la primera l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorUp();

	/*=====================================================
	* Name: lcdMoveCursorDown
	*
	* Entra: -
	* Resulta: Pasa el cursor a la segunda l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorDown();
	/*=====================================================
	* Name: lcdMoveCursorRight
	*
	* Entra: -
	* Resulta: Avanza el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorRight();
	/*=====================================================
	* Name: lcdMoveCursorLeft
	*
	* Entra: -
	* Resulta: Retrocede el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorLeft();

	/*=====================================================
	* Name: lcdSetCursorPosition
	* Entra: Recibe una estructura tipo cursorPosition
	* Resulta: Posiciona el cursor en la posici�n dada
	* por row y column. row[0-1] col[0-19]. Ante un valor inv�lido
	* de row y/o column ignora la instrucci�n (no hace nada).
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	bool lcdSetCursorPosition(const cursorPosition pos);
	/*=====================================================
	* Name: lcdGetCursorPosition
	* Entra: -
	* Resulta: Devuelve la posici�n actual del cursor.
	*
	*
	* Devuelve una estructura tipo cursorPosition
	*=====================================================*/
	cursorPosition lcdGetCursorPosition();
private:
	int crear_lcd(void);
	void escribir(void);
	bool errorinit;
	cursorPosition cursor;
	char matrizlcd[FILAS][COLS];
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* display;
};
