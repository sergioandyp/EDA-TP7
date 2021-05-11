#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"
#include "graphics.h"
#include <string>

#include "TwitterAPI.h"

#define MAX_VEL 10
#define INIT_VEL 5
using namespace std;

int main(void) {


    //CONFIGURACIONES PARA EMPEZAR
    char usuario[10] = "";
    int cantidadTweets=0;

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    int lcd = 0;
    
    Config conf(INIT_VEL, usuario, cantidadTweets);

    Graphics gui;

    //Inicializo la parte gráfica: display de allegro, imGui, cola de eventos de allegro... 
    if (!gui.initGraphics()) {
        return 1;
    }


    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        gui.drawInit(usuario, cantidadTweets, doExit);

    }

    TwitterAPI api;

    if (!api.startTweetsDownload(usuario, cantidadTweets)) {

        // Imprimir al display que hubo error

    	cout << "No se pudo obtener los tweets: " << endl;
    	cout << api.getError() << endl;
    	return 0;
    }
    
    while (api.runDownload()) {
    	cout << "Descargando..." << endl;
        
        // Aca pantalla de de que está descargando y poder detenerlo

    }
    
    vector<Tweet> tweets;
    
    if (!api.getTweets(tweets)) {

        // Imprimir al display que hubo error

    	cout << "Error al obtener los tweets: " << endl << api.getError() << endl;
    	return 0;
    }

    doExit = false;
    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        gui.drawConfig(conf, doExit);

    }

    gui.destroyGraphics();

	return 0;
}