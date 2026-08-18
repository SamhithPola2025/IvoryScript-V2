// symtab.cpp
#include "symtab.hpp"
#include "parser.hpp"

Symbol::Symbol(const Symbol &other)
    : type(other.type), Scope(other.Scope), defLiteral(other.defLiteral) {
    for (const auto &param : other.params) {
        params.push_back(std::make_unique<Param>(*param));
    }
}

Symbol &Symbol::operator=(const Symbol &other) {
    if (this == &other) {
        return *this;
    }

    type = other.type;
    Scope = other.Scope;
    defLiteral = other.defLiteral;
    params.clear();
    for (const auto &param : other.params) {
        params.push_back(std::make_unique<Param>(*param));
    }
    return *this;
}

void symbolTableHandler::enterScope(scope newScope) {
    symbolTables.push_back(Context{newScope});
}

void symbolTableHandler::leaveScope() {
    if (symbolTables.size() > 1) {
        symbolTables.pop_back();
    }
}

void symbolTableHandler::pushToTable(const std::string &name,
                                     const Symbol &symbol) {
    symbolTables.back().symbols[name] = symbol;
}

std::pair<std::string, Symbol>
symbolTableHandler::pullFromTable(const std::string &name, const Symbol &symbol,
                                  bool isFuncCall) const {
    for (auto table = symbolTables.rbegin(); table != symbolTables.rend();
         ++table) {
        if (table->Scope != symbol.Scope) {
            continue;
        }

        const auto found = table->symbols.find(name);
        if (found != table->symbols.end()) {
            return {name, found->second};
        }
    }

    Parser::printError(
        "Invalid " +
        std::string(isFuncCall ? "function called: " : "variable accessed: ") +
        name);
    return {name, Symbol{}};
}
