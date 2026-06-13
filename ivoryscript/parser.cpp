#include "tokenizer.hpp"
#include <string>
#include <memory>

class Parser {
    const std::vector<Token>& tokens;
    size_t pos;
    Tokenizer tokenizer;

    public:
        //constructor
        Parser(const std::vector<Token>& t)
            : tokens(t), pos(0) {}

        //helpers
        const Token& peek() {
            return tokens[pos];
        }

        void advance() {
            if (pos < tokens.size() - 1) {
                pos++;
            }
        }

        void expect(tokenType expected) {
            if (peek().type != expected) {
                error("Unexpected token, expected different type.");
            }
            advance();
        }

        bool match(tokenType token) {
            if (peek().type == token) {
                advance();
                return true;
            }
            return false;
        }

        void error(const std::string& message) {
            const Token& t = peek();
            std::cout << "Error on line" << tokenizer.currline << "column" << tokenizer.currcol << ". " << message << std::endl;
            exit(1);
        }

        std::unique_ptr<Program> parseProgram() {
            std::unique_ptr<Program> program = std::make_unique<Program>();

            while (peek().type != tokenType::eof) {
                std::unique_ptr<Stmt> stmt = parseStmt();

                program->statements.push_back(std::move(stmt));
            }

            return program;
        }

        std::unique_ptr<Stmt> parseStmt() {
            if (peek().type == tokenType::_return) {
                advance();

                auto expr = parseExpr();
                expect(tokenType::semicolon);

                return std::make_unique<ReturnStmt>(std::move(expr));
            }

            error("Unknown Statement");
            return nullptr;
        }

        std::unique_ptr<Expr> parsePrimary() {
            Token t = peek();
            if (t.type == tokenType::integer_lit) {
                advance();
                return std::make_unique<Number>(t);
            }

            if (t.type == tokenType::open_paren) {
                advance();
                return std::make_unique<Expr>(t);
            }

            return nullptr;
            error("Expression expected.");
        }

        std::unique_ptr<Expr> parseExpr() {
            Token t = peek();
            binaryExpr newExpr = std::make_unique<binaryExpr>;
            if (t.type == tokenType::minus) {
                return std::make_unique<Number>(t);
            }
        }

};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expr {
    virtual ~Expr() = default;
};

struct binaryExpr : Expr {
    Token op;

    binaryExpr(Token op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
    : op(op), left(std::move(l)), right(std::move(r)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;
};

struct ReturnStmt : Stmt {
public:
    ReturnStmt(std::unique_ptr<Expr> expr)
        : returnExpression(std::move(expr)){}

    std::unique_ptr<Expr> returnExpression;
};

struct Number : public Expr {
    int value;
    Number(int v) : value(v) {}
};

// token stream reader
// we need to figure out what to do if error