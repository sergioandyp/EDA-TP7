#pragma once
#pragma warning(disable : 4996).

#include"basicLCD.h"
#include "graphics.h"
#include "string"			//pal tp nuevo

using namespace std;

class scrolling
{
public:
	void setcompleto(int completo);
	int getcompleto(void);
	float getVel(void);

	scrolling();
	~scrolling();
	bool scrollingOK(void);
	bool timerDisplay(basicLCD* lcd, string twit, float velocidad);

private:
	int contador;
	bool completo;
	float vel;
	int posicion;
	char* cstr;
	bool error;
	bool twitMostrado;
	ALLEGRO_TIMER* timer = NULL;
	cursorPosition cursor;
	void setVel(float vel);
	string  completaString(string twit);
	void ToDisplay(basicLCD* lcd, std::string twit, int letras);
	void printToDisplay(basicLCD* lcd, const unsigned char c);
	void movechar(basicLCD* lcd, int cursor, const unsigned char c);
	void movimiento(basicLCD* lcd, std::string twit, int letra);
	void movimFinal(basicLCD* lcd, std::string twit);
	bool initallegro(void);


};



