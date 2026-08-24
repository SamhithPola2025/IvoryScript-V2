// main.cpp
#include "modules/helpers/helpers.hpp"
#include "modules/parser/parser.hpp"
#include "modules/repl/repl.hpp"
#include "modules/tokenizer/tokenizer.hpp"
#include "modules/bytecodevm/traverser.hpp"

#include <iterator>

namespace fs = std::filesystem;

std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "Type: " << tokenTypeToString(token.type);
    if (token.content) {
        os << ", Content: " << *token.content;
    }
    return os;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        replEval REPL;
        REPL.replLoop();

        return 0;
    } else if (argc != 2) {
        std::cerr << "Incorrect usage, correct usage:" << std::endl;
        std::cerr << "ivscript <input.ivc>" << std::endl;

        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);

    if (!input) {
        if (!fs::exists(argv[1])) {
            std::cerr << "File " << argv[1] << " doesn't exist or is corrupted"
                      << std::endl;
        } else {
            std::cerr << "Failed to open/read file: " << argv[1] << std::endl;
        }
        return 1;
    }

    std::string content{std::istreambuf_iterator<char>(input),
                        std::istreambuf_iterator<char>()};

    // tokenizing

    std::vector<Token> tokens = tokenize(content);

    // parsing

    Parser p(tokens);
    std::unique_ptr<Program> program = p.parseProgram();

    // printing AST (intermediate step)

    Traverser traverser(*program);
    traverser.printAst();

    return 0;
}
