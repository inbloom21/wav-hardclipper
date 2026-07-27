#pragma once
#include <cstdint>
#include <vector>

class AudioProcessor{
public:
    virtual ~AudioProcessor() = default;
    virtual void process(std::vector<int16_t>& audioData) = 0;
};

class HardClipper : public AudioProcessor{
private:
    float gain {4.0f};
    int16_t threshold {15000};
public:
    void process(std::vector<int16_t>& audioData) override;
};
