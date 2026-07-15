// main.cpp
#include "modules/tokenizer.hpp"
#include "modules/ast.hpp"
#include <iterator>
namespace fs = std::filesystem;

// enum -> string helper
std::string tokenTypeToString(tokenType tType) {
    switch (tType) {
        case tokenType::_return:
            return "return";
        case tokenType::integer_lit:
            return "integer_lit";
        case tokenType::semicolon:
            return "semicolon";
        case tokenType::string_type:
            return "string_type";
        case tokenType::list_type:
            return "list_type";
        case tokenType::string_lit:
            return "string_lit";
        case tokenType::identifier:
            return "var_name";
        case tokenType::equal:
            return "equals";
        case tokenType::int_type:
            return "int";
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
    if (argc != 2) {
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

    
    return 0;
}