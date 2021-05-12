#include "initallegro.h"
ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_FONT* font;;
using namespace std;


int init_allegro() {            // incio allegro
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_image_addon()) {
        cout << "failed to init image addon!" << endl;
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << "failed to load event queue!" << endl;
        al_destroy_display(display);

        return -1;
    }
    else if (!al_install_keyboard()) {
        cout << "failed to install keyboard!" << endl;


        al_destroy_event_queue(event_queue);
        return -1;
    }
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_init_primitives_addon();
    al_init_image_addon();  // initialize the font addon
    al_init_font_addon();
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon





    return NO_ERROR;
}
