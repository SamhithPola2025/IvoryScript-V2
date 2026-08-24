# IvoryScript 1.0.0 — Examples

This document describes the `examples/` folder at the root of the repository. The examples are small IvoryScript (`.ivc`) source files that exercise the parts of the language the tokenizer and parser already understand.

## How to run an example

From the repository root, after building the `ivoryscriptV2` binary (see the top-level `README.md` for build instructions), pass any example file as the source argument:

```
./ivoryscriptV2 examples/hello.ivc
```

The current pipeline ends at AST printing, so running an example will print a debug view of the program's AST rather than executing the program. That is intentional at this stage — these examples are designed to exercise and verify the parser.

> Tip: keep the examples deliberately short so it is easy to diff the AST output against the source by eye.

## What the examples cover

Every example in this folder is built out of syntax the parser actually supports today:

- Top-level `func <type> <name>() { ... }` declarations (no parameters).
- Top-level `return <expr>;` (optional, used when the program result is consumed externally).
- `print(<expr>);` statements.
- `if (...) { ... }` blocks (without `else`).
- `while (...) { ... }` loops.
- Variable declarations using only the implemented type keywords (`int`, `long`, `bool`, `string`, `list`).
- Integer, string, and char literals.
- Arithmetic operators: `+`, `-`, `*`, `/`.
- Comparison operators: `==`, `!=`, `<`, `<=`, `>`, `>=` (only inside `if`, `while`, or `for` conditions — not as bare top-level expression statements).
- Parenthesized sub-expressions.
- `//` line comments and `/* ... */` block comments.

If a construct is not on the list above, it will not appear in the examples.

## Conventions

- File names are lowercase, use underscores, and end in `.ivc`.
- Each file is self-contained — no imports or cross-file references.
- Comments at the top of each file explain what it is meant to demonstrate.
- The leading underscore in `_input.ivc` marks it as the historical reference example that the language has shipped with; treat it as the canonical "does it still parse?" check rather than a teaching example.

## Not yet covered by the examples

The following language features are real and documented (see `syntax.md`) but are not used in the examples yet because they would currently fail to parse:

- `for (<init>; <cond>; <increment>) { ... }` loops — the for-init slot is parsed as a bare expression, so neither `int i = 0;` style declarations nor chained assignments parse today.
- Initializing a `bool` variable from a `true`/`false` literal (e.g. `bool flag = true;`) — the literals tokenize, but the variable-declaration RHS does not accept `bool_lit` tokens.
- Function parameters.
- Function calls.
- Default-constructor variable form (`int count(5);`).
- User-defined objects, lists, maps.
- File-format specifiers like `@Ordered`.
- The `use (standl);` import and its members (`rin`, `rout`, etc.).

When those land, new examples should be added here that exercise them in isolation so the AST output can be inspected cleanly.
