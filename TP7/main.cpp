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

#define LCD_COUNT   3

using namespace std;

enum DOWNLOAD_STATE { NOT_DOWNLOADING, DOWNLOADING, DOWNLOADED, DISPLAY_DOWNLOAD};

void printToLCDs(vector<basicLCD*>& lcds, string text);

template<typename LCDT> void toggleLCD(basicLCD*& lcd) {     // Para agregaar o borrar LCDS
        if (lcd == nullptr) {
                lcd = new LCDT;
        }
        else {
            delete lcd;
            lcd = nullptr;
       }
}

void clearLCDs(vector<basicLCD*>& lcds);

void deleteLCDs(vector<basicLCD*>& lcds);

void resetScroll(scrolling*& scroll, unsigned int size);


int main(void) {

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    DOWNLOAD_STATE downloadState = NOT_DOWNLOADING;
    
    Config conf;
    TwitterAPI api;
    vector<Tweet> tweets;
    Graphics gui;
    scrolling* scroll = new scrolling[LCD_COUNT];
    scrolling* loadScroll = new scrolling[LCD_COUNT];
    int tweetIndex = 0;
    bool nexttwit = false;

    vector<basicLCD*> lcds(LCD_COUNT, nullptr);     // Vector con los lcds

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

                printToLCDs(lcds, conf.getUser());


                if (!api.startTweetsDownload(conf.getUser(), conf.getTweetCount())) {

                    printToLCDs(lcds, "Error al descargar los tweets");
                }
                else {
                    downloadState = DOWNLOADING;
                    resetScroll(loadScroll, LCD_COUNT);
                }
                
           }

           
        if (downloadState == DOWNLOADING) {
            
            for (int i = 0; i < LCD_COUNT; i++) {       // Muestra de que esta cargando
                basicLCD* lcd = lcds[i];
                if (lcd != nullptr) {
                    if (loadScroll[i].timerDisplay(lcd, "-", "", 9)) {
                        loadScroll[i].settwitMostrado(false);
                    }
                }
            }

            if (api.runDownload()) {
                
                if(gui.buttonCancelar()) {
                    api.stopDownload();
                    downloadState = NOT_DOWNLOADING;
                    clearLCDs(lcds);
                }

            }
            else {
                downloadState = DOWNLOADED;
                tweets.clear();
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
                clearLCDs(lcds);
            }

        }

        if (downloadState == DISPLAY_DOWNLOAD) {

            if (gui.buttonAnterior()) {
                if (tweetIndex > 0) {
                    tweetIndex--;
                    resetScroll(scroll, LCD_COUNT);
                    clearLCDs(lcds);
                }
            }
            if (gui.buttonRepetir()) {
                resetScroll(scroll, LCD_COUNT);
                clearLCDs(lcds);
            }
            if (gui.buttonSiguiente()) {
                tweetIndex++;
                resetScroll(scroll, LCD_COUNT);
                clearLCDs(lcds);
            }

            if (tweetIndex < tweets.size()) {
                Tweet tweet = tweets[tweetIndex];
                nexttwit = false;
                for (int i = 0; i < lcds.size(); i++) {
                    basicLCD* lcd = lcds[i];
                    if (lcd != nullptr) {
                        nexttwit = scroll[i].timerDisplay(lcd, tweets[tweetIndex].getText(), tweets[tweetIndex].getDate(),(int)conf.getSpeed());
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

void deleteLCDs(vector<basicLCD*>& lcds) {
    for (basicLCD* lcd : lcds) {
           if (lcd != nullptr) {
               delete lcd;
           }
    }
}


void printToLCDs(vector<basicLCD*>& lcds, string text) {
    for (basicLCD* lcd : lcds) {
           if (lcd != nullptr) {
               lcd->lcdClear();
               *lcd << text.c_str();
           }
    }
}

void clearLCDs(vector<basicLCD*>& lcds) {
for (basicLCD* lcd : lcds) {
    if (lcd != nullptr) {
        lcd->lcdClear();
    }
}
}

void resetScroll(scrolling*& scrolls, unsigned int size) {
    delete[] scrolls;
    scrolls = new scrolling[size];  // Destruyo y los creo nuevamente
}