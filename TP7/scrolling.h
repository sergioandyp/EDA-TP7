#pragma once
#pragma warning(disable : 4996).

#include"basicLCD.h"

#include "string"			//pal tp nuevo

class scrolling
{
public:
	void setcompleto(int completo);
	int getcompleto(void);
	void ToDisplay(basicLCD* lcd, std::string twit, int letras);
	float getVel(void);
	void setVel(float vel);
	//void strings(basicLCD* lcd, std::string twit, int letras);
	scrolling();
	//void movimiento(basicLCD* lcd, const unsigned char* c, int letra,int len);

private:
	int cola;
	bool completo;
	float vel;
	int posicion;
	char* cstr;
	cursorPosition cursor;
	void printToDisplay(basicLCD* lcd, const unsigned char c);
	void movechar(basicLCD* lcd, int cursor, const unsigned char c);
	void movimiento(basicLCD* lcd, std::string twit, int letra);
	void movimFinal(basicLCD* lcd, std::string twit);
	std::string divisor(std::string twit);
	int modulo(int len);

};



