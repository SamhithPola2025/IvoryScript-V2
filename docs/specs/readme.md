# IvoryScript 1.0.0 - Specification

IvoryScript is a compiled, bytecode-based, general-purpose programming language implemented in C++. It is designed for writing algorithms, application-level code, and developer tooling, emphasizing low-level control and predictable execution.

IvoryScript provides you with the freedom that you need

- Language Classification: Compiled (Bytecode-compiled, not AOT native)

- Purpose: General-purpose language that can be used for solving algorithmic problems, building applications and making developer tools in.

- Namespace model: Everything is in the global namespace when it is included, though this can cause naming conflicts this is chosen for the sake of simplicity.

## Implementation model:

- Interpreted into an abstract syntax tree (AST) using recursive descent parsing and is written entirely in C++.

- Then the AST is traversed and printed by a debug traverser (an intermediate step before bytecode emission).

- Bytecode emission and the custom VM are **not implemented yet** — currently the pipeline stops at AST generation.

- This language does not compile to a pre-existing runtime because the bytecode and VM are designed around the components and specification of the language.

## Current implementation status (1.0.0):

- Implemented:
  - Tokenizer (full lexical analysis for all currently-supported syntax).
  - Recursive-descent parser producing an AST.
  - AST debug printer (traverser) used as the current program output.
  - REPL (read-eval-print-loop) shell entry point.

- Not implemented yet:
  - Bytecode code generation.
  - Custom virtual machine execution.
  - Standard library import / module system.
  - File-format specifiers (e.g. `@Ordered`).
