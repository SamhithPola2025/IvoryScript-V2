// tokenizer.cpp
#include "tokenizer.hpp"
using std::string, std::vector;

std::vector<Token> tokenize(const std::string &contents) {
    currline = 1;
    currcol = 1;
    tokenline = 1;
    tokencol = 1;

    std::vector<Token> tokens;
    std::string currentToken;

    for (int i = 0; i < contents.length(); ++i) {
        char c = contents[i];
        tokenline = currline;
        tokencol = currcol;

        if (c == '\n') {
            currline++;
            currcol = 1;
        } else {
            currcol++;
        }

        if (isspace(c))
            continue;

        // identifier / keywords
        if (isalpha(c) || c == '_') {
            std::string word;
            word += c;

            while (i + 1 < contents.size() &&
                   (isalnum(contents[i + 1]) || contents[i + 1] == '_')) {
                word += contents[++i];
                ++currcol;
            }

            // string
            if (word == "string") {
                tokens.push_back({tokenType::string_type});
            }

            // docs (inbuilt func for printing documentation)
            else if (word == "docs") {
                tokens.push_back({tokenType::docs});
            }

            else if (word == "exit") {
                tokens.push_back({tokenType::exit});
            }

            // int
            else if (word == "int") {
                tokens.push_back({tokenType::int_type});
            }

            // bool
            else if (word == "bool") {
                tokens.push_back({tokenType::bool_type});
            }

            // long
            else if (word == "long") {
                tokens.push_back({tokenType::long_type});
            }

            // return
            else if (word == "return") {
                tokens.push_back({tokenType::_return});
            }

            else if (word == "if") {
                tokens.push_back({tokenType::_if});
            }

            else if (word == "while") {
                tokens.push_back({tokenType::_while});
            }

            else if (word == "for") {
                tokens.push_back({tokenType::_for});
            }

            // print
            else if (word == "print") {
                tokens.push_back({tokenType::print, std::nullopt});
            }

            // bool literal
            else if (word == "true" || word == "false") {
                tokens.push_back({tokenType::bool_lit, word});
            }

            // function
            else if (word == "func") {
            std::string ret_type = "void";

                i++;
                ++currcol;

                while (i < contents.size() && isspace(contents[i])) {
                    if (contents[i] == '\n') {
                        ++currline;
                        currcol = 1;
                    } else {
                        ++currcol;
                    }
                    i++;
                }

                if (i < contents.size() && isalpha(contents[i])) {
                    std::string possible_type;

                    while (i < contents.size() && isalpha(contents[i])) {
                        possible_type += contents[i];
                        ++currcol;
                        i++;
                    }

                    ret_type = possible_type;
                    i--;
                }

                tokens.push_back({tokenType::func_type, ret_type});
            }

            // identifier
            else {
                tokens.push_back({tokenType::identifier, word,
                                  std::make_shared<std::string>(word)});
            }

            continue;
        }

        // int literal
        if (isdigit(c)) {
            currentToken = c;

            while (i + 1 < contents.length() && isdigit(contents[i + 1])) {
                currentToken += contents[++i];
                ++currcol;
            }

            tokens.push_back({tokenType::integer_lit, currentToken});
            continue;
        }

        // string literal
        if (c == '"') {
            std::string value = "";
            int j = i + 1;

            while (j < contents.size() && contents[j] != '"') {
                value.push_back(contents[j]);
                if (contents[j] == '\n') {
                    ++currline;
                    currcol = 1;
                } else {
                    ++currcol;
                }
                j++;
            }

            if (j < contents.size() && contents[j] == '"') {
                ++currcol;
                tokens.push_back({tokenType::string_lit, value});
                i = j;
                continue;
            }
        }

        // char literal
        if (c == '\'') {
            char value;
            int j = i + 1;

            if (j < contents.size() && contents[j] != '\'') {
                value = contents[j];
                currcol++;
                
                j++;
            } else {
                std::cerr << "Size of char literal is invalid or contains invalid data." << "\n";
            }

            if (j < contents.size() && contents[j] == '\'') {
                ++currcol;
                i = j;

                tokens.push_back({tokenType::char_lit, std::string(1,value)});
                continue;
            }
        }

        // Comments are ignored here so the parser never needs to handle them.
        if (contents.substr(i, 2) == "//") {
            i += 2;
            ++currcol; // The outer loop already counted the first '/'.
            while (i < contents.size() && contents[i] != '\n') {
                ++currcol;
                ++i;
            }
            --i; // Let the outer loop process the newline normally.
            continue;
        }

        if (contents.substr(i, 2) == "/*") {
            i += 2;
            ++currcol; // The outer loop already counted the first '/'.
            while (i + 1 < contents.size() && contents.substr(i, 2) != "*/") {
                if (contents[i] == '\n') {
                    ++currline;
                    currcol = 1;
                } else {
                    ++currcol;
                }
                ++i;
            }

            if (i + 1 < contents.size()) {
                currcol += 2; // Count the closing '*' and '/'.
                ++i;          // The outer loop skips the closing '/'.
            }
            continue;
        }

        // operators
        if (contents.substr(i, 2) == "==") {
            tokens.push_back({tokenType::equal_equal});
            i++;
            continue;
        }

        if (contents.substr(i, 2) == "!=") {
            tokens.push_back({tokenType::not_equal});
            i++;
            continue;
        }

        if (contents.substr(i, 2) == ">=") {
            tokens.push_back({tokenType::greater_equal});
            i++;
            continue;
        }

        if (contents.substr(i, 2) == "<=") {
            tokens.push_back({tokenType::less_equal});
            i++;
            continue;
        }

        switch (c) {
        case ';':
            tokens.push_back({tokenType::semicolon, std::nullopt});
            continue;
        case '=':
            tokens.push_back({tokenType::equal, std::nullopt});
            continue;
        case '(':
            tokens.push_back({tokenType::open_paren, std::nullopt});
            continue;
        case '\\':
            tokens.push_back({tokenType::backslash, std::nullopt});
            continue;
        case ')':
            tokens.push_back({tokenType::close_paren, std::nullopt});
            continue;

        case '{':
            tokens.push_back({tokenType::open_brace, std::nullopt});
            continue;

        case '}':
            tokens.push_back({tokenType::close_brace, std::nullopt});
            continue;

        case ',':
            tokens.push_back({tokenType::comma, std::nullopt});
            continue;

        case '\'':
            tokens.push_back({tokenType::single_quote, std::nullopt});
            continue;

        case '"':
            tokens.push_back({tokenType::double_quote, std::nullopt});
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

        case '*':
            tokens.push_back({tokenType::asterisk, std::nullopt});
            continue;

        case '/':
            tokens.push_back({tokenType::solidus, std::nullopt});
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

    tokenline = currline;
    tokencol = currcol;
    tokens.push_back({tokenType::eof, std::nullopt});

    return tokens;
}
