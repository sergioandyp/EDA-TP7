#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    Config();
    Config(float speed_, std::string user_, int count_);
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