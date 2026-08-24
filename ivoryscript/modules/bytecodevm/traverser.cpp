#include "traverser.hpp"
#include "../helpers/helpers.hpp"

#include <iostream>

void Traverser::printAst() const {
    std::cout << "Currently generated AST:\n";
    if (program_.statements.empty()) {
        std::cout << "  <empty program>\n";
        return;
    }

    printStatements(program_.statements, 1);
}

void Traverser::printIndent(int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
}

void Traverser::printStatements(
    const std::vector<std::unique_ptr<Stmt>> &statements, int level) {
    for (const auto &statement : statements) {
        if (statement) {
            printStmt(*statement, level);
        }
    }
}

void Traverser::printStmt(const Stmt &stmt, int level) {
    printIndent(level);

    if (const auto *function = dynamic_cast<const FuncStmt *>(&stmt)) {
        std::cout << "Function: " << function->name << "\n";
        printStatements(function->funcStmts, level + 1);

    } else if (const auto *ifStmt = dynamic_cast<const IfStmt *>(&stmt)) {
        std::cout << "If\n";
        printIndent(level + 1);
        std::cout << "Condition:\n";
        if (ifStmt->condition) {
            printExpr(*ifStmt->condition, level + 2);
        }
        printIndent(level + 1);
        std::cout << "Then:\n";
        printStatements(ifStmt->thenBranch, level + 2);

    } else if (const auto *whileStmt = dynamic_cast<const WhileStmt *>(&stmt)) {
        std::cout << "While\n";
        if (whileStmt->condition) {
            printExpr(*whileStmt->condition, level + 1);
        }
        printStatements(whileStmt->body, level + 1);

    } else if (const auto *forStmt = dynamic_cast<const ForStmt *>(&stmt)) {
        std::cout << "For\n";
        printIndent(level + 1);
        std::cout << "Init:\n";
        if (forStmt->init) {
            printStmt(*forStmt->init, level + 2);
        }
        printIndent(level + 1);
        std::cout << "Condition:\n";
        if (forStmt->condition) {
            printExpr(*forStmt->condition, level + 2);
        }
        printIndent(level + 1);
        std::cout << "Increment:\n";
        if (forStmt->increment) {
            printStmt(*forStmt->increment, level + 2);
        }
        printStatements(forStmt->body, level + 1);

    } else if (const auto *print = dynamic_cast<const PrintStmt *>(&stmt)) {
        std::cout << "Print\n";
        if (print->printExpression) {
            printExpr(*print->printExpression, level + 1);
        }

    } else if (const auto *exitStmt = dynamic_cast<const ExitStmt *>(&stmt)) {
        std::cout << "Exit\n";

    } else if (const auto *docsStmt = dynamic_cast<const DocsStmt *>(&stmt)) {
        std::cout << "Docs\n";

    } else if (const auto *returnStmt =
                   dynamic_cast<const ReturnStmt *>(&stmt)) {
        std::cout << "Return\n";
        if (returnStmt->returnExpression) {
            printExpr(*returnStmt->returnExpression, level + 1);
        }

    } else if (const auto *variable = dynamic_cast<const VarStmt *>(&stmt)) {
        std::cout << "Variable: " << variable->name << "\n";
        if (variable->varExpression) {
            printExpr(*variable->varExpression, level + 1);
        }
    } else if (const auto *assign = dynamic_cast<const AssignStmt *>(&stmt)) {
        std::cout << "Assign: " << assign->name << "\n";
        if (assign->value) {
            printExpr(*assign->value, level + 1);
        }
    } else if (const auto *expression = dynamic_cast<const ExprStmt *>(&stmt)) {
        std::cout << "Expression statement\n";
        if (expression->expr) {
            printExpr(*expression->expr, level + 1);
        }
    } else {
        std::cout << "<unhandled statement>\n";
    }
}

void Traverser::printExpr(const Expr &expr, int level) {
    printIndent(level);

    if (const auto *number = dynamic_cast<const Number *>(&expr)) {
        std::cout << "Number: " << number->value << "\n";
    } else if (const auto *boolean = dynamic_cast<const BoolExpr *>(&expr)) {
        std::cout << "Bool: " << (boolean->value ? "true" : "false") << "\n";
    } else if (const auto *binary = dynamic_cast<const BinaryExpr *>(&expr)) {
        std::cout << "Binary operator: " << operatorText(binary->op.type)
                  << "\n";
        if (binary->left) {
            printExpr(*binary->left, level + 1);
        }
        if (binary->right) {
            printExpr(*binary->right, level + 1);
        }
    } else if (const auto *string = dynamic_cast<const StringExpr *>(&expr)) {
        std::cout << "String: \"" << string->_str.content.value_or("")
                  << "\"\n";
    } else if (const auto *character = dynamic_cast<const CharExpr *>(&expr)) {
        std::cout << "Char: '" << character->_char.content.value_or("")
                  << "'\n";
    } else if (const auto *ident =
                   dynamic_cast<const IdentifierExpr *>(&expr)) {
        std::cout << "Identifier: " << ident->name << "\n";
    } else {
        std::cout << "<unhandled expression>\n";
    }
}
