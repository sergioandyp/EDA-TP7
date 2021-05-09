#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#define MAX_VEL 10
#define INIT_VEL 5
#define DISPLAY_X	500
#define DISPLAY_Y	500


//Devuelve un 1 si todo salió bien. Ante algun problema devuelve un 0.
int initGraphics(void);

//Destruye display, cola de eventos...
void destroyGraphics(void);

//Permite ver y cambiar las configuraciones: LCD, velocidad, anterior, siguiente...
void drawConfig(float& vel, int& lcd, bool& doExit, bool& siguiente, bool& repetir, bool& anterior);

//Permite ingresar el nombre de usiario de Twitter y la cantidad de Twets
void drawInit(char* name, int& cantidadTweets, bool& doExit);

#endif //TP3EDA_GRAPHICS_H