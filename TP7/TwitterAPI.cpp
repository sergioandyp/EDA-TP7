#include "TwitterAPI.h"
#include <curl/curl.h>
#include <iostream>

// URL base de la API de Twitter que se va a utilizar, luego se agregan los parametros
#define API_BASE_URL	"https://api.twitter.com/1.1/statuses/user_timeline.json"

// Bearer Token generado directamente desde la Developer Console de Twitter
#define BEARER_TOKEN	"AAAAAAAAAAAAAAAAAAAAAHE8PQEAAAAAlZ%2BWWSwi%2B%2FbKZDlTAfGzWY%2BM7eo%3DB8WeqY3ZZOxTHfuzcaodd5StXAPFa3qp9Vfo1Svg3aE68egkqV"

//#define DEBUG		// Habilita los logs para debugear

using namespace std;
using json = nlohmann::json;

static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);


bool TwitterAPI::getTweets(string user, unsigned int count, vector<Tweet>& tweets) {

	json response;

	if (!getTweetsResponse(user, count, response)) return 0;

	try
	{
		if (response.empty()) {
			error = "Error: User has no tweets to show";
			return 0;
		}

		if (response.find("errors") == response.end()) {		// Vemos si la request se hizo bien
			//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
			for (auto tweet : response) {
				string tweet_user = tweet["user"]["name"];
				string tweet_date = tweet["created_at"];
				string tweet_text = tweet["text"];
				
				// Si esta truncado, o retwiteado saco el link del final
				if (tweet["truncated"] || tweet.find("retweeted_status") != tweet.end()) {
					tweet_text = tweet_text.substr(0, tweet_text.rfind(" https"));
					tweet_text += "...";
				}
				
				tweets.push_back(Tweet(tweet_user, tweet_text, tweet_date));
			}
		}
		else {
			error = "Error from Twitter API: \n";
			for (auto e : response["errors"]) {
				error += "Error " + string(e["code"]) + ": " + string(e["message"]);
			}
			return 0;
		}


	}
	catch (exception& e)
	{
		//Muestro si hubo un error de la libreria
		error = e.what();
		return 0;
	}

	return 1;

}

bool TwitterAPI::getTweetsResponse(string user, unsigned int count, json& response) {
	
	string token = BEARER_TOKEN;

	string query = API_BASE_URL;

	map<string, string> params;
	params["screen_name"] = user;
	//params["exclude_replies"] = "true";
	if (count>0) params["count"] = to_string(count);

	query += '?';
	for (auto i : params) {
		query += i.first + '=' + i.second + '&';
	}

	if (*(query.end() - 1) == '&') {
		query.pop_back();
	}

#ifdef DEBUG
	cout << "El query es: " << query << endl;
#endif

	CURL* curl = curl_easy_init();
	CURLM* multiHandle = curl_multi_init();
	CURLMcode res;

	string readString = "";

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
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		int stillRunning = 0;
		// Realizamos ahora un perform no bloqueante
		res = curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning)
		{
			// Debemos hacer polling de la transferencia hasta que haya terminado
			res = curl_multi_perform(multiHandle, &stillRunning);

			// Mientras tanto podemos hacer otras cosas

			// COMO PODER CANCELARLO Y ESAS COSAS


		}

		// Siempre realizamos el cleanup al final
		curl_easy_cleanup(curl);

		// Checkeamos errores
		if (res != CURLM_OK)
		{
			error = "Error en la conexion con la API de Twitter";
			return 0;
		}

		// Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		// con toda la informacion de los tweets que le pedimos
		try {
			response = json::parse(readString);
		}
		catch (exception& e) {
			error = "Error parsing response: " + string(e.what());
			return 0;
		}

#ifdef DEBUG
		cout << response << endl;
#endif

	}
	else {
		error = "Cannot download tweets. Unable to start cURL";
		return 0;
	}

	return 1;	// Salida sin error
}

// Devuelvo el error
string TwitterAPI::getError() {
	return error;
}

//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	string* s = (string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}
