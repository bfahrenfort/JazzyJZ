# JazzyJZ
## Students
DO NOT USE ANY OF MY CODE. I am not responsible for any academic integrity violations you may incur.

## Description
The JazzyJZ compiler system is a language translator for the procedural language Java Zero. 

It supports the modern language concepts of variables, constants, arrays, nested if statements, nested while statements, and recursive procedure calls. 

The compiler generates detailed error messages from invalid scans and parses, and performs limited semantic analysis to determine symbol correctness. It also performs limited optimization, eliminating symbols in favor of literals where possible, and attempting to minimize temporaries.

## Dependencies
C and C++ environment: Any modern GCC

Haskell environment: GHC >= 8.10

Assembler: NASM >=2.0
## Download and Compilation
```
git clone <repo link> --recursive
cd JazzyJZ
chmod +x build.sh
./build.sh
```
## Use
`bin/jazzy <source file>.jz [-o <output name>]` 

For compilation, see `build.sh`. 
