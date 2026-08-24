# IvoryScript 1.0.0 - Specification

## Standard Library

By far the most vast library in the language, included using the following syntax:

``use (standl);``

Where the parenthesis surrounding ``standl`` are based on convention but are not strictly required.

## Implemented built-ins

The following built-in identifiers are recognized directly by the tokenizer/parser. They are **not** part of a `use (standl);` import — they are language-level keywords today.

``print()`` — A built-in that takes any streamable type (section ex.) and outputs it to the standard output stream. By default this flushes the output stream and makes a new line.

``docs()`` — A built-in primarily used by the Ivory REPL feature. Outputs an IvoryScript documentation menu to the standard output stream.

``exit()`` — A built-in token recognized by the tokenizer, used to terminate program/REPL execution. The exact runtime handling is part of the still-to-be-built VM stage.

## Not implemented yet

The following entries are part of the planned standard library surface but are **not** part of the current tokenizer/parser pipeline:

- ``use (standl);`` import statement — the module/import system has not been built yet.
- ``rin()`` — planned input function that reads from the console/terminal into a string type.
- ``rout()`` — planned output function for streamable types that does **not** flush or append a newline by default.
- ``print()`` and ``docs()`` are recognized as tokens but their runtime behavior is not wired up until the VM stage lands. Until then, they show up in the AST as `PrintStmt` / `docs` tokens and are emitted by the AST traverser.
