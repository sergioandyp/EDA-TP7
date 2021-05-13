#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H


#include<iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "Config.h"

#define MAX_VEL 10
#define INIT_VEL 5
#define DISPLAY_X	500
#define DISPLAY_Y	500

class Graphics {
public:

	//Constructor.
	Graphics();

	//Devuelve un 1 si todo salió bien. Ante algun problema devuelve un 0.
	int initGraphics(void);

	//Destruye display, cola de eventos...
	void destroyGraphics(void);

	//Permite ver y cambiar las configuraciones: LCD, velocidad, anterior, siguiente...
	void drawConfig(Config& config, bool& doExit);

	//Permite ingresar el nombre de usiario de Twitter y la cantidad de Twets
	//void drawInit(Config& config, bool& doExit);

	//Devuelve true cuando se tocó el botón de "anterior"
	bool buttonAnterior();
	
	//Devuelve true cuando se tocó el botón de "siguiente"
	bool buttonSiguiente();

	//Devuelve true cuando se tocó el botón de "repetir"
	bool buttonRepetir();

	//Si no se tocó ningún botón devuelve 0. Si se toca uno de los botones (1,2,3) devuelve 1,2,3.
	int buttonLCD();

	//Si se apretó el botón aceptar devuelve un true
	bool buttonAceptar();

	//Si se apretó el botón cancelar (descarga) devuelve un true
	bool buttonCancelar();

private:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* eventQueue;

	bool lcd1;
	bool lcd2;
	bool lcd3;

	bool siguiente;
	bool repetir;
	bool anterior;
	bool aceptar;
	bool cancelarDescarga;
};

/*
extern ALLEGRO_EVENT_QUEUE* event_queue;

enum ERRORES		// Distintos tipos de errores
{
	NO_ERROR = -2,
	ERROR = -1
};

 //Funcion que le da la bienvenida al usuario por allegro
int init_allegro();
*/

#endif //GRAPHICS_H