// ast.hpp
#ifndef AST_HPP
#define AST_HPP

#include "tokenizer.hpp"
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

// enum -> string helper
class helpers {
public:
std::string tokenTypeToString(tokenType tType) {
    switch (tType) {
        case tokenType::_return:
            return "return";
        case tokenType::integer_lit:
            return "integer_lit";
        case tokenType::semicolon:
            return "semicolon";
        case tokenType::string_type:
            return "string_type";
        case tokenType::list_type:
            return "list_type";
        case tokenType::string_lit:
            return "string_lit";
        case tokenType::identifier:
            return "var_name";
        case tokenType::equal:
            return "equals";
        case tokenType::int_type:
            return "int";
        case tokenType::eof:
            return "whitespace";
        case tokenType::plus:
            return "plus";
        case tokenType::minus:
            return "minus";
        case tokenType::asterisk:
            return "asterisk";
        case tokenType::solidus:
            return "solidus";
        case tokenType::pipe:
            return "pipe";
        default:
            return "unknown";
    }
}
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expr {
    virtual ~Expr() = default;
};

struct ReturnStmt : Stmt {
public:
    ReturnStmt(std::unique_ptr<Expr> expr)
        : returnExpression(std::move(expr)){}

    std::unique_ptr<Expr> returnExpression;
};

struct PrintStmt : Stmt {
public:
    PrintStmt(std::unique_ptr<Expr> expr)
        : printExpression(std::move(expr)){}

    std::unique_ptr<Expr> printExpression;
};

struct FuncStmt : Stmt {
public:
    FuncStmt(std::unique_ptr<Expr> expr)
        : funcExpression(std::move(expr)){}

    std::unique_ptr<std::string> retT;
    void checkReturnType (Token funcT);
    std::vector<std::unique_ptr<Stmt>> funcStmts;

    std::unique_ptr<Expr> funcExpression;
};

struct Number : public Expr {
    int value;
    Number(int v) : value(v) {}
};

struct binaryExpr : Expr {
    // format:
    // std::make_unique<binaryExpr>(op, std::move(left), std::move(right));

    Token op;

    binaryExpr(Token op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
    : op(op), left(std::move(l)), right(std::move(r)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};



struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;
};

class Parser {
public:
    Parser(const std::vector<Token>& t);
    std::unique_ptr<Program> parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t pos;

    std::unique_ptr<Stmt> currentNode; 
    bool isInline = false;

    const Token& peek();
    const Token& peekNext();
    void advance();

    bool match(tokenType token);

    bool isOperator(tokenType type);
    bool isExpStarter(tokenType type);

public:
    void error(const std::string& message);
    static void printError(const std::string& message);

    void expect(tokenType expected);

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseExpr();
};


#endif