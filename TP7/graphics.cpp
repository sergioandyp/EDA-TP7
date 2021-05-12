#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"
#include "Config.h"

#include "graphics.h"

//Para poder usar la función strcpy.
#pragma warning(disable : 4996)

using namespace std;


int Graphics::initGraphics(void) {

    if (!al_init()) {
        cout << "Error al inicializar allegro\n";
        return 0;
    }

    if (!al_init_image_addon()) {
        cout << "Error al inicializar image_addon\n";
        return 0;
    }

    if (!al_init_primitives_addon()) {
        cout << "Error al inicializar primitives_addon\n";
        al_shutdown_image_addon();
        return 0;
    }

    if (!al_init_font_addon()) {
        cout << "Error al inicializar font_addon\n";
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        return 0;
    }

    if (!al_init_ttf_addon()) {
        cout << "Error al inicializar ttf_addon\n";
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        al_shutdown_font_addon();
        return 0;
    }


    al_install_keyboard();
    al_install_mouse();
    if (!al_init_primitives_addon()) { return 0; }
    display = al_create_display(DISPLAY_X, DISPLAY_Y);
    al_set_window_title(display, "Display Twitter");

    eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();   //Tema oscuro
    ImGui_ImplAllegro5_Init(display);

    return 1;
}

void Graphics::destroyGraphics(void) {

    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);

    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
}

void Graphics::drawInit(Config& config, bool& doExit) {

    int tempTweetCount = config.getTweetCount();
    char tempUser[100] = {};
    strcpy(tempUser, config.getUser().c_str());
    //string tempStringUser=config.getUser();
    //char* ptrTempUser[100] = tempStringUser.c_str();


    

    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = 0; 
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(DISPLAY_X, DISPLAY_X / 4));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ALLEGRO_EVENT ev;

    while (al_get_next_event(eventQueue, &ev))
    {
        ImGui_ImplAllegro5_ProcessEvent(&ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            doExit = true;
    }


    ImGui::Begin("Ingresar Datos", NULL, window_flags);

    ImGui::InputText("Nombre de Usuario", tempUser, 100);
    config.setUser(tempUser);
 
    ImGui::InputInt("Cantidad de tweets",&tempTweetCount, 1, 10);
    if (tempTweetCount < 1) {
        tempTweetCount = 1;
    }
    config.setTweetCount(tempTweetCount);

    al_flip_display();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

}

void Graphics::drawConfig(Config& config, bool& doExit){

    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(DISPLAY_X,DISPLAY_X/4));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ALLEGRO_EVENT ev;

    while (al_get_next_event(eventQueue, &ev))
    {
        ImGui_ImplAllegro5_ProcessEvent(&ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            doExit = true;
    }

    //Ventana de configuraciones
    ImGui::Begin("Configuraciones",NULL, window_flags);
    
    //Para variar la velocidad (número float que va desde 0 hasta MAX_VEL)
    float speed = config.getSpeed();
    ImGui::SliderFloat("Velocidad", &speed, 0, MAX_VEL, "%.5f");
    config.setSpeed(speed);

    ImGui::SetCursorPosX(DISPLAY_X*0.3);
    ImGui::Text("LCD");

    if (ImGui::Button("1", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd1 = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("2", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd2 = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("3", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd3 = true;
    }

 

    if (ImGui::Button("< Anterior", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y * 0.2))) {
        anterior= true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Repetir Tweet", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y * 0.2))) {
        repetir=true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Siguiente >", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y * 0.2))) {
        siguiente= true;
    }


    al_flip_display();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

    ////Experimento
    //ImGui_ImplAllegro5_NewFrame();
    //ImGui::NewFrame();
}


//ALLEGRO_DISPLAY* display;
//ALLEGRO_EVENT_QUEUE* event_queue;

/*
ALLEGRO_FONT* font;;

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
*/
