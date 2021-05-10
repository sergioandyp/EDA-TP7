#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    Config();
    float getSpeed();
    std::string getUser();
    int getTweetCount();

    void setSpeed(float newSpeed);
    void setTweetCount(int newTweetCount);
    void setUser(std::string newUser); 

private:

    float       speed;
    std::string user;
    int         tweetCount;

};

#endif //CONFIG_H