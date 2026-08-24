// parser.cpp
#include "parser.hpp"
#include "../helpers/helpers.hpp"
#include "symtab.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <cassert>
#include <memory>

Parser::Parser(const std::vector<Token> &t) : tokens(t), pos(0) {}

std::vector<tokenType> varTypes = {tokenType::string_type, tokenType::int_type,
                                   tokenType::long_type, tokenType::list_type,
                                   tokenType::bool_type

};

dataType tokenToDataType(tokenType type) {
    switch (type) {
    case tokenType::int_type:
        return dataType::Int;
    case tokenType::string_type:
        return dataType::Str;
    case tokenType::bool_type:
        return dataType::Bool;
    case tokenType::long_type:
        return dataType::Long;
    case tokenType::list_type:
        return dataType::List;
    default:
        return dataType::COUNT;
    }
}

const Token &Parser::peekNext() {
    if (pos + 1 < tokens.size() && tokens[pos + 1].type != tokenType::eof) {
        return tokens[pos + 1];
    }
    return tokens.back();
}

const Token &Parser::peek() { return tokens[pos]; }

void Parser::advance() {
    if (pos < tokens.size() - 1) {
        pos++;
    }
}

void Parser::expect(tokenType expected) {
    if (peek().type != expected) {
        error("Unexpected token \"" + tokenTypeToString(peek().type) +
              "\", expected \"" + tokenTypeToString(expected) + "\".");
        return;
    }

    advance();
}

bool Parser::match(tokenType token) {
    if (peek().type == token) {
        advance();
        return true;
    }
    return false;
}

bool Parser::isOperator(tokenType type) {
    switch (type) {
    case tokenType::plus:
        return true;
    case tokenType::minus:
        return true;
    case tokenType::solidus:
        return true;
    case tokenType::asterisk:
        return true;
    default:
        return false;
    }
}

bool Parser::isExpStarter(tokenType type) {
    switch (type) {
    case tokenType::open_paren:
        return true;
    case tokenType::integer_lit:
        return true;
    case tokenType::identifier:
        return true;
    default:
        return false;
    }
}

void Parser::error(const std::string &message) {
    const Token &token = peek();
    std::cerr << "Error on line " << token.line << " column "
              << token.column << ". " << message << std::endl;
    exit(1);
}

void Parser::printError(const std::string &message) {
    std::cerr << "Error on line " << currline << " column " << currcol << ". "
              << message << std::endl;
    exit(1);
}

std::unique_ptr<Program> Parser::parseProgram() {
    std::unique_ptr<Program> program = std::make_unique<Program>();
    while (peek().type != tokenType::eof) {
        size_t startPos = pos;
        std::unique_ptr<Stmt> stmt = parseStmt();

        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        
        // prevents infinite looping if parsestmt for some reason doesn't advance.
        if (pos == startPos && peek().type != tokenType::eof) {
            advance();
        }
    }

    return program;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    // comments were removed prior to the parser seeing them.

    // reassignment: name = expr;
    if (peek().type == tokenType::identifier && peekNext().type == tokenType::equal) {
        const std::string name = *peek().content;
        Symbol placeholder;
        placeholder.Scope = currentScope;
        symbols.pullFromTable(name, placeholder, false);
        advance(); // identifier
        advance(); // =
        auto value = parseExpr();
        if (!value) {
            return nullptr;
        }
        expect(tokenType::semicolon);

        Symbol updated;
        updated.Scope = currentScope;
        updated.defLiteral = nullptr;
        if (!symbols.updateSymbol(name, updated)) {
            error("Cannot reassign undeclared variable '" + name + "'.");
        }
        return std::make_unique<AssignStmt>(name, std::move(value));
    }

    // variable declaration example: int count = 0;
    for (tokenType typeToken : varTypes) {
        if (peek().type != typeToken) {
            continue;
        }

        const dataType declaredType = tokenToDataType(typeToken);
        advance();

        if (peek().type != tokenType::identifier || !peek().content) {
            error("Expected variable name.");
            return nullptr;
        }

        const std::string name = *peek().content;
        advance();
        expect(tokenType::equal);

        auto value = parseExpr();
        if (!value) {
            return nullptr;
        }

        expect(tokenType::semicolon);

        auto variable = std::make_unique<VarStmt>(std::move(value));
        variable->name = name;
        variable->type = declaredType;

        Symbol variableSymbol(variable.get());
        variableSymbol.type = declaredType;
        variableSymbol.Scope = currentScope;
        symbols.pushToTable(name, variableSymbol);
        return variable;
    }

    // return statement
    if (peek().type == tokenType::_return) {
        advance();
        std::unique_ptr<Expr> expr = parseExpr();
        expect(tokenType::semicolon);
        return std::make_unique<ReturnStmt>(std::move(expr));
    }

    if (peek().type == tokenType::_if) {
        advance();
        expect(tokenType::open_paren);
        auto condition = parseComparison();
        if (!condition) {
            return nullptr;
        }
        expect(tokenType::close_paren);
        expect(tokenType::open_brace);

        auto ifStatement = std::make_unique<IfStmt>();
        ifStatement->condition = std::move(condition);

        const scope outerScope = currentScope;
        currentScope = scope::Block;
        symbols.enterScope(currentScope);

        while (peek().type != tokenType::eof &&
               peek().type != tokenType::close_brace) {
            size_t startPos = pos;
            if (auto statement = parseStmt()) {
                ifStatement->thenBranch.push_back(std::move(statement));
            }
            if (pos == startPos) {
                advance();
            }
        }

        expect(tokenType::close_brace);
        symbols.leaveScope();
        currentScope = outerScope;
        return ifStatement;
    }

    if (peek().type == tokenType::_while) {
        advance();
        expect(tokenType::open_paren);
        auto condition = parseComparison();
        if (!condition) {
            return nullptr;
        }
        expect(tokenType::close_paren);
        expect(tokenType::open_brace);

        auto whileStatement = std::make_unique<WhileStmt>();
        whileStatement->condition = std::move(condition);

        const scope outerScope = currentScope;
        currentScope = scope::Block;
        symbols.enterScope(currentScope);

        while (peek().type != tokenType::eof &&
               peek().type != tokenType::close_brace) {
            size_t startPos = pos;
            if (auto statement = parseStmt()) {
                whileStatement->body.push_back(std::move(statement));
            }
            if (pos == startPos) {
                advance();
            }
        }

        expect(tokenType::close_brace);
        symbols.leaveScope();
        currentScope = outerScope;
        return whileStatement;
    }

    if (peek().type == tokenType::_for) {
        advance();
        expect(tokenType::open_paren);

        std::unique_ptr<Stmt> initStatement;
        if (peek().type != tokenType::semicolon) {
            initStatement = parseStmt();
            if (!initStatement) {
                return nullptr;
            }
        } else {
            advance();
        }

        std::unique_ptr<Expr> condition;
        if (peek().type != tokenType::semicolon) {
            condition = parseComparison();
            if (!condition) {
                return nullptr;
            }
        }
        expect(tokenType::semicolon);

        std::unique_ptr<Stmt> increment;
        if (peek().type != tokenType::close_paren) {
            increment = parseStmt();
            if (!increment) {
                return nullptr;
            }
        }

        expect(tokenType::close_paren);
        expect(tokenType::open_brace);

        auto forStatement = std::make_unique<ForStmt>();
        forStatement->init = std::move(initStatement);
        forStatement->condition = std::move(condition);
        forStatement->increment = std::move(increment);

        const scope outerScope = currentScope;
        currentScope = scope::Block;
        symbols.enterScope(currentScope);

        while (peek().type != tokenType::eof &&
               peek().type != tokenType::close_brace) {
            size_t startPos = pos;
            if (auto statement = parseStmt()) {
                forStatement->body.push_back(std::move(statement));
            }
            if (pos == startPos) {
                advance();
            }
        }

        expect(tokenType::close_brace);
        symbols.leaveScope();
        currentScope = outerScope;
        return forStatement;
    }

    if (peek().type == tokenType::func_type) {
        Token functionToken = peek();

        if (!peek().content ||
            stringToEnum(peek().content.value()) == dataType::COUNT) {
            error("Expected function return type.");
            return nullptr;
        } else {
            advance();
        }

        auto function = std::make_unique<FuncStmt>();
        function->name = *peek().content;
        function->checkRetType(functionToken);

        Symbol functionSymbol(function.get());
        functionSymbol.type = function->retT;
        functionSymbol.Scope = scope::Global;
        symbols.pushToTable(function->name, functionSymbol);
        advance();

        expect(tokenType::open_paren);
        if (peek().type != tokenType::close_paren) {
            error("Function parameters are not supported yet.");
            return nullptr;
        }
        advance();
        expect(tokenType::open_brace);

        const scope outerScope = currentScope;
        currentScope = scope::Local;
        symbols.enterScope(currentScope);

        while (peek().type != tokenType::eof &&
               peek().type != tokenType::close_brace) {
            size_t startPos = pos;
            if (auto statement = parseStmt()) {
                function->funcStmts.push_back(std::move(statement));
            }
            if (pos == startPos) {
                advance();
            }
        }

        expect(tokenType::close_brace);
        symbols.leaveScope();
        currentScope = outerScope;
        return function;
    }

    // print statement
    if (peek().type == tokenType::print) {
        advance();
        std::unique_ptr<Expr> expr = parseExpr();
        expect(tokenType::semicolon);

        return std::make_unique<PrintStmt>(std::move(expr));
    }

    // exit() - halt the program
    if (peek().type == tokenType::exit) {
        quitRepl = true;

        advance();
        expect(tokenType::open_paren);
        expect(tokenType::close_paren);
        expect(tokenType::semicolon);
        return std::make_unique<ExitStmt>();
    }

    // docs() - print documentation
    if (peek().type == tokenType::docs) {
        helpRepl = true;

        advance();
        expect(tokenType::open_paren);
        expect(tokenType::close_paren);
        expect(tokenType::semicolon);
        return std::make_unique<DocsStmt>();
    }

    // Allow expression statements like: 5 + 6 * 8; or 1 == 1;
    std::unique_ptr<Expr> expr = parseComparison();
    if (!expr) {
        error("Unknown Statement");
        return nullptr;
    }

    if (peek().type == tokenType::semicolon) {
        advance();
    } else if (peek().type != tokenType::eof) {
        error("Expected ';' after expression.");
        return nullptr;
    }

    std::unique_ptr<ExprStmt> stmt = std::make_unique<ExprStmt>();
    stmt->expr = std::move(expr);
    return stmt;
}

std::unique_ptr<Expr> Parser::parsePrimary() {
    if (peek().type == tokenType::char_lit) {
        CharExpr exprb;

        if (peek().content.value().length() != 1) {
            error("Invalid string type in char container.");
        } else {
            exprb._char = peek();
            advance();

            return std::make_unique<CharExpr>(exprb);
        }
    } else if (peek().type == tokenType::string_lit) {
        StringExpr exprc;

        if (peek().content.value().length() >= 1) {
            exprc._str = peek();
            advance();

            return std::make_unique<StringExpr>(exprc);
            // i'll handle string concatenation later
        } else {
            return nullptr;
        }
    }

    if (peek().type == tokenType::integer_lit) {
        const std::string &str = *peek().content;

        if (!str.empty() &&
            std::isdigit(static_cast<unsigned char>(str[0]))) {
            advance();
            return std::make_unique<Number>(std::stoi(str));
        }

        return nullptr;
    }

    if (peek().type == tokenType::bool_lit) {
        const std::string &str = *peek().content;
        bool value = (str == "true");
        advance();
        return std::make_unique<BoolExpr>(value);
    }

    if (peek().type == tokenType::open_paren) {
        advance();
        auto expr = parseExpr();
        expect(tokenType::close_paren);
        return expr;
    }

    if (peek().type == tokenType::identifier) {
        std::string name = *peek().content;
        advance();
        return std::make_unique<IdentifierExpr>(name);
    }

    error("Invalid expression.");
    return nullptr;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto left = parsePrimary();
    if (!left) {
        return nullptr;
    }

    // mult and div
    while (peek().type == tokenType::asterisk ||
           peek().type == tokenType::solidus) {
        Token op = peek();
        advance();
        auto right = parsePrimary();
        if (!right) {
            return nullptr;
        }
        left =
            std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;

    // if char I believe this just returns the char and same goes for parseexpr
    // or that might be bs
}

std::unique_ptr<Expr> Parser::parseExpr() {
    auto left = parseTerm();

    if (!left) {
        return nullptr;
    }

    while (peek().type == tokenType::plus ||
     peek().type == tokenType::minus ||
     peek().type == tokenType::asterisk ||
     peek().type == tokenType::solidus) {

        Token op = peek();
        advance();
        auto right = parseTerm();
        if (!right) {
            return nullptr;
        }
        left =
            std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseComparison() {
    auto left = parseExpr();
    if (!left) {
        return nullptr;
    }

    switch (peek().type) {
    case tokenType::equal_equal:
    case tokenType::not_equal:
    case tokenType::less:
    case tokenType::less_equal:
    case tokenType::greater:
    case tokenType::greater_equal: {
        Token op = peek();
        advance();
        auto right = parseExpr();
        if (!right) {
            return nullptr;
        }
        return std::make_unique<BinaryExpr>(op, std::move(left),
                                            std::move(right));
    }
    default:
        return left;
    }
}

std::string toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return str;
}

void FuncStmt::checkRetType(Token funcT) { // fixed
    if (funcT.content) {
        retT = stringToEnum(funcT.content.value());

        bool valid = retT != dataType::COUNT;

        if (!valid) {
            Parser::printError("Invalid function return type present.");
        }

    } else {
        Parser::printError("No function return type present.");
    }
}

void VarStmt::checkType(Token varT) {
    if (varT.content) {
        type = stringToEnum(varT.content.value());

        bool valid = type != dataType::COUNT;

        if (!valid) {
            Parser::printError("Invalid data type present.");
        }

    } else {
        Parser::printError("Unexpected identifier " + *varT.name);
    }
}
