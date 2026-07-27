#include "AudioProcessor.h"

void HardClipper::process(std::vector<int16_t>& audioData){
    for (size_t i = 0; i < audioData.size(); i++){
        float amplified {audioData[i] * gain};
        if (amplified > threshold){
            audioData[i] = threshold;
        } else if (amplified < -threshold){
            audioData[i] = -threshold;
        } else{
            audioData[i] = static_cast<int16_t>(amplified);
        } 
    }
}
