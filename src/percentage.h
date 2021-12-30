
#include <iostream>
#include <chrono>
#include <vector>
#include <atomic> 
#ifndef DISABLE_THREADS
    #include <thread> 
#endif
using namespace std;

inline string printTime(int seconds){
    if(seconds > 60){
        int minutes = seconds/60;
        return to_string(minutes)+" min "+to_string(seconds - minutes*60)+" seconds";
    }
    return to_string(seconds)+" seconds";
}

class PercentageIndicator {
    public:
        PercentageIndicator(){
            _percentage = 0;
            lastPercentage = 0;
            lastTime = chrono::system_clock::now();
            _eta = 0;
        }
        int percentage(){
            return _percentage*100;
        }
        int eta(){
            return _eta;
        }
        void update(float newPercentage){
            auto time = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = time - lastTime;
            if(elapsed_seconds.count() > 2){
                lastTime = time;
                _eta = (1-newPercentage)/(newPercentage-lastPercentage)*elapsed_seconds.count();
                lastPercentage = newPercentage;
            }
            _percentage = newPercentage;
        };
        void print(ostream &out){
            out << "\r";
            out << "| " << int(_percentage*100) << "% | ";
            out << "ETA: " << printTime(_eta) << flush;
        };
    private:
        chrono::system_clock::time_point lastTime;
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
                    this_thread::sleep_for(chrono::milliseconds(1000));
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
            out << "\r";
            for(int p = 0; p < amountThreads; p++){
                out << percentages[p].percentage() << "% | ";
            }
            int maxEta = 0;
            for(int p = 0; p < amountThreads; p++){
                maxEta = max(maxEta, percentages[p].eta());
            }
            out << "ETA: " << printTime(maxEta) << flush;
        };
};
#endif