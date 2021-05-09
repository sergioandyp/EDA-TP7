#ifndef _TWITTER_API_H_
#define _TWITTER_API_H_

#include "Tweet.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

// WAITING: No hay procesos corriendo
// RUNNING: Hay un proceso en ejecucion
// READY: Hay resultados esperando a ser obtenidos
enum STATE { WAITING, RUNNING, READY };

class TwitterAPI {
public:

	// Comienza la decarga de tweets del usuario especificado
	// Tener en cuenta que la cantidad de tweets puede ser menor al count pasado
	bool startTweetsDownload(std::string user, unsigned int count);
	bool startTweetsDownload(std::string user);

	// Corre un ciclo de la descarga, se debe llamar continuamente hasta que
	// la descarga termine o sea cancelada.
	// Devuelve 0 si se completo la descarga o hubo error
	bool runDownload();

	// Detiene la descarga de tweets en curso.
	void stopDownload();

	// Devuelve los últimos tweets del ususario solicitado.
	// Tener en cuenta que la cantidad de tweets puede ser menor al count pasado
	// Devuelve 0 si hubo error. En caso contrario, agrega los tweets del usuario al vector
	bool getTweets(std::vector<Tweet>& tweets);

	// Devuelve el texto del error que se provoco si una funcion devolvio error
	// Si no hay error, puede devlver el último error o un string vacio, por lo que hay
	// que asegurarse de llamar a esta función solo si se obtuvo un error nuevo.
	std::string getError();

private:

	std::string error;
	STATE state;
	std::string response;
	int runningDownload;
	CURL* curl;
	CURLM* multiHandle;

	// Devuelve en response el JSON que se obtiene de la request a Twitter
	bool getTweetsResponse(nlohmann::json& response);

	std::string makeQuery(std::string user, unsigned int count);
	std::string makeQuery(std::string user);
};

#endif