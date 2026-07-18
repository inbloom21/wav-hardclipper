# WAV Hardclipper

A C++ command-line program built to parse, manipulate, and rewrite audio files. Currently, the program applies a hard-clipping distortion to 16-bit PCM WAV files.

## How to Use

Make sure you have a C++ compiler and CMake installed on your computer.

### 1. Clone this repository
```bash
git clone https://github.com/inbloom21/wav-hardclipper.git
cd wav-hardclipper
```

### 2. Generate the build files and compile
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 3. Run the Program
Put the 16-bit `.wav` file named `input.wav` in the repository root and run:
*  For Windows (Visual Studio)
```bash
.\Release\WavHardClipper.exe
```
* For Mac/Linux/VS Code
```bash
./WavHardClipper.exe
```
_This code uses relative paths to locate the WAV file. Make sure you run the executable while your terminal is currently inside the `build` directory_

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
