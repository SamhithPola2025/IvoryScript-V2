// parser.cpp
#include "ast.hpp"
#include "tokenizer.hpp"
#include <memory>

Parser::Parser(const std::vector<Token>& t)
    : tokens(t), pos(0) {}

    const Token& Parser::peekNext() {
        if (pos+1<tokens.size() && tokens[pos+1].type != tokenType::eof) {
            return tokens[pos+1];
        }
        return tokens.back();
    }

    const Token& Parser::peek() {
        return tokens[pos];
    }

    void Parser::advance() {
        if (pos < tokens.size() - 1) {
            pos++;
        }
    }

    void Parser::expect(tokenType expected) {
        helpers helper;
        auto t = peek().type;

        std::string fullMsg = "Unexpected token \"" + helper.tokenTypeToString(t) + "\", expected \"" + helper.tokenTypeToString(expected) + "\".";

        // for simplification sake

        if (peek().type != expected) {
            if ((expected == tokenType::semicolon) && (t != expected)){
                error("Expected semicolon.");
            }
        } else if (peek().type != expected) {
            error(fullMsg);
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
            default:
                return false;
        }
    }

    void Parser::error(const std::string& message) {
        std::cout << "Error on line " << currline << " column " << currcol << ". " << message << std::endl;
        //exit(1); commented for debugging purposes
    }

    void Parser::printError(const std::string& message) {
        std::cout << "Error on line " << currline << " column " << currcol << ". " << message << std::endl;
        //exit(1); commented for debugging purposes
    }

    std::unique_ptr<Program> Parser::parseProgram() {
        std::unique_ptr<Program> program = std::make_unique<Program>();
        while (peek().type != tokenType::eof) {
            size_t startPos = pos;
            std::unique_ptr<Stmt> stmt = parseStmt();
           // std::unique_ptr<Stmt> inlineStmt = parseStmt();

            /*
            if (isInline && inlineStmt) {
                program->statements[]
            } else if (stmt) */
            if (stmt) {
                program->statements.push_back(std::move(stmt));
            }

            // Prevent an infinite loop if parseStmt reports an error without consuming input.
            if (pos == startPos && peek().type != tokenType::eof) {
                advance();
            }
        }
        
        return program;
    }

    std::unique_ptr<Stmt> Parser::parseStmt() {
        //return statement
        if (peek().type == tokenType::_return) {
            advance();
            std::unique_ptr<Expr> expr = parseExpr();
            expect(tokenType::semicolon);
            return std::make_unique<ReturnStmt>(std::move(expr));
        }

        if (peek().type == tokenType::func_type) {
            std::unique_ptr<Expr> expr = nullptr;

            std::unique_ptr<FuncStmt> fStmt = std::make_unique<FuncStmt>(std::move(expr));
            
            if (peek().content) {
                fStmt->checkReturnType(peek());
                advance();
            } else {
                error("No function return type present");
            }

            expect(tokenType::open_brace);

            while (peek().type != tokenType::eof && peek().type != tokenType::close_brace) {
                fStmt->funcStmts.push_back(parseStmt());
                expect(tokenType::semicolon);
            }

            expect(tokenType::close_brace);

            advance();
            return fStmt;
        }
        
        //print statement
        if (peek().type == tokenType::print) {
            advance();
            std::unique_ptr<Expr> expr = parseExpr();
            expect(tokenType::semicolon);
            
            return std::make_unique<PrintStmt>(std::move(expr));
        }

        // Allow expression statements like: 5 + 6 * 8;
        std::unique_ptr<Expr> expr = parseExpr();
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
        Token t = peek();
        if (t.type == tokenType::integer_lit) {
            advance();
            return std::make_unique<Number>(std::stoi(*t.content));
        }
        if (t.type == tokenType::open_paren) {
            advance();
            auto expr = parseExpr();
            expect(tokenType::close_paren);
            return expr;
        }
        error("Invalid expression.");
        return nullptr;
    }

    std::unique_ptr<Expr> Parser::parseTerm() {
        Token t = peek();
        auto left = parsePrimary();
        
        //mult and div
        while (peek().type == tokenType::asterisk || peek().type == tokenType::solidus) {
                Token op = peek();
                advance();
                auto right = parsePrimary();
                left = std::make_unique<binaryExpr>(op, std::move(left), std::move(right));
            }
        return left;
    }

    std::unique_ptr<Expr> Parser::parseExpr() {
        Token t = peek();
        auto left = parseTerm();
        while (peek().type == tokenType::plus|| peek().type == tokenType::minus) {
            Token op = peek();
            advance();
            auto right = parseTerm();
            left = std::make_unique<binaryExpr>(op,std::move(left),std::move(right));
        }
        return left;
    }

std::vector<std::string> validRetTypes = {"void", "int", "string", "bool"};

void FuncStmt::checkReturnType(Token funcT) {
    if (funcT.content) {
        retT = std::make_unique<std::string>(funcT.content.value());

        for (int i = 0; i < validRetTypes.size(); ++i) {
            if (i <= validRetTypes.size()) {
                if (*retT == validRetTypes[i]) {
                    break;
                }
            } else {
                Parser::printError("Invalid function return type present");
            }
        }
    } else {
        Parser::printError("No function return type present.");
    }
}