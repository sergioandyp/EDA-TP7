#ifndef _TWITTER_API_H_
#define _TWITTER_API_H_

#include "Tweet.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class TwitterAPI {
public:

	// Devuelve los últimos tweets del ususario solicitado.
	// Tener en cuenta que la cantidad de tweets puede ser menor al count pasado
	// Devuelve 0 si hubo error. En caso contrario, agrega los tweets del usuario al vector
	static bool getTweets(std::string user, unsigned int count, std::vector<Tweet>& tweets);
	static bool getTweets(std::string user, std::vector<Tweet>& tweets);

private:
	// Devuelve en response el JSON que se obtiene de la request a Twitter
	static bool getTweetsResponse(std::string user, unsigned int count, nlohmann::json& response);
};

#endif