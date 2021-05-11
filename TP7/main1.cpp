#include <iostream>
#include <string>
#include <vector>
#include "Tweet.h"
#include "TwitterAPI.h"

using namespace std;

void printTweets(vector<Tweet>& tweets);
//
//int main() {
//
//	string user;
//	unsigned int count;
//	int N;
//	cin >> N;
//
//	for (int i = 0; i < N; i++) {
//		cin >> user >> count;
//
//		TwitterAPI api;
//
//		if (!api.startTweetsDownload(user, count)) {
//			cout << "No se pudo obtener los tweets: " << endl;
//			cout << api.getError() << endl;
//			return 0;
//		}
//
//		while (api.runDownload()) {
//			cout << "Descargando..." << endl;
//			if (rand() % 10 == 0) {
//				//cout << "Detengo" << endl;
//				//api.stopDownload();
//			}
//		}
//
//		vector<Tweet> tweets;
//
//		if (!api.getTweets(tweets)) {
//			cout << "Error al obtener los tweets: " << endl << api.getError() << endl;
//			return 0;
//		}
//
//		cout << "Tweets retrieved from Twitter account: " << endl;
//		printTweets(tweets);
//	
//	}
//
//	return 0;
//}


//Funcion auxiliar para imprimir los tweets en pantalla una vez parseados
void printTweets(vector<Tweet>& tweets)
{
	for (auto& t : tweets)
	{
		cout << t.getDate() << " - " << t.getUser() << " - " << t.getText() << endl;
		std::cout << "-----------------------------------------" << std::endl;
	}
}

