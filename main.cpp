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

    return 0;
}
