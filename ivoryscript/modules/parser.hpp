// ast.hpp

#include "tokenizer.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// helpers
// enum -> string helper
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

// types and handlers

enum class dataType { Int, Str, List, Long, Llong, Short, Bool, Char, Float, COUNT };
enum class scope { Global, Local, Block };

dataType stringToEnum (std::string str) {
    return str == "int" ? dataType::Int :
     str == "float" ? dataType::Float :
     str == "string" ? dataType::Str :
     str == "list" ? dataType::List:
     str == "long" ? dataType::Long :
     str == "long long" ? dataType::Llong :

    /* gotta figure this out seperately, ^^
     conventionally 8 bytes too, but not sure of its exact purpose in this compiler as of now
     */

     str == "short" ? dataType::Short:
     str == "char" ? dataType::Char :
     str == "bool" ? dataType::Bool : dataType::COUNT; // COUNT is just held as placeholder for invalid type

    // string to enum helper function
}

class Symbol {
  public:
        Symbol() = default;

        Symbol(Stmt *stmt)
                : defLiteral(stmt) {}

    dataType type; // this would be the return type in the case of functions

    std::vector<std::optional<Param>> params;

    scope Scope = scope::Global;
    Stmt *defLiteral = nullptr;
};

struct Context {
public:
    scope Scope = scope::Global;
    std::unordered_map<std::string, Symbol> symbols;
};

struct Param {
  public:
    dataType paramType;
    std::unique_ptr<Param> nxtParam;

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
    FuncStmt(std::unique_ptr<Expr> expr) : funcExpression(std::move(expr)) {}

    dataType retT;
    void checkReturnType(Token funcT);
    std::vector<std::unique_ptr<Stmt>> funcStmts;

    std::unique_ptr<Expr> funcExpression;
};

struct varStmt : Stmt {
    public: 
        VarStmt(std::unique_ptr<Expr> expr) : varExpression(std::move(expr)) {}

        dataType type;
        Stmt value;
        
}

struct Number : public Expr {
    int value;
    Number(int v) : value(v) {}
};

struct binaryExpr : Expr {
    Token op;

    binaryExpr(Token op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(op), left(std::move(l)), right(std::move(r)) {}

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;
};

// parser and handler

class Parser {
  private:
    const std::vector<Token> &tokens;
    size_t pos;

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
};

class symbolTableHandler {
  private:
    std::vector<Context> symbolTables;

  public:
    symbolTableHandler() {
        symbolTables.emplace_back();
    }  

    void pushToTable(std::string name, Symbol &symbol);
    std::pair<std::string, Symbol> pullFromTable(std::string name, Symbol &symbol, bool &isFuncCall);
};