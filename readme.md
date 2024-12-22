# 4LP1 - A Practice Project for the _Computer Architecture_ Course

This repository contains a simple "4 in a Line" (Connect 4) game implemented partially in C and partially in x86-64 Assembly. It is developed as a practice assignment for the subject _Computer Architecture_.

## Contents

- **4LP1c.c**: C program that manages the overall game logic and calls the assembly subroutines.
- **4LP1.asm**: Assembly file containing low-level subroutines (register management, printing, cursor movement, etc.) that integrate with the C code.

## How to Compile

1. **Assemble** the `.asm` file:

   ```bash
   yasm -f elf64 -g dwarf2 4LP1.asm -o 4LP1.o
   ```

2. **Compile** and **link** with the C code:

   ```bash
   gcc -no-pie -mincoming-stack-boundary=3 -o 4LP1 -g 4LP1.o 4LP1c.c
   ```

## How to Run

After a successful build, simply execute the resulting program:

```bash
./4LP1
```

## Notes

- 4LP1c-es.c must not be modified as per the assignment requirements.
- 4LP1-es.asm contains the core assembly routines.
- This practice demonstrates how to mix C and Assembly, focusing on how global variables and functions are shared between the two languages.
