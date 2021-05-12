#pragma once
#ifndef _INITALLEGRO_H_
#define _INITALLEGRO_H_




#include<iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// otros archivos puueden ver las variabes de allegro


extern ALLEGRO_EVENT_QUEUE* event_queue;






enum ERRORES		// Distintos tipos de errores
{
	NO_ERROR = -2,
	ERROR = -1
};

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
 //Funcion que le da la bienvenida al usuario por allegro
int init_allegro();
#endif


