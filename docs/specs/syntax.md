# IvoryScript 1.0.0 — Syntax

Language type: Compiled (bytecode compilation, not AOT)

Purpose: General-purpose language for algorithms, applications, and developer tooling. Implemented in C++ with a recursive-descent parser producing an AST, which is compiled to custom bytecode executed by a VM.

---

## File-format specifiers (FFS)

FFS are special tags placed at the top of a source file to influence parse/compile ordering and behavior. They are evaluated during parsing into the AST.

Example (placed at the top of the file):
```
@Ordered
```

Behavior notes:
- By default, top-level declarations are collected before other top-level statements; top-level code serves as the implicit entry point (no explicit `main()` required).
- `@Ordered` forces the file to be parsed and compiled in source order (useful when top-level order matters).

---

## Top-level entry point

- The top-level body of the file is considered the program entry point.
- Declarations (functions, objects, variables) are parsed before executing top-level statements unless `@Ordered` is specified.

---

## Basic constructs

Return statement:
```
return (expression);
```

Function declaration:
```
func <return-type> <name>(<param-list>) {
    <body>
}
```
- Example:
```
func int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```

Variable declaration:
- Given that IvoryScript is **strictly typed**, it is required that you provide a type name while declaring variables, an example is provided below:
```
type name = value;
int name = 5;
```
Alternatively, you can use the default constructor, outlined below:

```
type name(value);
int count(5);
```

- If you prefer your original form, replace with the language-specific syntax you already use.

---

## Comments

- Line comment:
```
// this is a single-line comment
```
- Block comment:
```
/* multi-line
   comment */
```

---

## Types (examples — adapt as needed)

- Primitive: `int`, `float`, `bool`, `string`, `void`, `double`, `long`
- Composite: lists, maps, user-defined objects

---

## Control flow (examples)

- If Statement:
```
if (cond) {
    // true-branch
} else {
    // false-branch
}
```
- For Loop:
```
for (initialization given )
```

- While Loop:
```
while (cond) {
    // body
}
```