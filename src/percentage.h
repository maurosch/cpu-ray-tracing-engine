#ifndef PERCENTAGE_H
#define PERCENTAGE_H

#include <iostream>
#include <chrono>
#include <vector>
#include <atomic> 
#ifndef DISABLE_THREADS
    #include <thread> 
#endif
#include "utils/time.h" 
using namespace std;
using namespace chrono;

class PercentageIndicator {
    public:
        PercentageIndicator(){
            _percentage = 0;
            lastPercentage = 0;
            lastTime = system_clock::now();
            _eta = 0;
        }
        int percentage(){
            return _percentage*100;
        }
        int et(){
            updateET();
            return _eta;
        }
        void updateET(){
            auto elapsed_seconds = duration_cast<seconds> (
                system_clock::now() - lastTime
            ).count();
            
            _eta = (1-_percentage)/_percentage*elapsed_seconds;
            
            lastPercentage = _percentage;
        };
        void update(float newPercentage){
            _percentage = newPercentage;
        };
        void print(ostream &out){
            updateET();
            out << "\r";
            out << "| " << int(_percentage*100) << "% | ";
            out << "ETA: " << Time(_eta).toString() << flush;
        };
    private:
        system_clock::time_point lastTime;
        float _eta;
        float lastPercentage;
        float _percentage;
};

#ifndef DISABLE_THREADS
class MultiThreadPercentageIndicator {
    private:
        vector<PercentageIndicator> percentages;
        int amountThreads;
        bool keepPrinting;
        thread printerThread;

    public:
        MultiThreadPercentageIndicator(int _amountThreads){
            percentages = vector<PercentageIndicator>(_amountThreads);
            amountThreads = _amountThreads;
        }
        void startPrinting(ostream &out){
            keepPrinting = true;
            printerThread = thread([&]() {
                while(keepPrinting) {
                    print(out);
                    this_thread::sleep_for(milliseconds(1000));
                }
            });
        }
        void stopPrinting(){
            keepPrinting = false;
            printerThread.join();
        }
        void update(int coreNumber, float newPercentage){
            percentages[coreNumber].update(newPercentage);
        }
        void print(ostream &out){
            out << "\r| ";
            for(int p = 0; p < amountThreads; p++){
                out << percentages[p].percentage() << "% | ";
            }
            int maxEta = 0;
            for(int p = 0; p < amountThreads; p++){
                maxEta = max(maxEta, percentages[p].et());
            }
            out << "Estimated Time: " << Time(maxEta).toString() << flush;
        };
};
#endif

#endif