#include "Tweet.h"

using namespace std;

Tweet::Tweet(string user, string text, string date)
	: user(user), text(text), date(date) {
}

string Tweet::getUser() {
	return user;
}

string Tweet::getText() {
	return text;
}

string Tweet::getDate() {
	return date;
}