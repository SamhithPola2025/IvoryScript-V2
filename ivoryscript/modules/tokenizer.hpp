// tokenizer.hpp
#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

inline int currline = 1, currcol = 1;

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
    open_brace,
    close_brace,
    print,
    exit,
    docs,
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
    sl_comment,
    eof
};

struct Token {
    tokenType type;
    std::optional<std::string> content;

    bool operator==(const Token &other) const {
        return this->type == other.type &&
               this->content.value() == other.content.value();
    }
};

std::vector<Token> tokenize(const std::string &contents);
