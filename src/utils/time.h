#ifndef TIME_H
#define TIME_H
#include <string>
using namespace std;

class Time {
public:
    Time(int seconds) : seconds(seconds) {};

    string toString(){
        if(seconds > 3600){
            int hours = seconds/3600;
            int minutes = (seconds-hours*3600)/60;
            int _seconds = seconds - hours*3600 - minutes*60;
            return to_string(hours)+" hs "+to_string(minutes)+" min "+to_string(_seconds)+" seconds";
        }
        else if(seconds > 60){
            int minutes = seconds/60;
            return to_string(minutes)+" min "+to_string(seconds - minutes*60)+" seconds";
        }
        return to_string(seconds)+" seconds";
    }
private:
    int seconds;
};

#endif