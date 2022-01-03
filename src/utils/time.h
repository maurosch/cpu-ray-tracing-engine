#ifndef TIME_H
#define TIME_H
#include <string>
using namespace std;

class Time {
public:
    Time(int seconds) : seconds(seconds) {};

    string toString(){
        if(seconds > 60){
            int minutes = seconds/60;
            return to_string(minutes)+" min "+to_string(seconds - minutes*60)+" seconds";
        }
        return to_string(seconds)+" seconds";
    }
private:
    int seconds;
};

#endif