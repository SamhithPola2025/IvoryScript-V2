// tokenizer, it takes the extracted text and turns it into tokens
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "tokenizer.hpp"

std::vector<Token> tokenize(std::string& contents) {
    std::vector<Token> tokens;
    std::string currentToken;
    // std::cout << contents << std::endl;

    for (int i = 0; i < contents.length(); ++i ) {
        char c = contents[i];
        if (isspace(c)) continue;

        // handle specific tokens

        if (c == ';') {
            tokens.push_back({typeToken::semicolon, std::nullopt});
        }

        if (contents.substr(i, 6) == "return") {
            tokens.push_back({typeToken::_return, "return"});
            i += 5;
        }

        else if (isdigit(c)) {
            currentToken = c;
            while (i + 1 < contents.length() && isdigit(contents[i+1])) {
                currentToken += contents[++i];
            }
            tokens.push_back({typeToken::integer_lit, currentToken});
        }

        // std::cout << c << "\n";
    }

    return tokens;
}
