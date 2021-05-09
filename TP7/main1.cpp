#include <iostream>
#include <string>
#include <vector>
#include "Tweet.h"
#include "TwitterAPI.h"

using namespace std;

void printTweets(vector<Tweet>& tweets);

int main() {

	string user;
	unsigned int count;

	cin >> user >> count;


	TwitterAPI api;

	if (!api.startTweetsDownload(user, count)) {
		cout << "No se pudo obtener los tweets: " << endl;
		cout << api.getError() << endl;
		return 0;
	}

	while (api.runDownload()) {
		cout << "Descargando..." << endl;
	}

	vector<Tweet> tweets;

	api.getTweets(tweets);

	cout << "Tweets retrieved from Twitter account: " << endl;
	printTweets(tweets);

	return 0;
}


//Funcion auxiliar para imprimir los tweets en pantalla una vez parseados
void printTweets(vector<Tweet>& tweets)
{
	for (auto t : tweets)
	{

		cout << t.getDate() << " - " << t.getUser() << " - " << t.getText() << endl;

		std::cout << "-----------------------------------------" << std::endl;
	}
}

