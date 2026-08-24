# IvoryScript

> **Note**: This project was submitted to the Macondo YSWS (you > > ship - we ship) program, which is part of [Hack Club](hackclub.> com).

> **Note**: This project is currently an AST generator, not a full compiler. See below for more information.

## The premise
IvoryScript is a compiled programming language that is made entirely in C++ compiled with CMAKE. The choice to make this in C++ was purely based off my comfort in the language along with speed and performance reasons.

**The current version of IvoryScript is not complete.** What I mean to say by this is that while the ship itself is complete, the compiler does not currently execute bytecode and actually execute your program, but rather prints a complete AST of your program.

All code that you write must adhere to the languages syntax and rules, to understand those in more depth, please refer to the `docs/specs` folder. If you want to check this out without writing your own programs, I left a few sample programs in the `examples` folder that are ready to run without any syntax errors.

## Installation and instructions for use
To use IvoryScript, download the executable from <placeholder here for github releases page> and run it in your terminal with your second argument any file given it is in the IvoryScript file format that you wish to run. An example is written below:

``./ivoryscriptV2 test.ivc``

You can also run ivoryscript in REPL (read-eval-print-loop) mode, which is in a sense a kind of interactive mode where you can type in normal ivoryscript code line by line and get an output.

Example:

```
./ivscript

IvoryScript 1.0.0. (Sun Aug 23 20:04:06 2026) REPL (read-eval-print-loop) mode.
Type "docs();" for more information. Type "exit();" to exit the REPL.

int a = 5;
if (a == 5) {
    print("hello world");
}

exit();

Currently generated AST:
  Exit
  Variable: a
    Number: 5
  If
    Condition:
      Binary operator: ==
        Identifier: a
        Number: 5
    Then:
      Print
        String: "hello world"
  Exit
```

![alt text](image-1.png)
![alt text](image.png)
![alt text](image.png)