// ast.hpp
#ifndef AST_HPP
#define AST_HPP

#include "tokenizer.hpp"
#include <memory>
#include <vector>
#include <string>

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
}

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

struct Stmt {
    virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;
};

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expr {
    virtual ~Expr() = default;
};

#endif