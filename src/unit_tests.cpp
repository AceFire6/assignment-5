#define CATCH_CONFIG_MAIN

#include "Audio.h"

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

#include "catch.hpp"


using namespace std;

int sampleRate = 44100;

int8_t a = 5;
int16_t b = 5;

vector<int8_t> monoData8 = {a, a, a, a, a};
vector<int16_t> monoData16 = {b, b, b, b, b};

MLLJET001::Audio<int8_t, 1> mono8a(monoData8, sampleRate);
MLLJET001::Audio<int8_t, 1> mono8b(monoData8, sampleRate);

MLLJET001::Audio<int16_t, 1> mono16a(monoData16, sampleRate);
MLLJET001::Audio<int16_t, 1> mono16b(monoData16, sampleRate);

pair<int8_t, int8_t> pairA = make_pair(a, a);
pair<int16_t, int16_t> pairB = make_pair(b, b);

vector<pair<int8_t, int8_t>> stereoData8 = {pairA, pairA, pairA, pairA, pairA};
vector<pair<int16_t, int16_t>> stereoData16 = {pairB, pairB, pairB, pairB, pairB};

MLLJET001::Audio<int8_t, 2> stereo8a(stereoData8, sampleRate);
MLLJET001::Audio<int8_t, 2> stereo8b(stereoData8, sampleRate);

MLLJET001::Audio<int16_t, 2> stereo16a(stereoData16, sampleRate);
MLLJET001::Audio<int16_t, 2> stereo16b(stereoData16, sampleRate);


MLLJET001::Audio<int8_t, 1> mono8test = mono8a;
MLLJET001::Audio<int16_t, 1> mono16test = mono16a;
MLLJET001::Audio<int8_t, 2> stereo8test = stereo8a;
MLLJET001::Audio<int16_t, 2> stereo16test = stereo16a;
TEST_CASE("Test Copy Assignment Operator - Mono - 8bit", "[mono, 8bit, operator]") {
    REQUIRE(mono8test.getAudioData().size() == mono8a.getAudioData().size());
    REQUIRE(mono8test.getAudioData()[0] == mono8a.getAudioData()[0]);
}

TEST_CASE("Test Copy Assignment Operator - Mono - 16bit", "[mono, 16bit, operator]") {
    REQUIRE(mono16test.getAudioData().size() == mono16a.getAudioData().size());
    REQUIRE(mono16test.getAudioData()[0] == mono16a.getAudioData()[0]);
}

TEST_CASE("Test Copy Assignment Operator - Stereo - 8bit", "[stereo, 8bit, operator]") {
    REQUIRE(stereo8test.getAudioData().size() == stereo8a.getAudioData().size());
    REQUIRE(stereo8test.getAudioData()[0] == stereo8a.getAudioData()[0]);
}

TEST_CASE("Test Copy Assignment Operator - Stereo - 16bit", "[stereo, 16bit, operator]") {
    REQUIRE(stereo16test.getAudioData().size() == stereo16a.getAudioData().size());
    REQUIRE(stereo16test.getAudioData()[0] == stereo16a.getAudioData()[0]);
}

TEST_CASE("Test Pipe Operator - Mono - 8bit", "[mono, 8bit, operator]") {
    vector<int8_t> temp = {a, a, a, a, a, a, a, a, a, a};
    mono8test = mono8a | mono8b;

    REQUIRE(temp.size() == mono8test.getAudioData().size());

    bool equal = true;
    for (int i = 0; i < temp.size(); ++i) {
        if (mono8test.getAudioData()[i] != temp[i]) {
            equal = false;
        }
    }
    REQUIRE(equal);
}

TEST_CASE("Test Pipe Operator - Mono - 16bit", "[mono, 16bit, operator]") {
    vector<int16_t> temp = {a, a, a, a, a, a, a, a, a, a};
    mono16test = mono16a | mono16b;

    REQUIRE(temp.size() == mono16test.getAudioData().size());

    bool equal = true;
    for (int i = 0; i < temp.size(); ++i) {
        if (mono16test.getAudioData()[i] != temp[i]) {
            equal = false;
        }
    }
    REQUIRE(equal);
}

TEST_CASE("Test Pipe Operator - Stereo - 8bit", "[stereo, 8bit, operator]") {
    vector<pair<int8_t, int8_t>> temp = {pairA, pairA, pairA, pairA, pairA, pairA, pairA, pairA, pairA, pairA};
    stereo8test = stereo8a | stereo8b;

    REQUIRE(temp.size() == stereo8test.getAudioData().size());

    bool equal = true;
    for (int i = 0; i < temp.size(); ++i) {
        if (stereo8test.getAudioData()[i] != temp[i]) {
            equal = false;
        }
    }
    REQUIRE(equal);
}

TEST_CASE("Test Pipe Operator - Stereo - 16bit", "[stereo, 16bit, operator]") {
    vector<pair<int16_t, int16_t>> temp = {pairB, pairB, pairB, pairB, pairB, pairB, pairB, pairB, pairB, pairB};
    stereo16test = stereo16a | stereo16b;

    REQUIRE(temp.size() == stereo16test.getAudioData().size());

    bool equal = true;
    for (int i = 0; i < temp.size(); ++i) {
        if (stereo16test.getAudioData()[i] != temp[i]) {
            equal = false;
        }
    }
    REQUIRE(equal);
}

TEST_CASE("Test * Operator - Mono - 8bit", "[mono, 8bit, operator]") {
    mono8a = mono8a * pair<float, float>(0.2, 1);
    REQUIRE(mono8a.getAudioData()[0] == 1);
    mono8a = mono8a * pair<float, float>(5, 1);
    REQUIRE(mono8a.getAudioData()[0] == 5);
}

TEST_CASE("Test * Operator - Mono - 16bit", "[mono, 16bit, operator]") {
    mono16a = mono16a * pair<float, float>(0.2, 1);
    REQUIRE(mono16a.getAudioData()[0] == 1);
    mono16a = mono16a * pair<float, float>(5, 1);
    REQUIRE(mono16a.getAudioData()[0] == 5);
}

TEST_CASE("Test * Operator - Stereo - 8bit", "[stereo, 8bit, operator]") {
    stereo8a = stereo8a * pair<float, float>(0.2, 2);
    REQUIRE(stereo8a.getAudioData()[0].first == 1);
    REQUIRE(stereo8a.getAudioData()[0].second == 10);
    stereo8a = stereo8a * pair<float, float>(5, 0.5);
    REQUIRE(stereo8a.getAudioData()[0].first == 5);
    REQUIRE(stereo8a.getAudioData()[0].second == 5);
}

TEST_CASE("Test * Operator - Stereo - 16bit", "[stereo, 16bit, operator]") {
    stereo16a = stereo16a * pair<float, float>(0.2, 2);
    REQUIRE(stereo16a.getAudioData()[0].first == 1);
    REQUIRE(stereo16a.getAudioData()[0].second == 10);
    stereo16a = stereo16a * pair<float, float>(5, 0.5);
    REQUIRE(stereo16a.getAudioData()[0].first == 5);
    REQUIRE(stereo16a.getAudioData()[0].second == 5);
}

TEST_CASE("Test + Operator - Mono - 8bit", "[mono, 8bit, operator]") {
    mono8a + mono8b;
    REQUIRE(mono8a.getAudioData()[0] == 10);
    mono8a = mono8a * pair<float, float>(0.5, 1);
}

TEST_CASE("Test + Operator - Mono - 16bit", "[mono, 16bit, operator]") {
    mono16a + mono16b;
    REQUIRE(mono16a.getAudioData()[0] == 10);
    mono16a = mono16a * pair<float, float>(0.5, 1);
}

TEST_CASE("Test + Operator - Stereo - 8bit", "[stereo, 8bit, operator]") {
    stereo8a + stereo8b;
    REQUIRE(stereo8a.getAudioData()[0] == 10);
    stereo8a = stereo8a * pair<float, float>(0.5, 1);
}

TEST_CASE("Test + Operator - Stereo - 16bit", "[stereo, 16bit, operator]") {
    stereo16a + stereo16b;
    REQUIRE(stereo16a.getAudioData()[0] == 10);
    stereo16a = stereo16a * pair<float, float>(0.5, 1);
}

TEST_CASE("Test ^ Operator - Mono - 8bit", "[mono, 8bit, operator]") {
    mono8test = mono8test ^ pair<int, int>(0, 1);
    REQUIRE(mono8test.getAudioData().size() == 3);
}

TEST_CASE("Test ^ Operator - Mono - 16bit", "[mono, 16bit, operator]") {
    mono16test = mono16test ^ pair<int, int>(0, 1);
    REQUIRE(mono16test.getAudioData().size() == 3);
}

TEST_CASE("Test ^ Operator - Stereo - 8bit", "[stereo, 8bit, operator]") {
    stereo8test = stereo8test ^ pair<int, int>(0, 1);
    REQUIRE(stereo8test.getAudioData().size() == 3);
}

TEST_CASE("Test ^ Operator - Stereo - 16bit", "[stereo, 16bit, operator]") {
    stereo16test = stereo16test ^ pair<int, int>(0, 1);
    REQUIRE(stereo16test.getAudioData().size() == 3);
}

TEST_CASE("Test Copy Constructor - Mono - 8bit", "[mono, 8bit, constructor]") {
    mono8test = MLLJET001::Audio<int8_t, 1>(mono8a);
    REQUIRE(mono8a.getAudioData()[0] == mono8test.getAudioData()[0]);
    REQUIRE(mono8a.getAudioData().size() == mono8test.getAudioData().size());
}

TEST_CASE("Test Copy Constructor - Mono - 16bit", "[mono, 16bit, constructor]") {
    mono16test = MLLJET001::Audio<int16_t, 1>(mono16a);
    REQUIRE(mono16a.getAudioData()[0] == mono16test.getAudioData()[0]);
    REQUIRE(mono16a.getAudioData().size() == mono16test.getAudioData().size());
}

TEST_CASE("Test Copy Constructor - Stereo - 8bit", "[stereo, 8bit, constructor]") {
    stereo8test = MLLJET001::Audio<int8_t, 2>(stereo8a);
    REQUIRE(stereo8a.getAudioData()[0] == stereo8test.getAudioData()[0]);
    REQUIRE(stereo8a.getAudioData().size() == stereo8test.getAudioData().size());
}

TEST_CASE("Test Copy Constructor - Stereo - 16bit", "[stereo, 16bit, constructor]") {
    stereo16test = MLLJET001::Audio<int16_t, 2>(stereo16a);
    REQUIRE(stereo16a.getAudioData()[0] == stereo16test.getAudioData()[0]);
    REQUIRE(stereo16a.getAudioData().size() == stereo16test.getAudioData().size());
}

TEST_CASE("Test Reverse Function - Mono - 8bit", "[mono, 8bit, function]") {

}
