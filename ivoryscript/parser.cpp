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
            while (peek().type != tokenType::eof){
                pos++;
            }
        }

        Token* expect(tokenType expected) {
            if (expected == tokenType::semicolon) {
                advance();
            } else {
                error("Missing semicolon.");
                return nullptr;
            }
        }

        bool match(tokenType token) {
            if (peek().type == token) {
                advance();
                return true;
            }
            return false;
        }

        void error(const std::string& message) {
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
            Token t = peek();

            if (t.type == tokenType::_return) {
                advance();

                std::unique_ptr<Expr> expr = std::make_unique<Expr>(parseExpr);
                std::unique_ptr<Stmt> stmt = std::make_unique<ReturnStmt>(expr.get()); // depends on your Expr ownership

                expect(tokenType::semicolon);

                return stmt;
            }

            error("Unknown Statement");
            return nullptr;
        }

        std::unique_ptr<Expr> parsePrimary() {
            Token t = peek();
            if (t.type == tokenType::integer_lit) {
                advance();
                return std::make_unique<Expr>(t);
            }

            if (t.type == tokenType::open_paren) {
                
                advance();

            }

            return nullptr;
            error("Expression expected.");
        }

        std::unique_ptr<Expr> parseExpr() {
            Token t = peek();
         //   if (t.type == tokenType::minus)
        }

};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expr {};

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
};

class Addition : public Expr {
public: 
    Addition(std::unique_ptr<Expr> r, std::unique_ptr<Expr> l)
        : right(std::move(r)), left(std::move(l)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};


// token stream reader
// we need to figure out what to do if error