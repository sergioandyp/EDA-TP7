#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"

#include "graphics.h"

using namespace std;
ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* eventQueue;




int initGraphics(void) {

    if (!al_init()) {
        cout << "Error al inicializar allegro\n";
        return 0;
    }
    if (!al_init_image_addon()) {
        cout << "Error al inicializar image_addon\n";
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

void destroyGraphics(void) {

    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_display(display);
}

void drawInit(char* name, int& cantidadTweets, bool& doExit) {

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

   /*
    ImGui::InputText("utuario", name, 6, NULL);*/

    ImGui::Begin("Ingresar Datos", NULL, window_flags);
    ImGui::InputInt("Cantidad de tweets", &cantidadTweets, 1, 10);
    if (cantidadTweets < 1) {
        cantidadTweets = 1;
    }

    al_flip_display();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());


}

void drawConfig(float& vel, int& lcd, bool& doExit, bool& siguiente, bool& repetir, bool& anterior){

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
    
    //Para variar la velocidad (n�mero float que va desde 0 hasta MAX_VEL)
    ImGui::SliderFloat("Velocidad", &vel, 0, MAX_VEL, "%.5f");
    if (vel <= 0.0f) {
        vel = 0.0f;
    }

    ImGui::SetCursorPosX(DISPLAY_X*0.3);
    ImGui::Text("LCD");

    if (ImGui::Button("1", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd = 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("2", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd = 2;
    }
    ImGui::SameLine();
    if (ImGui::Button("3", ImVec2(ImGui::GetWindowSize().x*0.3, ImGui::GetWindowSize().y*0.15))){
        lcd = 3;
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
}