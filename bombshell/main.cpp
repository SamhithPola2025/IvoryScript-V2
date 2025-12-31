// entry-point file

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
namespace fs = std::filesystem;

// tokens:


int main(int argc, char* argv[]) {
    // ignore the fact that this is the way you would do this in older C++ versions, I find this way cleaner
    // does the same thing either way

    if (argc != 2) {
        // error for wrong arguments
        std::cerr << "Incorrect usage, correct usage:" << std::endl;
        std::cerr << "bshell <input.bsh>" <<  std::endl;
        return 1;
    }

    // next, we need to read the file as a string
    std::ifstream input(argv[1], std::ios::in);

    if (!input) {
        if (!fs::exists(argv[1])) {
            std::cerr << "File " << argv[1] << " doesn't exist or is corrupted" << std::endl;
        } else {
            std::cerr << "Failed to open/read file: " << argv[1] << std::endl;
        }
        return 1;
    }

    std::stringstream buffer; // buffer, making space for it in memory
    buffer << input.rdbuf(); // reading the file to the buffer

    std::string content = buffer.str(); // getting the string from the stream
    std::cout << content;

    return 0;
}