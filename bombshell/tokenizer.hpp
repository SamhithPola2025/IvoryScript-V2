#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <optional>
#include <fstream>
#include <filesystem>
#include <sstream>

enum class tokenType {
    _return, // done
    int_type, // done
    string_type, // done
    bool_type, // done
    long_type, // done
    list_type, // done
    semicolon,
    integer_lit,
    string_lit, 
    equal, // done
    plus,
    minus,
    comma
};

struct Token {
    tokenType type;
    std::optional<std::string> content;
};

std::vector<Token> tokenize(std::string& contents);