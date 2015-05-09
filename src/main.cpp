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
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio1(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 1> audio2(infile2, sampleRate);
                    (audio1 + audio2).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio1(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 1> audio2(infile2, sampleRate);
                    (audio1 + audio2).saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio1(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 2> audio2(infile2, sampleRate);
                    (audio1 + audio2).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio1(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 2> audio2(infile2, sampleRate);
                    (audio1 + audio2).saveFile(outfileName);
                }
            }
        } else if (op == "-cut") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            std::pair<int, int> excludeTime = std::make_pair(r1, r2);
            infile = argv[10 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                    (audio ^ excludeTime).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                    (audio ^ excludeTime).saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                    (audio ^ excludeTime).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                    (audio ^ excludeTime).saveFile(outfileName);
                }
            }
        } else if (op == "-radd") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            int s1 = stoi(argv[10 + mod]);
            int s2 = stoi(argv[11 + mod]);
            infile = argv[12 + mod];
            string infile2 = argv[13 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 1> audio2(infile2, sampleRate);
                    (audio.extractRange(r1, r2) + audio2.extractRange(s1, s2)).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 1> audio2(infile2, sampleRate);
                    (audio.extractRange(r1, r2) + audio2.extractRange(s1, s2)).saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 2> audio2(infile2, sampleRate);
                    (audio.extractRange(r1, r2) + audio2.extractRange(s1, s2)).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 2> audio2(infile2, sampleRate);
                    (audio.extractRange(r1, r2) + audio2.extractRange(s1, s2)).saveFile(outfileName);
                }
            }
        } else if (op == "-cat") {
            infile = argv[8 + mod];
            string infile2 = argv[9 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio1(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 1> audio2(infile2, sampleRate);
                    (audio1 | audio2).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio1(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 1> audio2(infile2, sampleRate);
                    (audio1 | audio2).saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio1(infile, sampleRate);
                    MLLJET001::Audio<int16_t, 2> audio2(infile2, sampleRate);
                    (audio1 | audio2).saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio1(infile, sampleRate);
                    MLLJET001::Audio<int8_t, 2> audio2(infile2, sampleRate);
                    (audio1 | audio2).saveFile(outfileName);
                }
            }
        } else if (op == "-v") {
            float r1 = stof(argv[8 + mod]);
            float r2 = stof(argv[9 + mod]);
            infile = argv[10 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio1(infile, sampleRate);
                    (audio1 * std::make_pair(r1, r2));
                    audio1.saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio1(infile, sampleRate);
                    (audio1 * std::make_pair(r1, r2));
                    audio1.saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio1(infile, sampleRate);
                    (audio1 * std::make_pair(r1, r2));
                    audio1.saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio1(infile, sampleRate);
                    (audio1 * std::make_pair(r1, r2));
                    audio1.saveFile(outfileName);
                }
            }
        } else if (op == "-rev") {
            infile = argv[8 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                    audio.reverse();
                    audio.saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                    audio.reverse();
                    audio.saveFile(outfileName);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                    audio.reverse();
                    audio.saveFile(outfileName);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                    audio.reverse();
                    audio.saveFile(outfileName);
                }
            }
        } else if (op == "-rms") {
            infile = argv[8 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio1(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio1(infile, sampleRate);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio1(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio1(infile, sampleRate);
                }
            }
        } else if (op == "-norm") {
            int r1 = stoi(argv[8 + mod]);
            int r2 = stoi(argv[9 + mod]);
            infile = argv[10 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                }
            }
        } else if (op == "-fadein") {
            float s = stof(argv[8 + mod]);
            infile = argv[9 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                }
            }
        } else if (op == "-fadeout") {
            float s = stof(argv[8 + mod]);
            infile = argv[9 + mod];
            if (numChannels == 1) {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 1> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 1> audio(infile, sampleRate);
                }
            } else {
                if (bitCount == 16) {
                    MLLJET001::Audio<int16_t, 2> audio(infile, sampleRate);
                } else {
                    MLLJET001::Audio<int8_t, 2> audio(infile, sampleRate);
                }
            }
        }
    } catch (exception& e) {
        cout << "Malformed argument list." << endl;
    }

    return 0;
}
