// repl.hpp
#pragma once

#include "../parser/parser.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <ctime>

class replEval {
  public:
    int replLoop();
};