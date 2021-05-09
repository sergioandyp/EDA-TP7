#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_allegro5.h"
#include "graphics.h"
#include <string>

#define MAX_VEL 10
#define INIT_VEL 5
using namespace std;

int main(void) {


    //CONFIGURACIONES PARA EMPEZAR
    char usuario[10];
    int cantidadTweets=0;

    //CONFIGURACIONES MIENTRAS MUESTRO
    bool doExit = false;
    int lcd = 0;
    float velocidad = INIT_VEL;
    bool siguiente=false;
    bool repetir = false;
    bool anterior=false;

    //Inicializo la parte gráfica: display de allegro, imGui, cola de eventos de allegro... 
    if (!initGraphics()) {
        return 1;
    }


    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        drawInit(usuario, cantidadTweets,doExit);

    }
    doExit = false;
    while (!doExit) {

        //Dibujo las configuraiones dentro del display
        drawConfig(velocidad, lcd,doExit, siguiente, repetir, anterior);
        //al_flip_display();
        if (siguiente) {
            cout << "siguiente" << endl;
            siguiente = false;
        }
        if (anterior) {
            cout << "anterior" << endl;
            anterior = false;
        }
        if (repetir) {
            cout << "pasame la repe" << endl;
            repetir = false;
        }
    }


    /*ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;*/



    
    
   /* while (!doExit)
    {
        al_get_next_event(queue, &ev);
        ImGui_ImplAllegro5_ProcessEvent(&ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            doExit = true;


        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(300, 100));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("MENU", NULL, window_flags);

        ImGui::SliderFloat("Global Speed", &velocidad, 0, MAX_VEL, "%.2f");
        if (velocidad <= 0.01f) {
            velocidad = 0.01f;
        }

        ImGui::End();
        ImGui::Render();
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

        al_flip_display();

    }*/


    
    destroyGraphics();

	return 0;
}