// helpers.cpp
#include "helpers.hpp"

dataType stringToEnum (std::string str) {
    return str == "int" ? dataType::Int :
     str == "float" ? dataType::Float :
     str == "string" ? dataType::Str :
     str == "list" ? dataType::List:
     str == "long" ? dataType::Long :
     str == "long long" ? dataType::Llong :

    /* gotta figure this out seperately, ^^
     conventionally 8 bytes too, but not sure of its exact purpose in this compiler as of now
     */

     str == "short" ? dataType::Short:
     str == "char" ? dataType::Char :
     str == "bool" ? dataType::Bool : dataType::COUNT; // COUNT is just held as placeholder for invalid type

    // string to enum helper function
}

std::string tokenTypeToString(tokenType tType) {
    switch (tType) {
        case tokenType::_return:
            return "return";
        case tokenType::integer_lit:
            return "integer_lit";
        case tokenType::semicolon:
            return "semicolon";
        case tokenType::string_type:
            return "string_type";
        case tokenType::list_type:
            return "list_type";
        case tokenType::string_lit:
            return "string_lit";
        case tokenType::identifier:
            return "var_name";
        case tokenType::equal:
            return "equals";
        case tokenType::int_type:
            return "int";
        case tokenType::eof:
            return "whitespace";
        case tokenType::plus:
            return "plus";
        case tokenType::minus:
            return "minus";
        case tokenType::asterisk:
            return "asterisk";
        case tokenType::solidus:
            return "solidus";
        case tokenType::pipe:
            return "pipe";
        default:
            return "unknown";
    }
}

bool isControlFlow(std::unique_ptr<Stmt> given) {
    if (typeid(*given) == typeid(funcStmt)) {
        return true;
    } else if (typeid(*given) == typeid(ifStmt)) {
        return true;
    } else if (typeid(*given) == typeid(whileStmt)) {
        return true;
    } else if (typeid(*given) == typeid(forStmt)) {
        return true;
    } else {
        return false;
    }
}