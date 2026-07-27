#pragma once
#include <string>
#include <vector>
#include <cstdint>

class FileHandler{
private:
    uint16_t numChannels {0};
    uint32_t sampleRate {0};
    uint16_t bitsPerSample {0};
    std::vector<int16_t> audioData {0};
public:
    bool load(std::string& inputPath);
    bool save(std::string& outputPath);
    std::vector<int16_t>& getAudioData();
};
