#pragma once

#include "tokenizer.hpp"
#include "_enums.hpp"

struct Stmt;
struct Param;

class Symbol {
  public:
    Symbol() = default;

    Symbol(Stmt *stmt) : defLiteral(stmt) {}
    Symbol(const Symbol& other);

    dataType type; // this would be the return type in the case of functions

    std::vector<std::unique_ptr<Param>> params;

    // assignment operator overload (the fix should work)

    Symbol& operator=(const Symbol& other)
    {
        if (this == &other) {
            return *this;
        }

        if (other.Scope != this->Scope) {
           this->Scope = other.Scope;
        }

        
        
        return *this;
    }

    scope Scope = scope::Global;
    Stmt *defLiteral = nullptr;
};

Symbol::Symbol(const Symbol& other)
    : Scope(other.Scope) {
        for (const auto& param : other.params) {
            params.push_back(std::make_unique<Param>(*param));
        }
    }

struct Context {
  public:
    scope Scope = scope::Global;
    std::unordered_map<std::string, Symbol> symbols;
};

class symbolTableHandler {
  private:
    std::vector<Context> symbolTables;

  public:
    symbolTableHandler() { symbolTables.emplace_back(); }

    void pushToTable(std::string name, Symbol &symbol);
    std::pair<std::string, Symbol>
    pullFromTable(std::string name, Symbol &symbol, bool &isFuncCall);
};