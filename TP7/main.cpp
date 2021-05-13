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
#include "claselcd2.h"
//#include "lcd_juan.h"
#include "scrolling.h"

#define MAX_VEL 10
#define INIT_VEL 5
using namespace std;

enum DOWNLOAD_STATE { NOT_DOWNLOADING, DOWNLOADING, DOWNLOADED, DISPLAY_DOWNLOAD};

void printToLCDs(vector<basicLCD*> lcds, string text);

template<typename LCDT> void toggleLCD(basicLCD*& lcd) {     // Para agregaar o borrar LCDS
        if (lcd == nullptr) {
                lcd = new LCDT;
        }
        else {
            delete lcd;
            lcd = nullptr;
       }
}

void deleteLCDs(vector<basicLCD*> lcds);


int main(void) {

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    DOWNLOAD_STATE downloadState = NOT_DOWNLOADING;
    
    Config conf;
    TwitterAPI api;
    vector<Tweet> tweets;
    Graphics gui;
    scrolling scroll[3];
    int tweetIndex = 0;
    bool nexttwit = false;

    vector<basicLCD*> lcds(3, nullptr);     // Vector con los lcds

    //Inicializo la parte gráfica: display de allegro, imGui, cola de eventos de allegro... 
    if (!gui.initGraphics()) {
        cout << "Error al iniciar la interfaz gráfica" << endl;
        return 1;
    }


    while (!doExit) {

        //Dibujo las configuraiones dentro del display

        gui.drawConfig(conf, doExit);

        int lcdBtn = gui.buttonLCD();
        switch (lcdBtn) {
        case 1:
            toggleLCD<claselcd2>(lcds[lcdBtn - 1]);
                break;
        case 2:
            toggleLCD<SergioLCD>(lcds[lcdBtn - 1]);      // JuanLCD
                break;
        case 3:
            toggleLCD<SergioLCD>(lcds[lcdBtn - 1]);
            break;
        default:
            break;
        }


        if (gui.buttonAceptar()) {
                if (!api.startTweetsDownload(conf.getUser(), conf.getTweetCount())) {

                    printToLCDs(lcds, "Error al descargar los tweets");


                    // Imprimir al display que hubo error
                    cout << "No se pudo obtener los tweets: " << endl;
                    cout << api.getError() << endl;
                }
                else {
                    downloadState = DOWNLOADING;
                }
                
           }

           
        if (downloadState == DOWNLOADING) {
            if (api.runDownload()) {
                cout << "Descargando..." << endl;    
                
                if(gui.buttonCancelar()) {
                    api.stopDownload();
                    downloadState = NOT_DOWNLOADING;
                }

            }
            else {
                downloadState = DOWNLOADED;
            }
        }

        if (downloadState == DOWNLOADED) {

            if (!api.getTweets(tweets)) {

                // Imprimir al display que hubo error
                printToLCDs(lcds, "Error al obtener los tweets");

                downloadState = NOT_DOWNLOADING;
            }
            else {
                downloadState = DISPLAY_DOWNLOAD;
                tweetIndex = 0;
            }

        }

        if (downloadState == DISPLAY_DOWNLOAD) {

            if (gui.buttonAnterior()) {
                if (tweetIndex > 0) {
                    tweetIndex--;
                    // AVISAR QUE SE CAMBIO DE TWEET
                }
            }
            if (gui.buttonRepetir()) {
                // HACER ACCION DE REPETIR
            }
            if (gui.buttonSiguiente()) {
                tweetIndex++;
                // AVISAR QUE SE CAMBIO DE TWEET
            }

            if (tweetIndex < tweets.size()) {
                Tweet tweet = tweets[tweetIndex];
                nexttwit = false;
                for (int i = 0; i < lcds.size(); i++) {
                    basicLCD* lcd = lcds[i];
                    if (lcd != nullptr) {


                        nexttwit = scroll[i].timerDisplay(lcd, tweets[tweetIndex].getText(), tweets[tweetIndex].getUser(),(int)conf.getSpeed());
                    }
                }

                if (nexttwit == true) {
                    tweetIndex++;
                    nexttwit = false;
                    for (int i = 0; i < 3; i++) {
                        scroll[i].settwitMostrado(false);
                    }
                }
            }
            else {
                printToLCDs(lcds, "ULTIMO TUIT");
            }

        }
        else {
            gui.buttonAnterior();       // Leemos los botones pero los ignoramos
            gui.buttonRepetir();        // Esto es para limpiarlos y no tener valores
            gui.buttonSiguiente();      // antiguos.
        }

        }

    deleteLCDs(lcds);

    gui.destroyGraphics();

	return 0;
}

void deleteLCDs(vector<basicLCD*> lcds) {
    for (basicLCD* lcd : lcds) {
           if (lcd != nullptr) {
               delete lcd;
           }
    }
}


void printToLCDs(vector<basicLCD*> lcds, string text) {
    for (basicLCD* lcd : lcds) {
           if (lcd != nullptr) {
               lcd->lcdClear();
               *lcd << text.c_str();
           }
    }
}