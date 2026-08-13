// helpers.hpp
#pragma once

#include "parser.hpp"
#include "tokenizer.hpp"

dataType stringToEnum(std::string str);

std::string tokenTypeToString(tokenType tType);

bool isControlFlow(std::unique_ptr<Stmt> given);