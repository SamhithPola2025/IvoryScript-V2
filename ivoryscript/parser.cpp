// keep in mind that this is a basic parser, which I will continue to expand on in coming updates
#include "tokenizer.hpp";

struct Parser {
    std::vector<Token> tokens;
    int pos = 0;

    Token current() {
        return tokens[pos];
    }

    void advance() {
        ++pos;
    }

    bool match(typeToken type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }
};