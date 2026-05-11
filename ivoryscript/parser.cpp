#include "tokenizer.hpp"
#include <string>

class Parser {
    const std::vector<Token>& tokens;
    size_t pos;

    public:
        //constructor
        Parser(const std::vector<Token>& t)
            : tokens(t), pos(0) {}

        //helpers
        const Token& peek() {
            return tokens[pos];
        }

        void advance() {
                pos++;
        }

        bool match(tokenType token) {
            if (peek().type == token) {
                advance();
                return true;
            }
            return false;
        }

        // returning expressions:

        Expr* 
};

struct Stmt {
    virtual void execute() {}
};

struct Expr {
    virtual ~Expr() = default;
};

struct Number : public Expr {
    int value;
};

class Addition : public Expr {
    public: 
        Expr* left;
        Expr* right;

        ~Addition () {
            delete left;
            delete right;
        }
};

// token stream reader
