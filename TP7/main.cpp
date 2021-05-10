#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"
#include "graphics.h"
#include <string>

#define MAX_VEL 10
#define INIT_VEL 5
using namespace std;

int main(void) {


    //CONFIGURACIONES PARA EMPEZAR
    char usuario[10];
    int cantidadTweets=0;

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    int lcd = 0;
    float velocidad = INIT_VEL;
    bool siguiente=false;
    bool repetir = false;
    bool anterior=false;

    //Inicializo la parte gráfica: display de allegro, imGui, cola de eventos de allegro... 
    if (!initGraphics()) {
        return 1;
    }


    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        drawInit(usuario, cantidadTweets, doExit);

    }
    doExit = false;
    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        drawConfig(velocidad, lcd,doExit, siguiente, repetir, anterior);

    }

    destroyGraphics();

	return 0;
}