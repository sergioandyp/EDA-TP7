

#include"scrolling.h"
#include"basicLCD.h"

scrolling::scrolling() {
	cola = 1;
	completo = 0;
	vel = 1.0;
	posicion = 0;

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

void scrolling::movimiento(basicLCD* lcd, std::string twit, int letra) {
	cursor.column = 15;
	cursor.row = 1;
	if (letra < twit.length() + 1) {
		std::string str = twit.substr(0, letra);
		char* cstr = new char[str.length() + 1];						//
		std::strcpy(cstr, str.c_str());
		cursor.column = cursor.column - letra;
		//		cout << cursor.column << endl;
		lcd->lcdSetCursorPosition(cursor);
		(*lcd) << (const unsigned char*)cstr;

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
		(*lcd) << (const unsigned char*)cstr;
		lcd->lcdClearToEOL();
		//std::cout << posicion<< std::endl;
		delete[] cstr;
		posicion++;
	}
	else {
		posicion = 0;
		completo = 0;
		cursor.column = 0;
		cursor.row = 1;
		lcd->lcdClearToEOL();

	}
}



float scrolling::getVel(void) {
	return this->vel;
}
void scrolling::setVel(float vel) {
	this->vel = vel;
}
void scrolling::ToDisplay(basicLCD* lcd, std::string twit, int letras) {	//funcion que recibe un string y lo muestra en el display letra x letra
	//char* cstr = new char[twit.length() + 1];						//
	//std::strcpy(cstr, twit.c_str());

	if (completo == 0) {		//si no esta completo
		movimiento(lcd, twit, letras);
	}
	else {			//si esta completo, todos los 16 caracters mostrados en el display
		movimFinal(lcd, twit);
	}

}

/*void config::strings(basicLCD* lcd, std::string twit, int letras) {
	cola = modulo(twit.length() + 1);
	std::cout << cola << std::endl;

	if (cola == 1) {
		ToDisplay(lcd, twit, letras);
	}
	else {
		std::cout << "hola" << std::endl;
		std:string s=divisor(twit);
		ToDisplay(lcd,s, letras);
	}
}*/

//std::string config::divisor(std::string twit) {
//	int j = cola - completo;
//	for (int i = cola; i > j; limite.column + 15, limite.row + 15,j+1) {
//
//	}
//	std::cout << limite.column << std::endl;
//	return(twit.substr(limite.column, limite.row));
//}


int scrolling::modulo(int len) {
	if ((len % 16) == 0) {
		return (len / 16);
	}
	else {
		return((len / 16) + 1);
	}
}


void scrolling::setcompleto(int completo) {
	this->completo = completo;
}
int scrolling::getcompleto(void) {
	return completo;
}

