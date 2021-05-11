#include "Config.h"

using namespace std;

Config::Config() : speed(), user(), tweetCount()
{
}

Config::Config(float speed_, string user_, int count_) : speed(speed_), user(user_), tweetCount(count_)
{

}

float Config::getSpeed() {
    return this->speed;
}

std::string Config::getUser() {
    return this->user;
}

int Config::getTweetCount() {
    return this->tweetCount;
}

void Config::setSpeed(float newSpeed) {
    this->speed = newSpeed;
}

void Config::setTweetCount(int newTweetCount) {
    this->tweetCount = newTweetCount;
}

void Config::setUser(string newUser) {
    this->user = newUser;
}