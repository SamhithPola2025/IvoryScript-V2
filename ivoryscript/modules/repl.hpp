// repl.hpp
#ifndef REPL_HPP
#define REPL_HPP

#include "parser.hpp"
#include "tokenizer.hpp"
#include <ctime>

class replEval {
  public:
    int replLoop();
};

#endif