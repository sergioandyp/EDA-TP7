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

//Para poder usar la funci�n strcpy.
#pragma warning(disable : 4996)

using namespace std;


Graphics::Graphics() {

    this->siguiente=false;
    this->repetir = false;
    this->anterior = false;
    this->aceptar = false;
    this->cancelarDescarga = false;
   

}

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


void Graphics::drawConfig(Config& config, bool& doExit){
    
    
    int tempTweetCount = config.getTweetCount();
    char tempUser[100] = {};
    strcpy(tempUser, config.getUser().c_str());
    
    
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(DISPLAY_X, DISPLAY_Y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    al_set_target_backbuffer(display);

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

    ImGui::InputInt("Cantidad de tweets", &tempTweetCount, 1, 10);
    if (tempTweetCount < 1) {
        tempTweetCount = 1;
    }

    if (ImGui::Button("Aceptar", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y * 0.3))) {
        aceptar = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancelar", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y * 0.3))) {
        cancelarDescarga = true;
    }
    config.setTweetCount(tempTweetCount);

    //Ventana de configuraciones
    
    //Para variar la velocidad (n�mero float que va desde 0 hasta MAX_VEL)
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

}

bool Graphics::buttonAnterior() {
    if (this->anterior) {
        this->anterior = false;
        return true;
    }
    else {
        return false;
    }
}

bool Graphics::buttonSiguiente() {
    if (this->siguiente) {
        this->siguiente = false;
        return true;
    }
    else {
        return false;
    }
}

bool Graphics::buttonRepetir() {
    if (this->repetir) {
        this->repetir = false;
        return true;
    }
    else {
        return false;
    }
}

int Graphics::buttonLCD() {

    int numLcd = 0;

    if (this->lcd1) {
        numLcd = 1;
        this->lcd1 = false;
    }
    if (this->lcd2) {
        numLcd = 2;
        this->lcd2 = false;
    }
    if (this->lcd3) {
        numLcd = 3;
        this->lcd3 = false;
    }

    return numLcd;
}

bool Graphics::buttonAceptar() {

    if (this->aceptar) {
        this->aceptar = false;
        return true;
    }
    else {
        return false;
    }

}

bool Graphics::buttonCancelar() {

    if (this->cancelarDescarga) {
        this->cancelarDescarga = false;
        return true;
    }
    else {
        return false;
    }

}