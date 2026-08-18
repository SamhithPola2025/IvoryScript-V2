// helpers.hpp
#pragma once

#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

dataType stringToEnum(std::string str);

std::string tokenTypeToString(tokenType tType);

bool isControlFlow(std::unique_ptr<Stmt> given);