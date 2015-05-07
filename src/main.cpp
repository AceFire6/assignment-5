#include <iostream>
#include <cstdint>
#include "Audio.h"

using namespace std;

int main(int argc,  const char* argv[]) {
    if (argc < 7) {
        cout << "Too few arguments." << endl;
        return 0;
    }

    try {
        int sampleRate = stoi(argv[2]);
        int bitCount = stoi(argv[4]);
        if (bitCount != 16 && bitCount != 8) {
            cout << "Unsupported bit count." << endl;
            return 0;
        }

        typedef int8_t sampleRateType;
        if (bitCount == 16) {
            typedef int16_t sampleRateType;
        }

        int numChannels = stoi(argv[6]);
        if (numChannels != 1 && numChannels != 2) {
            cout << "Unsupported channel count." << endl;
            return 0;
        }

        int mod = 0;
        string outfileName = "out";
        if (argv[7] == string("-o")) {
            outfileName = argv[8];
            mod = 2;
        }

        string op = argv[7 + mod];
//    cout << sampleRate << " " << bitCount << " " << numChannels << " " << outfileName << " " << mod << " " << op << endl;
        string infile = "";
        if (op == "-add") {
            infile = argv[8 + mod];
            string infile2 = argv[9 + mod];
        } else if (op == "-cut") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            infile = argv[10 + mod];
        } else if (op == "-radd") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            int s1 = stoi(argv[10 + mod]);
            int s2 = stoi(argv[11 + mod]);
            infile = argv[12 + mod];
            string infile2 = argv[13 + mod];
        } else if (op == "-cat") {
            infile = argv[8 + mod];
            string infile2 = argv[9 + mod];
            if (numChannels == 1) {
                MLLJET001::Audio<sampleRateType, 1> audio1(infile, sampleRate);
                MLLJET001::Audio<sampleRateType, 1> audio2(infile2, sampleRate);
                (audio1 | audio2).saveFile(outfileName);
            } else {
                MLLJET001::Audio<sampleRateType, 2> audio1(infile, sampleRate);
                MLLJET001::Audio<sampleRateType, 2> audio2(infile2, sampleRate);
                (audio1 | audio2).saveFile(outfileName);
            }
        } else if (op == "-v") {
            float r1 = stof(argv[8 + mod]);
            float r2 = stof(argv[9 + mod]);
            infile = argv[10 + mod];
            if (numChannels == 1) {
                MLLJET001::Audio<sampleRateType, 1> audio1(infile, sampleRate);
                (audio1 * std::make_pair(r1, r2));
                audio1.saveFile(outfileName);
            } else {
                MLLJET001::Audio<sampleRateType, 2> audio1(infile, sampleRate);
                (audio1 * std::make_pair(r1, r2));
                audio1.saveFile(outfileName);
            }
        } else if (op == "-rev") {
            infile = argv[8 + mod];
        } else if (op == "-rms") {
            infile = argv[8 + mod];
            if (numChannels == 1) {
                MLLJET001::Audio<sampleRateType, 1> audio1(infile, sampleRate);
                cout << audio1.calculateRMS() << endl;
            } else {
                MLLJET001::Audio<sampleRateType, 2> audio1(infile, sampleRate);
                std::pair<float, float> rms = audio1.calculateRMS();
                cout << rms.first << " " << rms.second << endl;
            }
        } else if (op == "-norm") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            infile = argv[10 + mod];
        } else if (op == "-fadein") {
            float s = stof(argv[8 + mod]);
            infile = argv[9 + mod];
        } else if (op == "-fadeout") {
            float s = stof(argv[8 + mod]);
            infile = argv[9 + mod];
        }
    } catch (exception& e) {
        cout << "Malformed argument list." << endl;
    }

    return 0;
}
