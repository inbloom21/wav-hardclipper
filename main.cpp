#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

int main(){
    std::string inputPath {"../input.wav"};
    std::string outputPath {"../output.wav"};

    // open wav file
    std::ifstream inputFile(inputPath, std::ios::binary);
    if (!inputFile.is_open()){
        std::cerr << "Could not open file!\n";
        return 1;
    }
    
    // RIFF chunk extraction
    char chunkId[4] {0};
    inputFile.read(chunkId, 4);

    uint32_t chunkSize;
    inputFile.read(reinterpret_cast<char*>(&chunkSize), 4);

    char format[4] {0};
    inputFile.read(format, 4);

    // fmt and data extraction
    uint16_t audioFormat {0};
    uint16_t numChannels {0};
    uint32_t sampleRate {0};
    uint16_t bitsPerSample {0};

    std::vector<int16_t> audioData;
    bool foundData {false};

    while (inputFile.read(chunkId, 4) && inputFile.read(reinterpret_cast<char*>(&chunkSize), 4)){
        if (std::strncmp(chunkId, "fmt ", 4) == 0){
            inputFile.read(reinterpret_cast<char*>(&audioFormat), 2);
            if (audioFormat != 1){
                std::cerr << "The WAV file isn't in PCM format!\n";
            }
            inputFile.read(reinterpret_cast<char*>(&numChannels), 2);
            inputFile.read(reinterpret_cast<char*>(&sampleRate), 4);
            inputFile.ignore(6);
            inputFile.read(reinterpret_cast<char*>(&bitsPerSample), 2);

            if (chunkSize > 16){
                inputFile.ignore(chunkSize - 16);
            }
        } else if (std::strncmp(chunkId, "data", 4) == 0){
            int samples {chunkSize / sizeof(int16_t)};
            audioData.resize(samples);
            inputFile.read(reinterpret_cast<char*>(audioData.data()), chunkSize);
            foundData = true;
            break;
        } else{
            inputFile.ignore(chunkSize);
        }  
    }
    inputFile.close();

    if (!foundData || bitsPerSample != 16){
        std::cerr << "Can't find the data!\n";
        return 1;
    }
    std::cout << "Found " << audioData.size() << " samples at " << sampleRate << "Hz!\n";

    // applying hardclipper
    float gain {4.0f};
    int16_t threshold {15000};

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

    // write the new file
    std::ofstream outputFile(outputPath, std::ios::binary);

    uint32_t newDataSize {audioData.size() * sizeof(int16_t)};
    uint32_t newRiffSize {36 + newDataSize};

    // new RIFF
    outputFile.write("RIFF", 4);
    outputFile.write(reinterpret_cast<char*>(&newRiffSize), 4);
    outputFile.write("WAVE", 4);

    // new fmt
    outputFile.write("fmt ", 4);
    uint32_t fmtSize {16};
    outputFile.write(reinterpret_cast<const char*>(&fmtSize), 4);
    outputFile.write(reinterpret_cast<const char*>(&audioFormat), 2);
    outputFile.write(reinterpret_cast<const char*>(&numChannels), 2);
    outputFile.write(reinterpret_cast<const char*>(&sampleRate), 4);

    uint32_t byteRate {sampleRate * numChannels * 2};
    outputFile.write(reinterpret_cast<const char*>(&byteRate), 4);

    uint16_t blockAlign {numChannels * 2};
    outputFile.write(reinterpret_cast<const char*>(&blockAlign), 2);
    outputFile.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // new data
    outputFile.write("data", 4);
    outputFile.write(reinterpret_cast<const char*>(&newDataSize), 4);
    outputFile.write(reinterpret_cast<const char*>(audioData.data()), newDataSize);
    
    outputFile.close();
    std::cout << "New WAV file is saved!\n";

    return 0;
}
