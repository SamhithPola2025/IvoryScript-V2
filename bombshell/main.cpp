// entry-point file
#include "tokenizer.hpp"
namespace fs = std::filesystem;

// helper function for enum to string, just for my visualization
std::string tokenTypeToString(tokenType tType) {
    switch (tType) {
        case tokenType::_return:
            return "return";
        case tokenType::integer_lit:
            return "integer_lit";
        case tokenType::semicolon:
            return "semicolon";
        case tokenType::list_type:
            return "list_type";
        default:
            return "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Type: " << tokenTypeToString(token.type);
    if (token.content) {
        os << ", Content: " << *token.content;
    }
    return os;
}

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
//    std::cout << content;

    std::vector<Token> tokens = tokenize(content);
    for (const Token& token : tokens) {
        std::cout << token << "\n";
    }
    return 0;
}