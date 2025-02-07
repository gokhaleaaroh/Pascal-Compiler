# My Pascal Compiler

This is an attempt at a Pascal compiler in C. The spec is more or less based on the Free Pascal manual (omitting the object pascal keywords). The current plan is to compile to LLVM IR to reap the benefits of the existing LLVM optimizations. 

## Lexer

The lexer is currently a (painstakingly) manually written DFA using if statements.

## Parser

WIP

## Code Gen

WIP
