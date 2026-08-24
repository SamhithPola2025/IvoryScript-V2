#pragma once

#include "../parser/parser.hpp"

class Traverser {
  public:
    explicit Traverser(const Program &program) : program_(program) {}

    void printAst() const;

  private:
    const Program &program_;

    static void printIndent(int level);
    static void printExpr(const Expr &expr, int level);
    static void printStmt(const Stmt &stmt, int level);
    static void printStatements(const std::vector<std::unique_ptr<Stmt>> &statements,
                                int level);
};
