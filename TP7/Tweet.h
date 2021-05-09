#ifndef _TWEET_H_
#define _TWEET_H_

#include <string>

class Tweet {
public:
	Tweet(std::string user, std::string text, std::string date);

	std::string getText();
	std::string getDate();
	std::string getUser();

private:
	std::string user;
	std::string text;
	std::string date;
};

#endif