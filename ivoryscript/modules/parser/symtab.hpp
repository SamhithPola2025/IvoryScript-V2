#pragma once

#include "../helpers/_enums.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Stmt;
struct Param;

class Symbol {
  public:
    Symbol() = default;

    Symbol(Stmt *stmt) : defLiteral(stmt) {}
    Symbol(const Symbol &other);
    Symbol &operator=(const Symbol &other);

    dataType type = dataType::COUNT;

    std::vector<std::unique_ptr<Param>> params;

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
    symbolTableHandler() { symbolTables.emplace_back(); }

    void enterScope(scope newScope);
    void leaveScope();
    void pushToTable(const std::string &name, const Symbol &symbol);
    std::pair<std::string, Symbol> pullFromTable(const std::string &name,
                                                 const Symbol &symbol,
                                                 bool isFuncCall) const;
};
