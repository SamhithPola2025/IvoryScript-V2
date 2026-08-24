# IvoryScript 1.0.0 — Types

This document tracks which type keywords the tokenizer/parser actually understand today. Anything listed under "Not implemented yet" is on the roadmap but has not been wired into the parser yet.

---

## Implemented types

These type keywords are recognized by the tokenizer and accepted by `VarStmt` parsing.

- `int` — integer type (stored as `Number` in the AST, parsed via `std::stoi`).
- `long` — long integer type keyword.
- `bool` — boolean type, accepts the literals `true` / `false`.
- `string` — string type, paired with `string_lit` tokens (`"..."`).
- `list` — list type keyword.

These are the only types currently accepted as variable declaration prefixes and as function return-type prefixes on `func` declarations.

---

## Implemented literal forms

- Integer literals — `0`, `42`, etc. (`integer_lit` token).
- String literals — `"hello"` (`string_lit` token).
- Char literals — `'a'` (`char_lit` token, single character).
- Boolean literals — `true`, `false` (`bool_lit` token).

---

## Implemented operators

Arithmetic (handled in `parseTerm` / `parseExpr`):

- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division (tokenized as `solidus`)

Comparison (handled in `parseComparison`):

- `==` equal
- `!=` not equal
- `<`  less than
- `<=` less than or equal
- `>`  greater than
- `>=` greater than or equal

The AST traverser prints each binary expression with its operator.

---

## Not implemented yet

The type keywords below are part of the planned type system but are **not** currently recognized as variable/function type prefixes by the parser:

- `float`
- `double`
- `void` (the function tokenizer hard-codes `void` as a fallback return type when no explicit type is provided, but `void` is not a valid `VarStmt` prefix and is not a first-class token keyword).
- `char` (the tokenizer recognizes `char_lit` but `char` is not yet a variable type keyword).
- `short`
- `long long`

Other type-related features that are **not implemented yet**:

- Composite / user-defined object types.
- Maps.
- Casting and type coercion beyond what the parser already accepts.
- Type checking across `BinaryExpr` operands (the parser does not currently validate that both sides of an operator share a type).
