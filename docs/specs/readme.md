# IvoryScript 1.0.0 - Specification

IvoryScript is a compiled, bytecode-based, general-purpose programming language implemented in C++. It is designed for writing algorithms, application-level code, and developer tooling, emphasizing low-level control and predictable execution.

IvoryScript provides you with the freedom that you need 

- Language Classification: Compiled (Bytecode-compiled, not AOT native)

- Purpose: General-purpose language that can be used for solving algorithmic problems, building applications and making developer tools in.

- Namespace model: Everything is in the global namespace when it is included, though this can cause naming conflicts this is chosen for the sake of simplicity.

## Implementation model:

- Interpreted into an abstract syntax tree (AST) using recursive descent parsing and is written entirely in C++.

- Then the AST is traversed and turn it into bytecode (custom-defined) 

- Bytecode is then executed by a custom defined VM (virtual machine) that executes the instructions on a low level.

- This language does not compile to a pre-existing runtime because the bytecode and VM are designed around the components and specification of the language.

