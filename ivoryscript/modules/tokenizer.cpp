// tokenizer.cpp
#include "tokenizer.hpp"
using std::string, std::vector;

bool isIdentC(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; 
    // oops fixed this i forgot alnum only takes unsigned char
}

std::vector<Token> tokenize(const std::string& contents) {
    std::vector<Token> tokens;
    std::string currentToken;
    // std::cout << contents << std::endl;

    for (int i = 0; i < contents.length(); ++i ) {
        char c = contents[i];

        if (c == '\n') {
            currline++;
            currcol = 1;
        } else {
            currcol++;
        }

        if (isspace(c)) continue;

        // handle specific tokens

        //string
        if (contents.substr(i, 6) == "string") {
            tokens.push_back({tokenType::string_type});
            i += 5;
            continue;
        }

        // int
        if (contents.substr(i, 3) == "int") {
            if ((contents[i + 1]) && (!isalnum((unsigned char)contents[i-1]))) {
                tokens.push_back({tokenType::int_type});
                i += 2;
                continue;
            }
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

        //print
        if (contents.substr(i, 5) == "print") {
            tokens.push_back({tokenType::print, std::nullopt});
            i += 4;
            continue;
        }

        //int literal
        if (isdigit(c)) {
            currentToken = c;
            while (i + 1 < contents.length() && isdigit(contents[i+1])) {
                currentToken += contents[++i];
            }
            tokens.push_back({tokenType::integer_lit, currentToken});
        }

        //identifier
        if (isalpha(c) || c == '_') {
            std::string name;
            name += c;

            while (i + 1 < contents.size() && (isalnum(contents[i+1]) || contents[i+1] == '_')) {
                name += contents[++i];
            }

            tokens.push_back({tokenType::identifier, name});
        }

        // std::cout << c << "\n";
        if (contents.substr(i, 4) == "true") {
            tokens.push_back({tokenType::bool_lit, "true"});
            continue;
        } else if (contents.substr(i, 5) == "false") {
            tokens.push_back({tokenType::bool_lit, "false"});
            continue;
        }

        //function
        if (contents.substr(i, 4) == "func") {
            std::string ret_type = "void";

            i += 4;

            while (i < contents.size() && isspace(contents[i])) {
                i++;
            }

            if (i < contents.size() && isalpha(contents[i])) {
                std::string possible_type;

                while (i < contents.size() && isalpha(contents[i])) {
                    possible_type += contents[i];
                    i++;
                }

                ret_type = possible_type;
            }

            tokens.push_back({tokenType::func_type, ret_type});
            continue;
        }

        if (contents.substr(i, 2) == "==") {
            tokens.push_back({tokenType::equal_equal});
            i += 2;
            continue;
        }

        if (contents.substr(i, 2) == "!=") {
            tokens.push_back({tokenType::not_equal});
            i += 2;
            continue;
        }

        if (contents.substr(i, 2) == ">=") {
            tokens.push_back({tokenType::greater_equal});
            i += 2;
            continue;
        }

        if (contents.substr(i, 2) == "<=") {
            tokens.push_back({tokenType::less_equal});
            i += 2;
            continue;
        }

        switch (c) {
            case ';':
                tokens.push_back({tokenType::semicolon, std::nullopt});
                continue;
            case '=':
                tokens.push_back({tokenType::equal, std::nullopt});
                continue;
            case '"': {
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
                break;
            }
            case '[':
                for (int k = i + 1; k < contents.size(); ++k) {
                    if (contents[k] == ']') {
                        tokens.push_back({tokenType::list_type});
                        i = k;
                        break;
                    }
                }
                continue;
            case '(':
                tokens.push_back({tokenType::open_paren, std::nullopt});
                continue;
            case ')':
                tokens.push_back({tokenType::close_paren, std::nullopt});
                continue;
            case ',':
                tokens.push_back({tokenType::comma, std::nullopt});
                continue;
            case '|':
                tokens.push_back({tokenType::pipe, std::nullopt});
                continue;
            case '&':
                tokens.push_back({tokenType::ampersand, std::nullopt});
                continue;
            case '+':
                tokens.push_back({tokenType::plus, std::nullopt});
                continue;
            case '-':
                tokens.push_back({tokenType::minus, std::nullopt});
                continue;
            case '/':
                tokens.push_back({tokenType::solidus, std::nullopt});
                continue;
            case '*':
                tokens.push_back({tokenType::asterisk, std::nullopt});
                continue;
            case '%':
                tokens.push_back({tokenType::percent, std::nullopt});
                continue;
            case '^':
                tokens.push_back({tokenType::caret, std::nullopt});
                continue;
            case '<':
                tokens.push_back({tokenType::less, std::nullopt});
                continue;
            case '>':
                tokens.push_back({tokenType::greater, std::nullopt});
                continue;
        }
    }
    
    tokens.push_back({tokenType::eof, std::nullopt});

    return tokens;
}