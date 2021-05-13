#pragma once
#pragma warning(disable : 4996).

#include"basicLCD.h"
#include "graphics.h"
#include "string"			//pal tp nuevo

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

class scrolling
{
public:
	void settwitMostrado(bool completo);
	bool gettwitMostrado(void);
	float getVel(void);

	scrolling();
	~scrolling();
	bool scrollingOK(void);
	bool timerDisplay(basicLCD* lcd, string twit, string  usuario, int velocidad);
	void titulo(basicLCD* lcd, string usuario);

private:
	int usuar;
	int contador;
	bool completo;
	int vel;
	int posicion;
	char* cstr;
	bool error;
	bool twitMostrado;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue;
	cursorPosition cursor;
	void setVel(int vel);
	float velelegida(int vel);
	string  completaString(string twit);
	void ToDisplay(basicLCD* lcd, std::string twit, int letras);
	void printToDisplay(basicLCD* lcd, const unsigned char c);
	void movechar(basicLCD* lcd, int cursor, const unsigned char c);
	void movimiento(basicLCD* lcd, std::string twit, int letra);
	void movimFinal(basicLCD* lcd, std::string twit);
	bool initallegro(void);


};




