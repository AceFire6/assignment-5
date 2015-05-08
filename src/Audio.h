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
                int numSamples = (int) (length / bitSize);
                duration = numSamples / sampleRate;

                infile.seekg (0, infile.beg);

                T * temp = new T[numSamples];
                infile.read((char *)temp, numSamples);

                audioData.resize((unsigned long) (numSamples));
                for (int i = 0; i < numSamples; ++i) {
                    audioData[i] = temp[i];
                }
                delete [] temp;
            } else {
                std::cout << "Couldn't open file: " << file << std::endl;
            }
            infile.close();
        }

        void saveFile(std::string file) {
            std::string fileName = file + "_" + std::to_string(sampleRate) + "_"
                                           + std::to_string(bitSize * 8) + "_mono.raw";
            std::ofstream outfile(fileName);

            if (outfile.is_open()) {
                for (auto data : audioData) {
                    outfile << data;
                }
            } else {
                std::cout << "Couldn't save file: " << fileName << std::endl;
            }
            outfile.close();
        }

        Audio & operator|(const Audio & rhs) {
            audioData.insert(audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
            return *this;
        }

        Audio & operator*(const std::pair<float, float> & volFactor) {
            for (int i = 0; i < audioData.size(); ++i) {
                audioData[i] *= volFactor.first;
            };
            return *this;
        }

        T clamp_sample(int sample) {
            if (bitSize == 1) {
                return (sample > INT8_MAX) ? INT8_MAX : sample;
            } else {
                return (sample > INT16_MAX) ? INT16_MAX : sample;
            }
        }

        Audio & operator+(const Audio & rhs) {
            for (int i = 0; i < audioData.size(); ++i) {
                audioData[i] = clamp_sample(audioData[i] + rhs.audioData[i]);
            }
            return *this;
        }

        Audio & operator^(const std::pair<int, int> & exclTime) {
            std::vector<T> newAudio;
            for (int i = 0; i < audioData.size(); ++i) {
                if (i < exclTime.first || i > exclTime.second) {
                    newAudio.push_back(audioData[i]);
                }
            }
            audioData = newAudio;
            return *this;
        }

        double calculateRMS() {
            double inverseSize = 1 / (float)audioData.size();
            return std::sqrt(inverseSize * std::accumulate(audioData.begin(), audioData.end(), 0,
                                                           [] (T a, T x) {return ((int)a + (x * x));}));
        }
    };


    template <typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T, T>> audioData;
        int bitSize;
        int sampleRate;
        double duration;
    public:
        Audio(std::string file, int sampleRate) {
            this->sampleRate = sampleRate;
            this->bitSize = (int) sizeof(T);
            std::cout << bitSize << std::endl;
            std::cout << "Pair: " << file << std::endl;
            readFile(file);
        }

        void readFile(std::string file) {
            std::ifstream infile(file);

            if (infile.is_open()) {
                infile.seekg (0, infile.end);
                long length = infile.tellg();
                int numSamples = (int) (length / (bitSize * 2));
                std::cout << numSamples << std::endl;
                duration = numSamples / sampleRate;

                infile.seekg (0, infile.beg);

                T * temp = new T[numSamples * 2];
                infile.read((char *)temp, numSamples * 2);

                audioData.resize((unsigned long) (numSamples));
                for (int i = 0; i < numSamples; ++i) {
                    audioData[i] = std::make_pair(temp[i], temp[i+1]);
                }
                delete [] temp;
            } else {
                std::cout << "Couldn't open file: " << file << std::endl;
            }
            infile.close();
        }

        void saveFile(std::string file) {
            std::string fileName = file + "_" + std::to_string(sampleRate) + "_"
                                   + std::to_string(bitSize * 8) + "_stereo.raw";
            std::ofstream outfile(fileName);

            if (outfile.is_open()) {
                for (auto data : audioData) {
                    outfile << data.first << data.second;
                }
            } else {
                std::cout << "Couldn't save file: " << fileName << std::endl;
            }
            outfile.close();
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

        Audio & operator|(const Audio & rhs) {
            audioData.insert(audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
            return *this;
        }

        Audio & operator*(const std::pair<float, float> & volFactor) {
            for (int i = 0; i < audioData.size(); ++i) {
                audioData[i] = std::make_pair(audioData[i].first * volFactor.first,
                                              audioData[i].second * volFactor.second);
            }
            return *this;
        }

        T clamp_sample(int sample) {
            if (bitSize == 1) {
                return (sample > INT8_MAX) ? INT8_MAX : sample;
            } else {
                return (sample > INT16_MAX) ? INT16_MAX : sample;
            }
        }

        Audio & operator+(const Audio & rhs) {
            for (int i = 0; i < audioData.size(); ++i) {
                T left = clamp_sample(audioData[i].first + rhs.audioData[i].first);
                T right = clamp_sample(audioData[i].second + rhs.audioData[i].second);
                audioData[i] = std::make_pair(left, right);
            }
            return *this;
        }

        Audio & operator^(const std::pair<int, int> & exclTime) {
            std::vector<std::pair<T, T>> newAudio;
            for (int i = 0; i < audioData.size(); ++i) {
                if (i < exclTime.first || i > exclTime.second) {
                    newAudio.push_back(audioData[i]);
                }
            }
            audioData = newAudio;
            return *this;
        }
    };
}

#endif //AUDIO_H
