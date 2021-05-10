#include "SergioLCD.h"

#include <iostream>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_primitives.h>

//#define DEBUG

#define BCKGD_IMAGE	"sergioLCD.jpg"
#define FONT_FILE	"sergioFont.ttf"	// Font monoespaciada
#define FONT_SIZE	74

#define START_X		116
#define START_Y		150
#define LINE_H		70
#define CHAR_W		42
#define CHAR_H		73
#define CHAR_GAP	4.3
#define C_START_Y	143

#define MAX_ROW	(LCD_ROW_COUNT-1)
#define MAX_COL	(LCD_COL_COUNT-1)

#define TOTAL_CHARS	(LCD_COL_COUNT*LCD_ROW_COUNT)

// Se considera caracter imprimible
#define IN_RANGE(c)		((c) >= 32 && (c) <= 126)

using namespace std;

SergioLCD::SergioLCD() : 
	cursor{ 0, 0 }, error("", "", E_OK),
	text(' ', TOTAL_CHARS),	// Construyo text lleno de espacios
	font(NULL), display(NULL), image(NULL) {

	if (al_is_image_addon_initialized() && al_is_font_addon_initialized()
		&& al_is_ttf_addon_initialized() && al_is_primitives_addon_initialized()) {

		image = al_load_bitmap(BCKGD_IMAGE);
		display = al_create_display(al_get_bitmap_width(image), al_get_bitmap_height(image));
		font = al_load_ttf_font(FONT_FILE, FONT_SIZE, 0);

	}

	if (font && image && display) {				// inicializacion OK
		print();
	}
	else {
		setError(E_INIT);
	}
}

SergioLCD::~SergioLCD() {
	al_destroy_font(font);
	al_destroy_bitmap(image);
	al_destroy_display(display);
}

bool SergioLCD::lcdInitOk() {
	return error.getErrorCode() != E_INIT;
}

lcdError& SergioLCD::lcdGetError() {
	return error;
}

bool SergioLCD::lcdClear() {
	
	text = string(' ', TOTAL_CHARS);

	cursor = { 0, 0 };
	print();
	return true;
}

bool SergioLCD::lcdClearToEOL() {

	size_t len = getLengthToEOL();

	text.replace(cursor.row*LCD_COL_COUNT + cursor.column, len, len, ' ');
	
	print();

	return true;
}

basicLCD& SergioLCD::operator<<(const char c) {
	if (IN_RANGE(c)) {
		text[cursor.row * LCD_COL_COUNT + cursor.column] = c;
		lcdMoveCursorRight();
		print();
	}
	return *this;
}

basicLCD& SergioLCD::operator<<(const char* c) {

	string s(c);

	for (int i = 0; i < s.size(); i++) {	// Borro caracteres invalidos
		if (!IN_RANGE(s[i])) {
			s.erase(i, 1);
			i--;
		}
	}

	int len = getLengthToEnd();
	if (s.size() > len) {
		s = s.substr(s.size() - len, len);
	}
	text.replace(cursor.row * LCD_COL_COUNT + cursor.column, s.size(), s);	// Reemplazo
	moveCursorOffset(s.size());
	print();
	return *this;
}

bool SergioLCD::lcdMoveCursorDown() {

	bool ret = moveCursorOffset(LCD_COL_COUNT);		// Le sumo al cursor la cantidad de columnas
	print();
	return ret;
}

bool SergioLCD::lcdMoveCursorUp() {

	bool ret = moveCursorOffset(-1*LCD_COL_COUNT);
	print();
	return ret;

}

bool SergioLCD::lcdMoveCursorRight() {
	
	bool ret = moveCursorOffset(+1);
	print();
	return ret;
}

bool SergioLCD::lcdMoveCursorLeft() {

	bool ret = moveCursorOffset(-1);
	print();
	return ret;

}

bool SergioLCD::lcdSetCursorPosition(const cursorPosition pos) {
	if (pos.column < 0 || pos.column > MAX_COL
		|| pos.row < 0 || pos.row > MAX_ROW) {		// error si pos fuera de rango
		setError(E_POS);
		return false;
	}
	cursor = pos;
	print();
	return true;
}

cursorPosition SergioLCD::lcdGetCursorPosition() {
	return cursor;
}


// Private Methods

void SergioLCD::print() {

	al_draw_bitmap(image, 0, 0, NULL);

	int x1 = START_X + cursor.column*(CHAR_W+CHAR_GAP) ;
	int y1 = C_START_Y + cursor.row * (LINE_H + CHAR_GAP);

	al_draw_filled_rectangle(x1, y1, x1+CHAR_W, y1+CHAR_H, al_map_rgba(47, 37, 255, 204));

	for (int i = 0; i < LCD_ROW_COUNT; i++) {
		al_draw_text(font, al_map_rgb(0, 0, 0), START_X, START_Y + i * LINE_H, 0,
			text.substr(i * LCD_COL_COUNT, LCD_COL_COUNT).c_str());	// linea a imprimir
	}

	al_flip_display();

	// DEBUG:
#ifdef DEBUG
	cout << "\"" << text << "\"" << endl;
	cout << "\"" << text.substr(0,16) << "\"" << endl;
	cout << "\"" << text.substr(16,16) << "\"" << endl << endl;
#endif

}

int SergioLCD::getLengthToEOL() {
	return LCD_COL_COUNT - cursor.column;
}

int SergioLCD::getLengthToEnd() {
	return getLengthToEOL() + LCD_COL_COUNT*(MAX_ROW - cursor.row);
}


bool SergioLCD::moveCursorOffset(int offset) {
	cursor.column += offset;

	if (cursor.column > MAX_COL) {								// Se pasa de los limites de columna
		cursor.row += (int)(cursor.column / LCD_COL_COUNT);		// Sumo diferencia de filas
		cursor.column = cursor.column % LCD_COL_COUNT;			// Sumo resto de columnas
	}
	else if (cursor.column < 0) {
		cursor.row += (int)((cursor.column+1) / LCD_COL_COUNT) - 1;		// Resto diferencia de filas
		cursor.column += LCD_COL_COUNT * (1 - (int)((cursor.column + 1) / LCD_COL_COUNT));
	}

	int newRow = min(cursor.row, MAX_ROW);
	newRow = max(0, newRow);

	if (cursor.row != newRow) {		// Se supero un limite maximo/minimo
		setError(E_POS);
		if (newRow == MAX_ROW) {
			cursor = { MAX_ROW, MAX_COL };
		}
		else {
			cursor = {0, 0};
		}
		return false;
	}

	return true;
}

void SergioLCD::setError(errorCode error) {
	switch (error)
	{
	case E_OK:
		this->error = lcdError("No error", "All is OK", E_OK);
		break;
	case E_INIT:
		this->error = lcdError("Initialization error", "Could not load resources", E_INIT);
		break;
	case E_POS:
		this->error = lcdError("Incorrect position",
			"Cursor reached a limit or position provided was incorrect",
			E_POS);;
		break;
	default:
		break;
	}
}