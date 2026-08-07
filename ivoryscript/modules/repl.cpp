#include "parser.hpp"
#include "tokenizer.hpp"
#include <ctime>

class replEval {
public:
    std::vector<Token> input;
    replEval(std::vector<Token> tokens) {
        this->input = tokens;
    }

    void replLoop() {
        time_t p_currDate = time(NULL);
        char *currDate = ctime(&p_currDate);

        std::cout << "IvoryScript 1.0.0. (\" " << *currDate << "\") REPL (read-eval-print-loop) mode." << "\n";
        std::cout << "Type \"docs()\" for more information." << std::endl;

        int i = input.size();
        Parser currParser(input);
        Program currProgram;

        while (i--) {
            currProgram = currParser.parseProgam();
        }
    }
};