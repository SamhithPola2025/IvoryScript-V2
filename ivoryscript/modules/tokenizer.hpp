// tokenizer.hpp
#ifndef TOKENIZER_
#define TOKENIZER_

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
    open_paren,
    close_paren,
    print,
    solidus,
    ampersand,
    pipe,
    asterisk,
    percent,
    caret,
    equal_equal,
    not_equal,
    less_equal,
    greater_equal,
    less,
    greater,
    eof
};

struct Tokenizer {
    int currline;
    int currcol;
};

struct Token {
    tokenType type;
    std::optional<std::string> content;
};

std::vector<Token> tokenize(const std::string& contents);

#endif