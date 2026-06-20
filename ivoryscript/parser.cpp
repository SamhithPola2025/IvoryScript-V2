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

        bool isOperator(tokenType type) {
            switch (type) {
                case tokenType::plus:
                    return true;
                case tokenType::minus:
                    return true;
                case tokenType::slash:
                    return true;
                case tokenType::asterisk:
                    return true;
                default:
                    return false;
            }
        }

        std::unique_ptr<Expr> parseExpr() {
            Token t = peek();
            if (t.type == tokenType::integer_lit) {
                advance();
                int value = std::stoi(*t.value);
                return std::make_unique<Number>(value);
            }
            if (t.type == tokenType::open_paren) {
                advance();
                auto expr = parseExpr();
                expect(tokenType::close_paren);
                return expr;
            }

            auto left = parsePrimary();

            if (isOperator(t.type)) {

                auto sign = peek();
                advance();
                auto right = parsePrimary();
                return std::make_unique<binaryExpr>(sign, std::move(left), std::move(right));
            }

            while(isOperator(t.type)) {
                //placeholder for chained operations with multiple expressions
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