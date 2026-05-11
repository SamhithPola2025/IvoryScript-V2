#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <optional>
#include <fstream>
#include <filesystem>
#include <sstream>

#define str string;

enum class tokenType {
    _return,
    int_type,
    string_type,
    bool_type,
    long_type,
    list_type,
    semicolon,
    integer_lit,
    string_lit,
    equal,
    plus,
    minus,
    identifier,
    bool_lit,
    func_type,
    comma,
    open_brack,
    close_brack,
    print,
    solidus,
    ampersand,
    pipe,
    eof
};

struct Token {
    tokenType type;
    std::optional<std::string> content;
};

std::vector<Token> tokenize(std::string& contents);