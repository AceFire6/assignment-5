#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace MLLJET001 {
    template <typename T, int numChannels> class Audio {
    private:
        std::vector<T> audioData;
        int tSize;
        int sampleRate;
        double duration;
    public:
        // Constructor for testing.
        Audio(std::vector<T> & data, int sampleRate) {
            this->tSize = (int) sizeof(T);
            this->sampleRate = sampleRate;
            audioData = data;
        }

        // Parametrized constructor
        Audio(std::string file, int sampleRate) {
            this->tSize = (int) sizeof(T);
            this->sampleRate = sampleRate;
            std::cout << "No Pair" << std::endl;
            readFile(file);
        }

        ~Audio() = default;

        // Copy Constructor
        Audio(const Audio &rhs) {
            this->sampleRate = rhs.sampleRate;
            this->tSize = rhs.tSize;
            this->duration = rhs.duration;
            this->audioData = rhs.audioData;
        }

        // Move Constructor
        Audio(Audio &&rhs) {
            this->sampleRate = rhs.sampleRate;
            this->tSize = rhs.tSize;
            this->duration = rhs.duration;
            this->audioData = std::move(rhs.audioData);
        }

        // Copy Assignment Operator
        Audio operator=(const Audio<T, 1> &rhs) {
            Audio<T, 1> temp(rhs);
            *this = std::move(temp);
            return *this;
        }

        // Move Assignment Operator
        Audio operator=(Audio<T, 1> &&rhs) {
            this->sampleRate = rhs.sampleRate;
            this->duration = rhs.duration;
            this->tSize = rhs.tSize;
            this->audioData = std::move(rhs.audioData);
            return *this;
        }

        // Reads in the file specified. Handles mono 8bit or 16bit reading.
        void readFile(std::string file) {
            std::ifstream infile(file, std::ios::binary | std::ios::in);

            if (infile.is_open()) {
                infile.seekg (0, infile.end);
                long length = infile.tellg();
                infile.seekg (0, infile.beg);

                int numSamples = (int) (length / (tSize * numChannels));
                duration = numSamples / (float) sampleRate;

                audioData.resize((unsigned long) (numSamples));

                for (int i = 0; i < audioData.size(); ++i) {
                    char buffer[tSize];
                    infile.read(buffer, tSize);
                    audioData[i] = *(T *)buffer;
                }
            } else {
                std::cout << "Couldn't open file: " << file << std::endl;
            }
            infile.close();
        }

        // Saves the file with the name given as the base.
        void saveFile(std::string file) {
            std::string fileName = file + "_" + std::to_string(sampleRate) + "_"
                                           + std::to_string(tSize * 8) + "_mono.raw";
            std::ofstream outfile(fileName, std::ios::binary | std::ios::out);

            if (outfile.is_open()) {
                outfile.write((char *)&audioData[0], audioData.size() * tSize);
            } else {
                std::cout << "Couldn't save file: " << fileName << std::endl;
            }
            outfile.close();
        }

        // Overloads pipe operator to implement concatenation.
        Audio & operator|(const Audio & rhs) {
            audioData.insert(audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
            return *this;
        }

        // Overloads * operator to work on pairs for floats that contain
        // the values that the left and right channel of the audio should
        // be multiplied by.
        Audio & operator*(const std::pair<float, float> & volFactor) {
            for (int i = 0; i < audioData.size(); ++i) {
                audioData[i] *= volFactor.first;
            };
            return *this;
        }

        // Helper function that clamps the given sample to its maximum value.
        T clamp_sample(T sample) {
            if (tSize == 1) {
                return (sample > INT8_MAX) ? INT8_MAX : sample;
            } else {
                return (sample > INT16_MAX) ? INT16_MAX : sample;
            }
        }

        // Overloads + operator. Adds two equally sized audio files with the
        // same tSize and number of samples.
        Audio & operator+(const Audio & rhs) {
            for (int i = 0; i < audioData.size(); ++i) {
                audioData[i] = clamp_sample(audioData[i] + rhs.audioData[i]);
            }
            return *this;
        }

        // Overload ^ operator to be used for cutting a selection of samples
        // from the sound file.
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

        // Reverses the audioData vector, reversing the sound file.
        void reverse() {
            std::reverse(audioData.begin(), audioData.end());
        }

        // Returns the audioData vector.
        std::vector<T> & getAudioData() {
            return audioData;
        }

        // Extracts a range from the current audio file.
        // Returns a new audio file.
        Audio & extractRange(int r1, int r2) {
            Audio * newAudio = new Audio(*this);
            newAudio->audioData.clear();
            newAudio->audioData.resize((unsigned long) (r2 - r1));
            std::copy(audioData.begin() + r1, audioData.begin() + r2, newAudio->audioData.begin());
            return *newAudio;
        }

        // Returns the RMS of the audio file.
        float calculateRMS() {
            float inverseSize = 1 / (float)audioData.size();
            return std::sqrt(inverseSize * std::accumulate(audioData.begin(), audioData.end(), 0,
                                                           [] (T accum, T x) {return (accum + pow(x, 2));}));
        }

        // Normalizes the audio file to some given RMS value.
        Audio & normalize(float rms) {
            NormalFunctor func(calculateRMS(), rms);
            Audio * newAudio = new Audio(*this);
            std::transform(audioData.begin(), audioData.end(), newAudio->audioData.begin(), func);
            return *newAudio;
        }

        // Inner functor class used to calculate the normal values.
        class NormalFunctor {
        public:
            float currentRms;
            float desiredRms;
            NormalFunctor(float rmsCurrent, float rmsDesired): currentRms(rmsCurrent), desiredRms(rmsDesired) {}

            T operator()(T inputAmp) {
                return (T) (inputAmp * (desiredRms / currentRms));
            }
        };
    };


    template <typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T, T>> audioData;
        int tSize;
        int sampleRate;
        double duration;
    public:
        // Constructor for testing.
        Audio(std::vector<std::pair<T, T>> & data, int sampleRate) {
            this->tSize = (int) sizeof(T);
            this->sampleRate = sampleRate;
            audioData = data;
        }

        Audio(std::string file, int sampleRate) {
            this->sampleRate = sampleRate;
            this->tSize = (int) sizeof(T);
            readFile(file);
        }

        ~Audio() = default;

        // Copy Constructor
        Audio(const Audio &rhs) {
            this->sampleRate = rhs.sampleRate;
            this->tSize = rhs.tSize;
            this->duration = rhs.duration;
            this->audioData = rhs.audioData;
        }

        // Move Constructor
        Audio(Audio &&rhs) {
            this->sampleRate = rhs.sampleRate;
            this->tSize = rhs.tSize;
            this->duration = rhs.duration;
            this->audioData = std::move(rhs.audioData);
        }

        // Copy Assignment Operator
        Audio operator=(const Audio<T, 2> &rhs) {
            Audio<T, 2> temp(rhs);
            *this = std::move(temp);
            return *this;
        }

        // Move Assignment Operator
        Audio operator=(Audio<T, 2> &&rhs) {
            this->sampleRate = rhs.sampleRate;
            this->duration = rhs.duration;
            this->tSize = rhs.tSize;
            this->audioData = std::move(rhs.audioData);
            return *this;
        }

        void readFile(std::string file) {
            std::ifstream infile(file);

            if (infile.is_open()) {
                infile.seekg (0, infile.end);
                long length = infile.tellg();
                infile.seekg (0, infile.beg);

                int numSamples = (int) (length / (tSize * 2));
                std::cout << numSamples << std::endl;
                duration = numSamples / sampleRate;

                audioData.resize((unsigned long) (numSamples));
                for (int i = 0; i < audioData.size(); ++i) {
                    char buffer[tSize];
                    infile.read(buffer, tSize);
                    char buffer2[tSize];
                    infile.read(buffer2, tSize);
                    audioData[i] = std::make_pair(*(T *)buffer, *(T *)buffer2);
                }
            } else {
                std::cout << "Couldn't open file: " << file << std::endl;
            }
            infile.close();
        }

        void saveFile(std::string file) {
            std::string fileName = file + "_" + std::to_string(sampleRate) + "_"
                                   + std::to_string(tSize * 8) + "_stereo.raw";
            std::ofstream outfile(fileName);

            if (outfile.is_open()) {
                for (auto data : audioData) {
                    outfile.write((char *)&data.first, tSize);
                    outfile.write((char *)&data.second, tSize);
                }
            } else {
                std::cout << "Couldn't save file: " << fileName << std::endl;
            }
            outfile.close();
        }

        std::pair<float, float> calculateRMS() {
            std::pair<float, float> rms;
            float inverseSize = 1 / (float) audioData.size();

            rms = std::accumulate(audioData.begin(), audioData.end(), std::pair<float, float>(0, 0),
                            [] (std::pair<float, float> accum, std::pair<T, T> x) {
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

        T clamp_sample(T sample) {
            if (tSize == 1) {
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

        void reverse() {
            std::reverse(audioData.begin(), audioData.end());
        }

        Audio & extractRange(int r1, int r2) {
            Audio<T, 2> * newAudio = new Audio(*this);
            newAudio->audioData.clear();
            newAudio->audioData.resize((unsigned long) (r2 - r1));
            std::copy(audioData.begin() + r1, audioData.begin() + r2, newAudio->audioData.begin());
            return *newAudio;
        }

        Audio & normalize(float rmsL, float rmsR) {
            NormalFunctor func(calculateRMS(), std::make_pair(rmsL, rmsR));
            Audio<T, 2> * newAudio = new Audio(*this);
            std::transform(audioData.begin(), audioData.end(), newAudio->audioData.begin(), func);
            return *newAudio;
        }

        std::vector<std::pair<T, T>> & getAudioData() {
            return audioData;
        }

        class NormalFunctor {
        public:
            std::pair<float, float> currentRms;
            std::pair<float, float> desiredRms;
            NormalFunctor(std::pair<float, float> rmsCurrent, std::pair<float, float> rmsDesired):
                    currentRms(rmsCurrent), desiredRms(rmsDesired) {}

            std::pair<T, T> operator()(std::pair<T, T> inputAmp) {
                return std::make_pair((T) (inputAmp.first * (desiredRms.first / currentRms.first)),
                                      (T) (inputAmp.second * (desiredRms.second / currentRms.second)));
            }
        };
    };
}

#endif //AUDIO_H
