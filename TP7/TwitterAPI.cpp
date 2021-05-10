#include "TwitterAPI.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// URL base de la API de Twitter que se va a utilizar, luego se agregan los parametros
#define API_BASE_URL	"https://api.twitter.com/1.1/statuses/user_timeline.json"

// Bearer Token generado directamente desde la Developer Console de Twitter
#define BEARER_TOKEN	"AAAAAAAAAAAAAAAAAAAAAHE8PQEAAAAAlZ%2BWWSwi%2B%2FbKZDlTAfGzWY%2BM7eo%3DB8WeqY3ZZOxTHfuzcaodd5StXAPFa3qp9Vfo1Svg3aE68egkqV"

//#define DEBUG		// Habilita los logs para debugear

using namespace std;
using json = nlohmann::json;

static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);

// Recibe en time la fecha y hora en el formato de Twitter y
// la devuelve en un formato de 16 caracteres.
// Si devuelve 1, no hubo error y se devuelve en time el nuevo formato.
// Si devuelve 0, huo error y time no se modifica.
bool formatTime(string& time);

bool TwitterAPI::startTweetsDownload(string user, unsigned int count) {

	string token = BEARER_TOKEN;

	string query = makeQuery(user, count);

#ifdef DEBUG
	cout << "El query es: " << query << endl;
#endif

	curl = curl_easy_init();
	multiHandle = curl_multi_init();

	if ((curl != NULL) & (multiHandle != NULL))
	{
		// Attacheo el easy handle para manejar una conexion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		// Seteamos URL FOLLOWLOCATION y los protocolos a utilizar.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		// Construimos el Header de autenticacion como lo especifica la API
		// usando el bearer token
		struct curl_slist* list = NULL;
		string aux = "Authorization: Bearer " + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		// Seteamos los callback
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// Realizamos ahora un perform no bloqueante
		if (curl_multi_perform(multiHandle, &runningDownload) != CURLM_OK) {
			error = "Curl perform return a error";
			state = WAITING;
			return 0;
		}

	}
	else {
		error = "Cannot download tweets. Unable to start cURL";
		state = WAITING;
		return 0;
	}

	state = RUNNING;
	return 1;	// Salida sin error
}


bool TwitterAPI::runDownload() {
	if (state == RUNNING && runningDownload) {
		CURLMcode res = curl_multi_perform(multiHandle, &runningDownload);
		if (!runningDownload) {	// Fin de la descarga
			stopDownload();
			state = READY;
			return 0;
		}
		
		if (res == CURLM_OK) return 1;
		else stopDownload();
	}
	
	// Error
	return 0;
}

void TwitterAPI::stopDownload() {
	curl_multi_remove_handle(multiHandle, curl);
	curl_easy_cleanup(curl);
	curl_multi_cleanup(multiHandle);
	runningDownload = false;
	state = WAITING;
}



bool TwitterAPI::getTweets(vector<Tweet>& tweets) {

	if (state == READY) {

		state = WAITING;

		json responseJSON;

		if (!getTweetsResponse(responseJSON)) return 0;

		try
		{
			if (responseJSON.empty()) {
				error = "Error: User has no tweets to show";
				return 0;
			}

			if (responseJSON.find("errors") == responseJSON.end()) {		// Vemos si la request se hizo bien
				//Al ser el JSON un arreglo de objetos JSON se busca el campo para cada elemento
				for (auto tweet : responseJSON) {
					string tweet_user = tweet["user"]["name"];
					string tweet_text = tweet["text"];
					string tweet_date = tweet["created_at"];

					if (!formatTime(tweet_date)) {		// Se formatea la fecha a 16 caracteres
						error = "Error parsing tweet date";
						return 0;
					}

					// Si esta truncado, o retwiteado saco el link del final
					if (tweet["truncated"] || tweet.find("retweeted_status") != tweet.end()) {
						tweet_text = tweet_text.substr(0, tweet_text.rfind(" https"));
						tweet_text += "...";
					}

#ifdef DEBUG
					cout << tweet_user << " - " << tweet_date << " - " << tweet_text << endl;
					cout << "_______________________________________________________________" << endl;
#endif

					tweets.push_back(Tweet(tweet_user, tweet_text, tweet_date));
				}
			}
			else {
				error = "Error from Twitter API: \n";
				for (auto& e : responseJSON["errors"]) {
					error += "Error " + to_string(e["code"]) + ": " + string(e["message"]);
				}
				return 0;
			}


		}
		catch (exception& e)
		{
			// Devuelvo si hubo un error de la libreria
			error = e.what();
			return 0;
		}

		return 1;
	}
	else {
		error = "Tweets are not ready yet";
		return 0;
	}

}

bool TwitterAPI::getTweetsResponse(json& responseJSON) {

		// Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		// con toda la informacion de los tweets que le pedimos
		try {
			responseJSON = json::parse(response);
		}
		catch (exception& e) {
			error = "Error parsing response: " + string(e.what());
			return 0;
		}

#ifdef DEBUG
		cout << response << endl;
#endif

	return 1;	// Salida sin error
}

// Devuelvo el error
string TwitterAPI::getError() {
	return error;
}


string TwitterAPI::makeQuery(string user, unsigned int count) {

	string query = API_BASE_URL;

	map<string, string> params;
	params["screen_name"] = user;
	//params["exclude_replies"] = "true";
	//params["include_rts"] = "false";
	if (count > 0) params["count"] = to_string(count);

	query += '?';
	for (auto i : params) {
		query += i.first + '=' + i.second + '&';
	}

	if (*(query.end() - 1) == '&') {
		query.pop_back();
	}

	return query;
}

// Recibe en time la fecha y hora en el formato de Twitter y
// la devuelve en un formato de 16 caracteres.
// Si devuelve 1, no hubo error y se devuelve en time el nuevo formato.
// Si devuelve 0, huo error y time no se modifica.
bool formatTime(string& time) {
	tm t = {};
	stringstream ss(time);
	ss >> get_time(&t, "%a %b %d %H:%M:%S +0000 %Y"); // Parseamos la fecha y hora segun el formato de Twitter

	if (ss.fail()) {
		return 0;
	}
	else {
		stringstream res;		// Un nuevo stream para no pisar el anterior
		res << put_time(&t, "%d/%m/%y - %H:%M");
		time = res.str();
		return 1;
	}
}

//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	string* s = (string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}
