#include <iostream>

#include "SergioLCD.h"

using namespace std;

bool initAllegro(void);
void destroyAllegro(void);

//
//int main() {
//
//    initAllegro();
//
//	basicLCD* lcd[2];
//
//	string s;
//
//	cin >> s;
//
//	lcd[0] = new SergioLCD;
//
//	cin >> s;
//
//	*lcd[0] << "Hola, texto para lcd 0";
//
//	cin >> s;
//
//	lcd[1] = new SergioLCD;
//
//	cin >> s;
//
//	*lcd[1] << "Un texto para lcd 1";
//
//	cin >> s;
//
//    *lcd[0] << "Vuelvo al lcd 0";
//	
//    cin >> s;
//
//    *lcd[1] << "Ahora al lcd 1";
//
//    cin >> s;
//
//    delete lcd[0];
//
//    cin >> s;
//
//    delete lcd[1];
//
//    cin >> s;
//
//    destroyAllegro();
//
//	return 0;
//}
//

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>



bool initAllegro(void) {                       //Funcion que inicializa todo allegro. No recibe nada. Devuelve un true si esta todo ok, false si hay error.

    if (!al_init()) {
        cout << "failed to initialize allegro!" << endl;
        return false;
    }

    if (!al_init_image_addon()) {       // ADDON necesario para manejo de imagenes 
        cout << "Failed to initialize image addon!" << endl;
        return false;
    }

    if (!al_init_font_addon()) {        // ADDON necesario para manejo de fuentes 
        cout << "Failed to initialize font addon!" << endl;
        al_shutdown_image_addon();
        return false;
    }

    if (!al_init_primitives_addon()) {
        cout << "Failed to initialize primitives addon!" << endl;
        al_shutdown_image_addon();
        al_shutdown_font_addon();
        return false;
    }

    if (!al_init_ttf_addon()) {
        cout << "Failed to initialize ttf addon!" << endl;
        al_shutdown_image_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return false;
    }

    return true;
}

void destroyAllegro(void) {
    al_shutdown_image_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
}