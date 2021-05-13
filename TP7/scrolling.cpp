

#include"scrolling.h"
#include"basicLCD.h"



void scrolling::titulo(basicLCD* lcd, string usuario) {
	cursor.column = 0;
	cursor.row = 0;
	lcd->lcdSetCursorPosition(cursor);
	string s = usuario.substr(0, 16);
	cout << s << endl;
	char* cstr = new char[s.length() + 1];						//
	std::strcpy(cstr, s.c_str());

	(*lcd) << (const  char*)cstr;

}
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

bool scrolling::timerDisplay(basicLCD* lcd, string twit, string  usuario, int velocidad) {
	if (usuar == 0 && twitMostrado == false) {
		cout << "titulo" << endl;
		titulo(lcd, usuario);
		usuar = 1;

	}

	setVel(velocidad);
	if (true == scrollingOK()) {
		if (twitMostrado == true) {
			cout << "llegue la final del tweet" << endl;
			return twitMostrado;
		}
		if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, .
		{
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				contador++;
				ToDisplay(lcd, twit, contador);
				//cout << "hubo ev" << endl;
				return false;
			}
			else {
				cout << "el ev detectato no es de tpo" << endl;
				return false;
			}
		}
		else {
			//cout << "no detecto un evento" << endl;
			return false;
		}

	}
	else {
		cout << "fallo en allegro" << endl;
		return false;
	}
}
float scrolling::velelegida(int vel) {
	switch (vel)
	{
	case 1: return 3.0; break;
	case 2: return 2.5; break;
	case 3: return 2.0; break;
	case 4: return 1.5; break;
	case 5: return 1.0; break;
	case 6: return 0.5; break;
	case 7: return 0.4; break;
	case 8: return 0.3; break;
	case 9: return 0.2; break;
	case 10: return 0.1; break;
	default: return 0.5;
		break;
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
		(*lcd) << (const char*)cstr;
		lcd->lcdClearToEOL();
		delete[] cstr;
		posicion++;
	}
	else {
		posicion = 0;
		completo = 0;
		contador = 0;
		twitMostrado = true;
		usuar = 0;
		cursor.column = 15;
		cursor.row = 1;
		lcd->lcdSetCursorPosition(cursor);
		lcd->lcdClearToEOL();

	}
}


float scrolling::getVel(void) {
	return this->vel;
}
void scrolling::setVel(int vel) {
	float velseleccionada = 0;
	if (this->vel != vel) {
		//cout << "se crea timer" << endl;
		velseleccionada = velelegida(vel);
		al_destroy_timer(timer);
		timer = al_create_timer(velseleccionada); //crea el timer pero NO empieza a correr
		if (!timer) {
			fprintf(stderr, "failed to create timer!\n");
			error = false;
		}
		this->vel = vel;
		al_register_event_source(event_queue, al_get_timer_event_source(timer));

		al_start_timer(timer); //Recien aca EMPIEZA el timer
	}
}
void scrolling::ToDisplay(basicLCD* lcd, std::string twit, int letras) {	//funcion que recibe un string y lo muestra en el display letra x letra
	string s = completaString(twit);

	if (completo == 0) {		//si no esta completo

		movimiento(lcd, s, letras);
		//cout << "despues"<<completo << endl;
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

