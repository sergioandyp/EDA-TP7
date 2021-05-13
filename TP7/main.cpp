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
    scrolling scroll;
    int tweetIndex = 0;
    bool nexttwit;

    vector<basicLCD*> lcds(3, nullptr);     // Vector con los lcds

    //Inicializo la parte gr�fica: display de allegro, imGui, cola de eventos de allegro... 
    if (!gui.initGraphics()) {
        cout << "Error al iniciar la interfaz gr�fica" << endl;
        return 1;
    }


    while (!doExit){

        //Dibujo las configuraiones dentro del display

        gui.drawConfig(conf, doExit);

        int i = gui.buttonLCD();
        switch (i) {
        case 1:
            toggleLCD<claselcd2>(lcds[i - 1]);      // GuadaLCD
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


                cout << "Error al obtener los tweets: " << endl << api.getError() << endl;

                downloadState = NOT_DOWNLOADING;
            }
            else {
                downloadState = DISPLAY_DOWNLOAD;
                tweetIndex = 0;
            }

        }

        if (downloadState == DISPLAY_DOWNLOAD) {
            for (int i = 0; i < lcds.size(); i++) {
                basicLCD* lcd = lcds[i];
                if (lcd != nullptr) {
                                     
                    // EJEMPLO PARA VER SI IMPRIME ALGO, CAMBIAR
    
                    nexttwit=scroll.timerDisplay(lcd,tweets[tweetIndex].getText(),(tweets[tweetIndex].getUser()),1-(conf.getSpeed()/10.0) );
                    if(nexttwit==true){
                        tweetIndex++;
                     nexttwiter=0;
                    }

                    if (tweetIndex >= tweets.size()) {
                        tweetIndex = tweets.size() -  1;
                        downloadState = END_DISPLAY;
                    }

                }
            }
        }

        if (downloadState == END_DISPLAY) {
            printToLCDs(lcds, "ULTIMO TWEET");
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
               *lcd << text.c_str();
           }
    }
}