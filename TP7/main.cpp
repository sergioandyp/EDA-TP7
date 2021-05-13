#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"
#include "graphics.h"
#include <string>

#include "TwitterAPI.h"
#include "SergioLCD.h"
#include "lcd_juan.h"

#define MAX_VEL 10
#define INIT_VEL 5
using namespace std;

void printToLCDs(vector<basicLCD*> lcds, string text);

template<typename LCDT> void toggleLCD(basicLCD* lcd) {     // Para agregaar o borrar LCDS
        if (lcd == nullptr) {
                lcd = new LCDT;
        }
        else {
            delete lcd[0];
       }
}


int main(void) {


    //CONFIGURACIONES PARA EMPEZAR
    //char usuario[10] = "";
    //int cantidadTweets=0;

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    //int lcd = 0;
    
    Config conf;
    TwitterAPI api;
    vector<Tweet> tweets;
    Graphics gui;

    vector<basicLCD*> lcds(3, nullptr);     // Vector con los lcds

    //Inicializo la parte gráfica: display de allegro, imGui, cola de eventos de allegro... 
    if (!gui.initGraphics()) {
        cout << "Error al iniciar la interfaz gráfica" << endl;
        return 1;
    }


    while (!doExit){

        //Dibujo las configuraiones dentro del display
        gui.drawInit(conf, doExit);
        gui.drawConfig(conf, doExit);

        int i = gui.buttonLCD();
        switch (i) {
        case 1:
            toggleLCD<SergioLCD>(lcds[i - 1]);      // GuadaLCD
                break;
        case 2:
            toggleLCD<SergioLCD>(lcds[i - 1]);      // JuanLCD
                break;
        case 3:
            toggleLCD<SergioLCD>(lcds[i - 1]);
            break;
        default:
            break;
        }

        if (gui.buttonAceptar()) {
                if (!api.startTweetsDownload(conf.getUser(), conf.getTweetCount())) {

                    // Imprimir al display que hubo error
                    cout << "No se pudo obtener los tweets: " << endl;
                    cout << api.getError() << endl;
                    return 0;
                }

                while (api.runDownload()) {
                    cout << api.runDownload() << endl;
                    cout << "Descargando..." << endl;

                    // Aca pantalla de de que está descargando y poder detenerlo

                }

                if (!api.getTweets(tweets)) {

                    // Imprimir al display que hubo error

                    cout << "Error al obtener los tweets: " << endl << api.getError() << endl;
                    return 0;
                }
            
                
           }


               ////Imprime en consola...
               // for (auto& t : tweets)
               // {
               //     cout << t.getDate() << " - " << t.getUser() << " - " << t.getText() << endl;
               //     std::cout << "-----------------------------------------" << std::endl;
               // }



                    //GRÁFICOS

                    for (basicLCD* lcd : lcds) {
                        if (lcd != nullptr) {
                            // Aca imprimo a cada LCD        
                            // timerDisplay(, twit, titulo, vel);       //devuelve si se ompleto el twit
                        }
                    }


        }
    gui.destroyGraphics();

	return 0;
}


void printToLCDs(vector<basicLCD*> lcds, string text) {
    for (basicLCD* lcd : lcds) {
           if (lcd != nullptr) {
               *lcd << text.c_str();
           }
    }
}