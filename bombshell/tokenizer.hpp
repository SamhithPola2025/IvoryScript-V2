#pragma once;

#include <vector>

enum class typeToken {
    _return,
    integer_lit,
    semicolon
};

struct Token {
    typeToken type;
    std::optional<std::string> content;
};

std::vector<Token> tokenize(std::string& contents);