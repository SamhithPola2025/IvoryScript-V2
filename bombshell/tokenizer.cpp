// tokenizer, it takes the extracted text and turns it into tokens
#include "tokenizer.hpp"
using std::string, std::vector;

std::vector<Token> tokenize(std::string& contents) {
    std::vector<Token> tokens;
    std::string currentToken;
    // std::cout << contents << std::endl;

    for (int i = 0; i < contents.length(); ++i ) {
        char c = contents[i];
        if (isspace(c)) continue;

        // handle specific tokens

        if (c == ';') {
            tokens.push_back({tokenType::semicolon, std::nullopt});
        }
    
        //string
        if (contents.substr(i, 6) == "string") {
            tokens.push_back({tokenType::string_type});
            i += 5;
        }

        // int
        if (contents.substr(i, 3) == "int") {
            tokens.push_back({tokenType::int_type});
            i += 2;
        }

        // bool
        if (contents.substr(i, 4) == "bool") {
            tokens.push_back({tokenType::bool_type});
            i += 3;
        }

        // long
        if (contents.substr(i, 4) == "long") {
            tokens.push_back({tokenType::long_type});
            i += 4;
        }

        // return
        if (contents.substr(i, 6) == "return") {
            tokens.push_back({tokenType::_return});
            i += 5;
        }

        if (c == '=') {
            tokens.push_back({tokenType::equal});
        }

        if (contents[i] == '"') {
            string value = "";
            bool canPush = false;
            for (int j = i + 1; j < contents.size(); ++j) {
                value.push_back(contents[j]);
                if (contents[j] == '"') {
                    canPush = true;
                }
            }
            if (canPush == true) {
                tokens.push_back({tokenType::string_lit, value});
            }
        }
        
        // list_type
        if (contents[i] == '[') {
            for (int k = i + 1; k < contents.size(); ++k) {
                if (contents[k] == ']') {
                    tokens.push_back({tokenType::list_type});
                    i = k;
                    break;
                }
            }
        }


        //int literal
        if (isdigit(c)) {
            currentToken = c;
            while (i + 1 < contents.length() && isdigit(contents[i+1])) {
                currentToken += contents[++i];
            }
            tokens.push_back({tokenType::integer_lit, currentToken});
        }

        // std::cout << c << "\n";
    }

    return tokens;
}
