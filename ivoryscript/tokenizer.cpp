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
            continue;
        }

        // int
        if (contents.substr(i, 3) == "int") {
            tokens.push_back({tokenType::int_type});
            i += 2;
            continue;
        }

        // bool
        if (contents.substr(i, 4) == "bool") {
            tokens.push_back({tokenType::bool_type});
            i += 3;
            continue;
        }

        // long
        if (contents.substr(i, 4) == "long") {
            tokens.push_back({tokenType::long_type});
            i += 4;
            continue;
        }

        // return
        if (contents.substr(i, 6) == "return") {
            tokens.push_back({tokenType::_return});
            i += 5;
            continue;
        }

        if (c == '=') {
            tokens.push_back({tokenType::equal});
        }

        if (contents[i] == '"') {
            std::string value = "";
            int j = i + 1;
            while (j < contents.size() && contents[j] != '"') {
                value.push_back(contents[j]);
                j++;
            }
            if (j < contents.size() && contents[j] == '"') {
                tokens.push_back({tokenType::string_lit, value});
                i = j;
                continue;
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

        if (isalpha(c) || c == '_') {
            std::string name;
            name += c;

            while (i + 1 < contents.size() && (isalnum(contents[i+1]) || contents[i+1] == '_')) {
                name += contents[++i];
            }

            tokens.push_back({tokenType::identifier, name});
        }
        // std::cout << c << "\n";
    }

    return tokens;
}
