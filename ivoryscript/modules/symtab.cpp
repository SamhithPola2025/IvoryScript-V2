// symtab.cpp
#include "symtab.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"

void symbolTableHandler::pushToTable(std::string name, Symbol &symbol) {
    long scopes = symbolTables.size();

    if (symbol.Scope != symbolTables.back().Scope) {
        for (int i = 0; i < scopes; ++i) {
            auto &s = symbolTables[i];

            if (s.Scope == symbol.Scope) {
                symbolTables[i].symbols[name] = symbol;
            } else {
                continue;
            }
        }
    } else {
        symbolTables.emplace_back(
            Context({symbol.Scope})); // agreggate initialization
        symbolTables.back().symbols[name] = symbol;
    }
}

std::pair<std::string, Symbol>
symbolTableHandler::pullFromTable(std::string name, Symbol &symbol,
                                  bool &isFuncCall) {
    bool isFound = false;

    if (symbol.Scope == symbolTables.back().Scope) {
        std::clog << "matched";
        return make_pair(name, symbolTables.back().symbols[name]);
    } else {
        for (int i = symbolTables.size() - 1; i >= 0; ++i) {
            if (symbolTables[i].Scope == symbol.Scope) {
                return make_pair(name, symbolTables[i].symbols[name]);
                isFound = true;
            } else {
                continue;
                isFound = false;
            }
        }

        if (!isFound) {
            Parser::printError(
                (std::string) "Invalid " +
                (isFuncCall ? "function called: " : "variable accessed: ") +
                name + "\n");
        }
    }
}