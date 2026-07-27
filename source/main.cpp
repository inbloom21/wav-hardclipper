#include <iostream>
#include <string>
#include "FileHandler.h"
#include "AudioProcessor.h"

int main(int argc, char* argv[]){
    if (argc != 3){
        std::cerr << "Please input 3 arguments! (<path-to-executable> <path-to-input-file.wav> <path-to-output-file.wav>)\n";
        return 1;
    }

    std::string inputPath {argv[1]};
    std::string outputPath {argv[2]};
    
    FileHandler wavAudio;
    if (!wavAudio.load(inputPath)){
        return 1;
    }

    HardClipper hardClipperEffect;
    hardClipperEffect.process(wavAudio.getAudioData());

    wavAudio.save(outputPath);
    return 0;
}
