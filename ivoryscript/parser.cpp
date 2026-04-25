#include "tokenizer.hpp"
#include <string>

struct Stmt {};

class letStmt : Stmt {
    std::string type;
    std::string val;
};

class returnStmt : Stmt {
    std::optional<std::string> type;
    std::string val;
};



