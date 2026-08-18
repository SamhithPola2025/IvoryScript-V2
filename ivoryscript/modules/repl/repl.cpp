// repl.cpp

#include "repl.hpp"
#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

void spacePrune(char *currDate, std::string currDateStr) {
    for (int i = 0; currDate[i] != '\0'; ++i) {
        bool hasSpace = false;
        if (currDate[i] == ' ') {
            hasSpace = true;
            if (currDate[i + 1] == ' ' && hasSpace == true) {
                currDateStr.erase(i, 1);
                hasSpace = false;
                // i think ts works
                continue;
            }
        }
    }
}

int replEval::replLoop() {
    time_t p_currDate = time(NULL);
    char *currDate = ctime(&p_currDate);

    currDate[strcspn(currDate, "\n")] = 0;
    std::string currDateStr(currDate);

    spacePrune(currDate, currDateStr);

    std::cout << "IvoryScript 1.0.0. (" << currDateStr
              << ") REPL (read-eval-print-loop) mode." << "\n";
    std::cout << "Type \"docs()\" for more information. "
              << "Type \"exit()\" to exit the REPL." << std::endl
              << "\n";

    std::string content;
    std::vector<std::string> lines;

        while (std::getline(std::cin, content)) {
            if (content.
                empty()) {
                break;
            }

            lines.push_back(content);
        }

        content.clear();
        for (int i = 0; i < lines.size(); ++i) {

            if (i < lines.size() - 1) {
                lines[i].append("\n");
            }

            content.append(lines[i]);
        
        }

        std::vector<Token> tokens = 
        tokenize(content);

        Parser currParser(tokens);
        std::unique_ptr<Program> currProgram;

        currProgram = currParser.parseProgram();

    std::cout << "this is just a test place for another breakpoint ig " << "\n";

    return 0;
}
