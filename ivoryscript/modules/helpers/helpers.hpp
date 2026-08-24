// helpers.hpp
#pragma once

#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

dataType stringToEnum(std::string str);

std::string tokenTypeToString(tokenType tType);

std::string isControlFlow(std::unique_ptr<Stmt> given);

const char *operatorText(tokenType type);