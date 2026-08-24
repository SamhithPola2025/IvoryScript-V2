# IvoryScript 1.0.0 — Syntax

Language type: Compiled (bytecode compilation, not AOT)

Purpose: General-purpose language for algorithms, applications, and developer tooling. Implemented in C++ with a recursive-descent parser producing an AST, which is compiled to custom bytecode executed by a VM.

---

## Implemented syntax (current parser)

The sections below describe only the syntax that the current tokenizer and parser actually understand. Each construct here has been verified against `ivoryscript/modules/tokenizer/tokenizer.cpp` and `ivoryscript/modules/parser/parser.cpp`.

### Comments

- Line comment:
```
// this is a single-line comment
```
- Block comment:
```
/* multi-line
   comment */
```
Both forms are stripped by the tokenizer before the parser ever sees them.

### Top-level entry point

- The top-level body of the file is the program entry point.
- Function declarations are recognized as statements at the top level; they are added to the symbol table under `scope::Global`.
- A trailing `return 0;` at the top level is allowed but optional.

### Variable declarations

IvoryScript is **strictly typed**, so every variable declaration requires a type prefix:
```
type name = value;
int count = 0;
```
Variable types currently accepted as prefixes (see `docs/specs/types.md`): `int`, `long`, `bool`, `string`, `list`.

> Note: the constructor form `int count(5);` shown in older revisions of this document is **not implemented yet**. Only the `type name = value;` form is parsed today.

### Functions

```
func <return-type> <name>() {
    <body>
}
```
Current limitations:

- The return type **must** be one of the recognized type keywords (`int`, `long`, `bool`, `string`, `list`).
- Function **parameters are not supported yet** — the parser explicitly errors out if anything other than `)` follows the opening `(`.
- Function bodies are parsed in a `scope::Local` symbol-table scope.

Example:
```
func int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```
> The recursive call above is illustrative only; function calls are **not implemented yet**, so this exact snippet will not parse today. The body parsing itself works.

### Control flow

If statement:
```
if (cond) {
    // true-branch
} else {
    // false-branch
}
```
The `else` branch is parsed into `IfStmt::elseBranch` (currently not surfaced by the AST traverser).

While loop:
```
while (cond) {
    // body
}
```

For loop (C-style, three-part header):
```
for (init; cond; increment;) {
    // body
}
```
All three header parts must be present and separated by `;`. The init is wrapped in an `ExprStmt`, the condition is parsed via `parseComparison`, and the increment is parsed via `parseExpr`.

### Return statement
```
return (expression);
```
`return` must be followed by an expression and a terminating `;`. The expression is parsed via `parseExpr`.

### Print statement
```
print(expr);
```
`print` is a keyword recognized directly by the tokenizer. The argument is parsed via `parseExpr` and stored in a `PrintStmt` node.

### Expression statements
Bare expressions are allowed as statements when followed by `;`:
```
5 + 6 * 8;
(2 * 3) + 2;
```
This is parsed into an `ExprStmt` wrapping the resulting expression.

### Operators

Arithmetic (binary, left-associative in `parseExpr`):

- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division

Comparison (in `parseComparison`):

- `==` equal
- `!=` not equal
- `<`  less than
- `<=` less than or equal
- `>`  greater than
- `>=` greater than or equal

Parenthesized expressions are supported via `parsePrimary`.

### Literals

- Integer: `42`
- String: `"hello"`
- Char: `'a'`
- Boolean: `true`, `false`

### AST output

When a file is passed to the compiled `ivscript` binary, the current pipeline ends at the AST. The traverser prints a debug view of the program, not bytecode execution. So running a program today prints an AST, not program output.

---

## Not implemented yet

The features below are described here so the spec stays complete, but they have **not** been wired into the parser yet. They are documented in older drafts of this spec; do not rely on them in real programs until they are marked implemented above.

### File-format specifiers (FFS)

```
@Ordered
```
FFS are planned tags placed at the top of a source file to influence parse/compile ordering. The parser does not currently recognize `@`-prefixed directives; passing `@Ordered` today will likely be tokenized as an identifier and produce a parse error.

### Default-constructor variable form
```
type name(value);
int count(5);
```
The constructor-style declaration is not implemented yet. Only `type name = value;` is parsed.

### Function parameters
```
func int add(int a, int b) {
    return a + b;
}
```
The parser explicitly errors out if a parameter list is provided. Functions must currently be declared as `func <type> <name>() { ... }`.

### Function calls

Calling a function by name (e.g. `factorial(n - 1)`) is not implemented yet. The parser only handles integer/string/char/boolean literals and parenthesized sub-expressions inside expression positions.

### User-defined objects, lists, maps

Composite types are planned but not implemented yet.

### Imports / module system

There is no module system yet. See `docs/specs/standl.md` for the planned `use (standl);` syntax.

### Bytecode execution

The custom bytecode emitter and VM are not implemented yet. The current binary ends at AST printing.
