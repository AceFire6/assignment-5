#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

namespace MLLJET001 {
    template <typename T, int numChannels> class Audio {
    public:
        Audio() {}
    };


    template <typename T> class Audio<T, 1> {
    private:
        std::vector<T> audioData;
    public:
        Audio() {
            std::cout << "No Pair" << std::endl;
        }
    };


    template <typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T, T>> audioData;
    public:
        Audio() {
            std::cout << "Pair" << std::endl;
        }
    };
}

#endif //AUDIO_H
