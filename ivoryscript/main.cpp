// main.cpp
#include "modules/tokenizer.hpp"
#include "modules/parser.hpp"
#include <iterator>
namespace fs = std::filesystem;

std::ostream& operator<<(std::ostream& os, const Token& token) {
    helpers helper;
    os << "Type: " << helper.tokenTypeToString(token.type);
    if (token.content) {
        os << ", Content: " << *token.content;
    }
    return os;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {

    } else {
        std::cerr << "Incorrect usage, correct usage:" << std::endl;
        std::cerr << "ivscript <input.ivc>" <<  std::endl;
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);

    if (!input) {
        if (!fs::exists(argv[1])) {
            std::cerr << "File " << argv[1] << " doesn't exist or is corrupted" << std::endl;
        } else {
            std::cerr << "Failed to open/read file: " << argv[1] << std::endl;
        }
        return 1;
    }

    std::string content{
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>()
    };

    // std::cout << content << std::endl;

    //tokenizing

    std::vector<Token> tokens = tokenize(content);

    for (const Token& token : tokens) {
        std::cout << token << "\n";
    }


    //parsing

    Parser p(tokens);
    std::unique_ptr<Program> program = p.parseProgram();

    std::cout << "placeholder btw" << std::endl;
    std::cout << "idfk" << std::endl;

    return 0;
}