// parser.cpp
#include "ast.hpp"
#include "tokenizer.hpp"

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
        const Token& t = peek();
        std::cout << "Error on line " << currline << " column " << currcol << ". " << message << std::endl;
        //exit(1); commented for testing purposes
    }

    std::unique_ptr<Program> Parser::parseProgram() {
        std::unique_ptr<Program> program = std::make_unique<Program>();
        while (peek().type != tokenType::eof) {
            size_t startPos = pos;
            std::unique_ptr<Stmt> stmt = parseStmt();
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
        

        if (peek().type == tokenType::_return) {
            advance();
            std::unique_ptr<Expr> expr = parseExpr();
            expect(tokenType::semicolon);
            return std::make_unique<ReturnStmt>(std::move(expr));
        }

        

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