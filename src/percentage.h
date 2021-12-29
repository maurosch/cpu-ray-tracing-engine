
#include <iostream>
#include <chrono>
#include <vector>
#include <atomic> 
#include <thread> 
using namespace std;

inline std::string printTime(int seconds){
    if(seconds > 60){
        int minutes = seconds/60;
        return std::to_string(minutes)+" min "+std::to_string(seconds - minutes*60)+" seconds";
    }
    return std::to_string(seconds)+" seconds";
}

class PercentageIndicator {
    public:
        PercentageIndicator(){
            _percentage = 0;
            lastPercentage = 0;
            lastTime = std::chrono::system_clock::now();
            _eta = 0;
        }
        int percentage(){
            return _percentage*100;
        }
        int eta(){
            return _eta;
        }
        void update(float newPercentage){
            auto time = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = time - lastTime;
            if(elapsed_seconds.count() > 2){
                lastTime = time;
                _eta = (1-newPercentage)/(newPercentage-lastPercentage)*elapsed_seconds.count();
                lastPercentage = newPercentage;
            }
            _percentage = newPercentage;
        };
        void print(){
            std::cerr << "\r";
            std::cerr << "| " << int(_percentage*100) << "% | ";
            std::cerr << "ETA: " << printTime(_eta) << std::flush;
        };
    private:
        std::chrono::system_clock::time_point lastTime;
        float _eta;
        float lastPercentage;
        float _percentage;
};

class MulticorePercentageIndicator {
    std::vector<PercentageIndicator> percentages;
    int amount_cores;
    bool keepPrinting;
    std::thread printerThread;

    public:
        MulticorePercentageIndicator(int _amount_cores){
            percentages = std::vector<PercentageIndicator>(_amount_cores);
            amount_cores = _amount_cores;
        }
        void startPrinting(){
            keepPrinting = true;
            printerThread = std::thread([&]() {
                while(keepPrinting) {
                    print();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
        void print(){
            std::cerr << "\r";
            for(int p = 0; p < amount_cores; p++){
                std::cerr << percentages[p].percentage() << "% | ";
            }
            int maxEta = 0;
            for(int p = 0; p < amount_cores; p++){
                maxEta = std::max(maxEta, percentages[p].eta());
            }
            std::cerr << "ETA: " << printTime(maxEta) << std::flush;
        };
};