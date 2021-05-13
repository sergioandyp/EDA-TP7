#include "claselcd2.h"

claselcd2::claselcd2() {

	//Inicializo.
	al_init_font_addon();
	al_init_ttf_addon();
	cursor = { 0,0 };

	//Error
	ptrError = new lcdError("No Error", "No Error", ERROR_NONE);

	//Inicializo el texto del display
	int i = 0;
	while (i < MAX_ROW * MAX_COLUMN) {

		++i;
		ptrText[i] = ' ';
	}

	this->display = al_create_display(DISPLAY_X, DISPLAY_Y);
	if (!this->display) {
		cout << "Error al crear el display" << endl;
		this->error = true;
	}
	this->font = al_load_ttf_font("claselcd2Font.ttf", FUENTE_SIZE, NULL);
	if (font == NULL) {
		cout << "Error al cargar la fuente" << endl;
		this->error = true;
	}
	timer = al_create_timer(1.0 / 60);
	if (timer == NULL) {
		cout << "Error al crear el timer" << endl;
		this->error = true;
	}

	//Registro el display en los eventos

	al_set_window_title(display, "Display 2");
	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	this->error = false;

}

claselcd2::~claselcd2() {

	al_destroy_display(this->display);
	al_destroy_font(this->font);
	al_destroy_timer(this->timer);
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	delete ptrError;


}
bool claselcd2::lcdInitOk() {
	if (this->error == true) {
		return false;
	}
	else {
		return true;
	}
}
bool claselcd2::lcdClear() {

	//Limpio el texto
	int i = 0;
	while (i < MAX_ROW * MAX_COLUMN) {

		ptrText[i] = ' ';
		++i;
	}
	//Cursor a (0,0)
	this->cursor.column = 0;
	this->cursor.row = 0;

	//"limpia" el display 
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	return true;
}
bool claselcd2::lcdClearToEOL() {

	//Arreglo para imprimir la línea
	char strLine[MAX_COLUMN];
	int curR = (this->cursor.row);
	int curC = (this->cursor.column);

	for (int c = ((MAX_COLUMN)*curR); curC < MAX_COLUMN; ++curC) {

		ptrText[c+curC] = ' ';
		this->lcdMoveCursorRight();
	};

	//"limpia" el display 
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	//Imprime todo el texto
	for (int i = 0; i < MAX_ROW; ++i) {
		for (int c = 0; c < MAX_COLUMN; ++c) {
			strLine[c] = ptrText[(i * MAX_COLUMN) + c];
		};
		al_draw_text(this->font, al_map_rgb(255, 255, 255), DISPLAY_X / 2, RANGO_BORDES + (FUENTE_SIZE * i), ALLEGRO_ALIGN_CENTRE, strLine);
	};
	al_flip_display();


	return true;
}

lcdError& claselcd2::lcdGetError()
{
	return *ptrError;	//Devuelve el objeto error
}


basicLCD& claselcd2:: operator<<(const char c) {

	//Arreglo para imprimir la línea
	char strLine[MAX_COLUMN];
	int curR=cursor.row;
	int curC=cursor.column;

	//La posición a la que apunta el cursor es = c
	this->ptrText[(curR * MAX_COLUMN) + (curC)] = c;
	this->lcdMoveCursorRight();

	//"limpia" el display 
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	//Imprime todo el texto
	for (int i = 0; i < MAX_ROW; ++i) {
		for (int c = 0; c < MAX_COLUMN; ++c) {
			strLine[c] = ptrText[(i * MAX_COLUMN) + c];
		};
		al_draw_text(this->font, al_map_rgb(255, 255, 255), DISPLAY_X / 2, RANGO_BORDES + (FUENTE_SIZE * i), ALLEGRO_ALIGN_CENTRE, strLine);
	};
	al_flip_display();
	return (*this);
}


basicLCD& claselcd2:: operator<<(const char* c) {

	//Arreglo para imprimir la línea
	char strLine[MAX_COLUMN];
	int curR=cursor.row;
	int curC=cursor.column;

	//Copio c en el texto
	for (int contChar = 0; (c[contChar] != '\0'); ++contChar) {

		if (((curR * MAX_COLUMN) + (curC) + contChar) < (MAX_ROW * MAX_COLUMN)) {
			this->ptrText[(curR * MAX_COLUMN) + (curC) + contChar] = c[contChar];
			this->lcdMoveCursorRight();
		}
	};

	cout << string(ptrText) << endl;
	//"limpia" el display 
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	//Imprime todo el texto
	for (int i = 0; i < MAX_ROW; ++i) {
		for (int c = 0; c < MAX_COLUMN; ++c) {
			strLine[c] = ptrText[(i * MAX_COLUMN) + c];
		};
		al_draw_text(this->font, al_map_rgb(255, 255, 255), DISPLAY_X / 2, RANGO_BORDES + (FUENTE_SIZE * i), ALLEGRO_ALIGN_CENTRE, strLine);
	};

	al_flip_display();
	return (*this);
}
bool claselcd2::lcdMoveCursorUp() {

	if (this->cursor.row == 0) {
		ptrError->ErrorName = "Cursor Limit";
		ptrError->ErrorDescription = "Can't Move Cursor Up.";
		ptrError->ErrorCode = ERROR_CURSOR;

		return false;
	}
	else {
		--(cursor.row);
	}
	return true;
}
bool claselcd2::lcdMoveCursorDown() {

	if (this->cursor.row == (MAX_ROW - 1)) {

		ptrError->ErrorName = "Cursor Limit";
		ptrError->ErrorDescription = "Can't Move Cursor Down.";
		ptrError->ErrorCode = ERROR_CURSOR;

		return false;
	}
	else {
		++(cursor.row);
	}
	return true;
}
bool claselcd2::lcdMoveCursorRight() {

	if ((this->cursor.column == (MAX_COLUMN - 1)) && (this->cursor.row == (MAX_ROW - 1))) {

		ptrError->ErrorName = "Cursor Limit";
		ptrError->ErrorDescription = "Can't Move Cursor Right.";
		ptrError->ErrorCode = ERROR_CURSOR;
		return  false;
	}
	else if ((this->cursor.column == (MAX_COLUMN - 1)) && (this->cursor.row != (MAX_ROW - 1))) {
		this->cursor.column = 0;
		++(this->cursor.row);
	}
	else {
		++(cursor.column);
	}
	return true;
}
bool claselcd2::lcdMoveCursorLeft() {

	if ((this->cursor.column == 0) && (this->cursor.row == 0)) {
		ptrError->ErrorName = "Cursor Limit";
		ptrError->ErrorDescription = "Can't Move Cursor Left.";
		ptrError->ErrorCode = ERROR_CURSOR;
		return  false;
	}
	else if ((this->cursor.column == 0) && (this->cursor.row != 0)) {
		this->cursor.column = (MAX_COLUMN - 1);
		--(this->cursor.row);
	}
	else {
		--(cursor.column);
	}
	return true;
}
bool claselcd2::lcdSetCursorPosition(const cursorPosition pos) {

	bool result = true;

	if ((0 <= pos.row) && (pos.row < MAX_ROW) && (0 <= pos.column) && (pos.column < MAX_COLUMN)) {
		this->cursor = pos;
		result = true;
	}
	else {
		ptrError->ErrorName = "Cursor Limit";
		ptrError->ErrorDescription = "Can't Set Cursor Position.";
		ptrError->ErrorCode = ERROR_CURSOR;
		result = false;
	}
	return result;
}
cursorPosition claselcd2::lcdGetCursorPosition() {

	return this->cursor;
}