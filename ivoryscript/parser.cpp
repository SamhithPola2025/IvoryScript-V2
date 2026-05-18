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
                pos++;
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
        }

        Program* parseProgram() {
            Program* program = new Program;

            while (peek().type != tokenType::eof) {
                Stmt* stmt = parseStmt();
                program->statements.push_back(stmt);
            }

            return program;
        }

        Stmt* parseStmt() {
                Token t = peek();
                if (t.type == tokenType::_return) {
                    advance();

                    Expr* expr = parseExpr();
                    Stmt* stmt = new ReturnStmt(expr);

                    expect(tokenType::semicolon);

                    return stmt;
                }
            
            error("Unknown Statement");
            return nullptr;
        }

        Expr* parseExpr() {

        }

};

struct Program {
    std::vector<Stmt*> statements;
};

struct Expr {};

struct Stmt {
    virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
    Expr* expr;
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