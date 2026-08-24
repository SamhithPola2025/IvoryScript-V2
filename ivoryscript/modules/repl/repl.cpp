// repl.cpp

#include "repl.hpp"
#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <cstring>

void spacePrune(char *s) {
    if (s == NULL)
        return;

    int len = strlen(s);
    for (int i = strlen(s) - 1; i > 0; --i) {
        if (s[i] == ' ' && s[i - 1] == ' ') {
            memmove(&s[i], &s[i + 1], len - i);
            len--;
        }
    }
}

int replEval::replLoop() {
    time_t p_currDate = time(NULL);
    char *currDate = ctime(&p_currDate);

    currDate[strcspn(currDate, "\n")] = 0;
    std::string currDateStr(currDate);

    spacePrune(currDate);

    std::cout << "IvoryScript 1.0.0. (" << currDateStr
              << ") REPL (read-eval-print-loop) mode." << "\n";
    std::cout
        << "Type \"docs();\" for more information. "
        << "Type \"exit();\" to exit the REPL." << "\n\n";

    std::string content;
    std::string line;
    std::vector<std::string> lines;

    Program finalProgram;

    while (true) {
        while (std::getline(std::cin, content)) {
            lines.push_back(content);

            if (content == "exit();") {
                break;
            }

            // determine here whether the accumulated code is complete
        }

        for (int i = 0; i < lines.size(); ++i) {
            if (i < lines.size() - 1) {
                lines[i].append("\n");
            }

            content.append(lines[i]);
        }

        lines.clear();

        std::vector<Token> tokens = tokenize(content);
        content.clear();

        Parser currParser(tokens);
        std::unique_ptr<Program> currProgram;

        currProgram = currParser.parseProgram();
        finalProgram = std::move(*currProgram);

        if (currParser.quitRepl) {
            break;
        }
    }

    Traverser traverser(finalProgram);
    traverser.printAst();

    return 0;
}