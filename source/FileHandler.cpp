#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <cstring>

bool FileHandler::load(std::string& inputPath){
    std::ifstream inputFile(inputPath, std::ios::binary);
    if (!inputFile.is_open()){
        std::cerr << "Could not open file!\n";
        return false;
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
            size_t samples {chunkSize / sizeof(int16_t)};
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
        return false;
    }
    std::cout << "Found " << audioData.size() << " samples at " << sampleRate << "Hz!\n";
    return true;
}

bool FileHandler::save(std::string& outputPath){
    std::ofstream outputFile(outputPath, std::ios::binary);

    uint32_t newDataSize {static_cast<uint32_t>(audioData.size() * sizeof(int16_t))};
    uint32_t newRiffSize {36 + newDataSize};

    // new RIFF
    outputFile.write("RIFF", 4);
    outputFile.write(reinterpret_cast<char*>(&newRiffSize), 4);
    outputFile.write("WAVE", 4);

    // new fmt
    outputFile.write("fmt ", 4);
    uint32_t fmtSize {16};
    outputFile.write(reinterpret_cast<const char*>(&fmtSize), 4);

    uint16_t audioFormat {1};
    outputFile.write(reinterpret_cast<const char*>(&audioFormat), 2);

    outputFile.write(reinterpret_cast<const char*>(&numChannels), 2);
    outputFile.write(reinterpret_cast<const char*>(&sampleRate), 4);

    uint32_t byteRate {sampleRate * numChannels * 2};
    outputFile.write(reinterpret_cast<const char*>(&byteRate), 4);

    uint16_t blockAlign {static_cast<uint16_t>(numChannels * 2)};
    outputFile.write(reinterpret_cast<const char*>(&blockAlign), 2);
    outputFile.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // new data
    outputFile.write("data", 4);
    outputFile.write(reinterpret_cast<const char*>(&newDataSize), 4);
    outputFile.write(reinterpret_cast<const char*>(audioData.data()), newDataSize);
    
    outputFile.close();
    std::cout << "New WAV file is saved!\n";

    return true;
}

std::vector<int16_t>& FileHandler::getAudioData(){
    return audioData;
}
