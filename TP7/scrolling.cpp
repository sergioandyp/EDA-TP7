

#include"scrolling.h"
#include"basicLCD.h"



scrolling::scrolling() {
	cstr = NULL;
	vel = 0.0;
	completo = 0;
	posicion = 0;
	contador = 0;
	usuar = 0;
	twitMostrado = false;
	if (true == initallegro()) {
		error = false;
	}
	else
	{
		error = true;
	}
}
scrolling::~scrolling() {
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}
bool scrolling::initallegro(void) {
	if (!al_init()) {
		cout << "failed to initialize allegro!!" << endl;
		return false;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		cout << "failed to load event queue!" << endl;
		al_destroy_timer(timer);
		return false;
	}
	return true;
}
bool scrolling::scrollingOK(void) {
	if (error == true) {
		return false;
	}
	else
	{
		return true;
	}
}

void scrolling::printToDisplay(basicLCD* lcd, const unsigned char c) {
	(*lcd) << c;
	lcd->lcdMoveCursorLeft();

}
void scrolling::movechar(basicLCD* lcd, int cursor, const unsigned char c) {

	this->cursor.column = cursor;

	this->cursor.row = 1;
	lcd->lcdSetCursorPosition(this->cursor);
	printToDisplay(lcd, c);
}
//void config::movimiento(basicLCD* lcd, const unsigned char* msm, int letras,int len) {
//	int j = 0, rep, i, replocal;
//	if (completo == 0) {
//
//		for (rep = 0; rep < letras; rep++) {
//			for (i = abs(15 - rep), j = 0, replocal = 0; replocal <= rep; i++, j++, replocal++) {
//
//				movechar(lcd, i, msm[j]);
//				if (j == 15) {
//					std::cout << "hoal" << std::endl;
//					completo=completo + 1;				//ojo cada vez que se cambie de twit hay q resetear
//				}
//			}
//		}
//	}
//}

void scrolling::titulo(basicLCD* lcd, string usuario) {
	cursor.column = 0;
	cursor.row = 0;
	string str = usuario.substr(0, 15);
	char* cstr = new char[str.length() + 1];
	(*lcd) << cstr;

}





bool scrolling::timerDisplay(basicLCD* lcd, string twit,string  usuario,float velocidad) {
	if (usuar ==0 && twitMostrado==false) {
		titulo(lcd, usuario);
		cout << "hola" << endl;
		usuar = 1;
	}
	
	setVel(velocidad);
	 if (true==scrollingOK()) {	
		 ALLEGRO_EVENT ev;
		 if (twitMostrado ==true) {
			 return true;
		 }
		 if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, .
		 {
			 if (ev.type == ALLEGRO_EVENT_TIMER){
				 contador++;
				 ToDisplay(lcd,twit, contador);
				 return false;
			 }
			 else {
				 return false;
			 }
		 }

	 }
	 else {
		 return false;
	 }
}


void scrolling::movimiento(basicLCD* lcd, std::string twit, int letra) {
	cursor.column = 15;
	cursor.row = 1;
	std::string str;

	if ((cursor.column - letra >= 0) && ((twit.length() + 1) >= 16)) {
		str = twit.substr(posicion, letra);
		char* cstr = new char[str.length() + 1];						//
		std::strcpy(cstr, str.c_str());
		cursor.column = cursor.column - letra;
		//		cout << cursor.column << endl;
		lcd->lcdSetCursorPosition(cursor);
		(*lcd) << (const  char*)cstr;

		delete[] cstr;
	}

	else {
		completo = 1;
	}
}




void scrolling::movimFinal(basicLCD* lcd, std::string twit) {
	if (posicion < twit.length() + 1) {
		std::string str = twit.substr(posicion, 16);
		char* cstr = new char[str.length() + 1];						//
		std::strcpy(cstr, str.c_str());
		cursor.column = 0;
		lcd->lcdSetCursorPosition(cursor);
		(*lcd) << (const  char*)cstr;
		lcd->lcdClearToEOL();
		delete[] cstr;
		posicion++;
	}
	else {
		posicion = 0;
		completo = 0;
		twitMostrado = true;
		cursor.column = 0;
		cursor.row = 1;
		lcd->lcdClear();
	}
}



float scrolling::getVel(void) {
	return this->vel;
}
void scrolling::setVel(float vel) {
	if (this->vel != vel) {

		al_destroy_timer(timer);
		cout << "setvel" << endl;
		timer = al_create_timer(vel); //crea el timer pero NO empieza a correr
		if (!timer) {
			fprintf(stderr, "failed to create timer!\n");
			error = false;
		}
		this->vel = vel;
		cout << "setvel" << endl;
		//startTimer();
		al_register_event_source(event_queue, al_get_timer_event_source(timer));

		al_start_timer(timer); //Recien aca EMPIEZA el timer
	}
}
void scrolling::ToDisplay(basicLCD* lcd, std::string twit, int letras) {	//funcion que recibe un string y lo muestra en el display letra x letra
	string s = completaString(twit);
	cout << s << endl;
	if (completo == 0) {		//si no esta completo
		movimiento(lcd, s, letras);
	}
	else {			//si esta completo, todos los 16 caracters mostrados en el display
		movimFinal(lcd, s);
	}

}



string scrolling::completaString(string twit) {
	std::string s = "               ";		//string completador
	int indice = 0;
	if (twit.length() + 1 < 16) {
		indice = 16 - twit.length();
		s = s.substr(0, indice);
		return(twit + s);
	}
	else
	{
		return(twit);
	}
}



void scrolling::settwitMostrado(bool completo) {
	this->twitMostrado = completo;
}
bool scrolling::gettwitMostrado(void) {
	return twitMostrado;
}
