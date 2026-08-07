# IvoryScript 1.0.0 - Specification

## Standard Library
By far the most vast library in the language, included using the following syntax:

``use (standl);``

Where the parenthesis surrounding ``standl`` are based on convention but are not strictly required.

``print()`` - A standard library function that takes any streamable type (section ex.) and outputs it to the standard output stream, by default this flushes the output stream and makes a new line.
``docs()`` - A standard library function that is mainly used in the Ivory REPL feature. Outputs an IvoryScript documentation menu to the standard output stream.
``rin()`` - A standard library function that takes input from the console/terminal and stores it into a string type.
``rout()`` - A standard library function that takes any streamable type (section ex.) and outputs it to the main stream without flushing or making a new line by default.
