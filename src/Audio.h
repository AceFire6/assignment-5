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
    private:
        std::vector<T> audioData;
        int bitSize;
        int sampleRate;
        double duration;
    public:
        Audio(std::string file, int sampleRate) {
            this->bitSize = (int) sizeof(T);
            this->sampleRate = sampleRate;
            std::cout << "No Pair" << std::endl;
            readFile(file);
        }

        void readFile(std::string file) {
            std::ifstream infile(file);

            if (infile.is_open()) {
                infile.seekg (0, infile.end);
                long length = infile.tellg();
                duration = (length / bitSize) / sampleRate;

                infile.seekg (0, infile.beg);

                float * temp = new float[length / bitSize];
                infile.read((char *) temp, length / bitSize);

                audioData.resize((unsigned long) (length / bitSize));
                for (int i = 0; i < length / bitSize; ++i) {
                    audioData[i] = temp[i];
                }
                delete [] temp;
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
        int sampleRate;
    public:
        Audio(std::string file, int sampleRate) {
            this->sampleRate = sampleRate;
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
