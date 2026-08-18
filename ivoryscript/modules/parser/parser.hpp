// ast.hpp
#pragma once

#include "../helpers/_enums.hpp"
#include "symtab.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// types and handlers

struct Param {
  public:
    dataType paramType;
    std::unique_ptr<Param> nxtParam;

    Param &operator=(const Param &other) {
        if (this == &other) {
            return *this;
        }

        paramType = other.paramType;

        if (other.nxtParam) {
            nxtParam = std::make_unique<Param>(*other.nxtParam);
        } else {
            nxtParam.reset();
        }

        return *this;
    }

    // Copy constructor
    Param(const Param &other) : paramType(other.paramType) {
        if (other.nxtParam) {
            nxtParam = std::make_unique<Param>(*other.nxtParam);
        }
    }

    // Normal constructor
    Param(dataType pType, std::unique_ptr<Param> next)
        : paramType(pType), nxtParam(std::move(next)) {}
};

// Program Flow:

struct Program {
    std::vector<std::unique_ptr<Stmt>> statements;
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct Expr {
    virtual ~Expr() = default;
};

struct ReturnStmt : Stmt {
  public:
    ReturnStmt(std::unique_ptr<Expr> expr)
        : returnExpression(std::move(expr)) {}

    std::unique_ptr<Expr> returnExpression;
};

struct PrintStmt : Stmt {
  public:
    PrintStmt(std::unique_ptr<Expr> expr) : printExpression(std::move(expr)) {}

    std::unique_ptr<Expr> printExpression;
};

struct ExitStmt : Stmt {};

struct FuncStmt : Stmt {
  public:
    FuncStmt() = default;

    FuncStmt(std::unique_ptr<Expr> expr) : funcExpression(std::move(expr)) {}

    std::string name;
    dataType retT;

    void checkRetType(Token funcT);
    std::vector<std::unique_ptr<Stmt>> funcStmts;

    std::unique_ptr<Expr> funcExpression;
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> thenBranch;
    std::vector<std::unique_ptr<Stmt>> elseBranch; // optional
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
};

struct ForStmt : Stmt {
    std::unique_ptr<Stmt> init;
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> increment;
    std::vector<std::unique_ptr<Stmt>> body;
};

struct VarStmt : Stmt {
  public:
    VarStmt(std::unique_ptr<Expr> expr) : varExpression(std::move(expr)) {}
    dataType type; // the type of the variable, not to be confused with value
    Stmt value; // what it's actually equal to - the value of the variable

    std::string name; // identifier name, not that it matters too much but we retain this info into parsing
    void checkType(Token varT);
    std::unique_ptr<Expr> varExpression;
};

struct Number : public Expr {
    int value;
    Number(int v) : value(v) {}
};

struct BinaryExpr : Expr {
    Token op;

    BinaryExpr(Token op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(op), left(std::move(l)), right(std::move(r)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

struct CharExpr : Expr {
    Token _char;
};

struct StringExpr : Expr {
    Token _str;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;
};

// parser and handler

class Parser {
  private:
    const std::vector<Token> &tokens;
    size_t pos;

    symbolTableHandler symbols;
    scope currentScope = scope::Global;

    std::unique_ptr<Stmt> currentNode;
    bool isInline = false;

    const Token &peek();
    const Token &peekNext();
    void advance();

    bool match(tokenType token);

    bool isOperator(tokenType type);
    bool isExpStarter(tokenType type);

  public:
    Parser(const std::vector<Token> &t);
    std::unique_ptr<Program> parseProgram();

    void error(const std::string &message);
    static void printError(const std::string &message);

    void expect(tokenType expected);

    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseComparison();
};
