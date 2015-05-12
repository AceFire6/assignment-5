#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <iterator>

#include "Audio.h"

using namespace std;

int main() {
    int16_t a = 5;

    vector<int16_t> data;
    data.resize(5);

    for (int i = 0; i < data.size(); ++i) {
        data[i] = a - i;
        cout << data[i] << endl;
    }

    MLLJET001::Audio<int16_t, 1> z(data, 44100);
    MLLJET001::Audio<int16_t, 1> l(data, 44100);// = z.extractRange(0, 3);
    l.saveFile("a");

    return 0;
};
