#ifndef CLASELCD2_H
#define CLASELCD2_H

#include "basicLCD.h"

enum {ERROR_NONE = 0, ERROR_CURSOR};

#define MAX_ROW			2
#define MAX_COLUMN		16
#define DISPLAY_X		1000
#define RANGO_BORDES	(DISPLAY_X/10)
#define FUENTE_SIZE		((DISPLAY_X-(2*RANGO_BORDES))/16)
#define	DISPLAY_Y		((FUENTE_SIZE*2)+(2*RANGO_BORDES))

class claselcd2 : public basicLCD {

public:
	claselcd2();
	~claselcd2();
	bool lcdInitOk();
	lcdError& lcdGetError();
	bool lcdClear();
	bool lcdClearToEOL();
	basicLCD& operator<<(const char c);
	basicLCD& operator<<(const char* c);
	bool lcdMoveCursorUp();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorLeft();
	bool lcdSetCursorPosition(const cursorPosition pos);
	cursorPosition lcdGetCursorPosition();

private:
	cursorPosition			cursor;
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_TIMER* timer = NULL;
	bool error;
	char ptrText[MAX_ROW*MAX_COLUMN];
	lcdError* ptrError;
};


#endif // !CLASELCD2_H