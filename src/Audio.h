#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <numeric>
#include <cmath>


namespace MLLJET001 {
    template <typename T, int numChannels> class Audio {
    protected:
        int bitSize;
    public:
        Audio() {}
    };


    template <typename T> class Audio<T, 1> {
    private:
        std::vector<T> audioData;
        int bitSize;
    public:
        Audio() {}
        Audio(std::string file) {
            this->bitSize = (int) (sizeof(T) * 8);
            std::cout << "No Pair" << std::endl;
            readFile(file);
        }

        void readFile(std::string file) {
            std::ifstream infile(file);

            if (infile.is_open()) {
                infile.seekg (0, infile.end);
                long length = infile.tellg();
                infile.seekg (0, infile.beg);
            }

            infile.close();
        }

        double calculateRMS() {
            double inverseSize = 1 / audioData.size();
            return std::sqrt(inverseSize * std::accumulate(audioData.begin(), audioData.end(), 0, [] (int a, T x) {return x * x;}));
        }
    };


    template <typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T, T>> audioData;
        int bitSize;
    public:
        Audio(std::string file) {
            this->bitSize = (int) (sizeof(T) * 8);
            std::cout << "Pair" << std::endl;
        }

        std::pair<double, double> calculateRMS() {
            std::pair<double, double> rms;
            double inverseSize = 1 / audioData.size();

            rms = std::accumulate(audioData.begin(), audioData.end(), std::pair<double, double>(0, 0),
                            [] (std::pair<double, double> accum, std::pair<T, T> x) {
                                accum = std::make_pair(accum.first + (std::pow(x.first, 2)), accum.second + (std::pow(x.second, 2)));
                                return accum;
                            });
            rms = std::make_pair(std::sqrt(rms.first * inverseSize), std::sqrt(rms.second * inverseSize));
            return rms;
        }
    };
}

#endif //AUDIO_H
