#pragma once

#include "parser.hpp"
#include "tokenizer.hpp"

class Symbol {
  public:
    Symbol() = default;

    Symbol(Stmt* stmt)
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