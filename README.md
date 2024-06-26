# Lab Assembler Project In C
## 📃Table of Contents📃
- [Grade](#Grade)
- [Introduction](#Introduction)
- [Usage](#Usage)
- [Code](#Code)
- [Limitations And Specifics](#Limitations-And-Specifics)
- [Documentation](#Documentation)
- [Author](#Author)

## 🏆Grade🏆
**Grade Received For The Project:  98/100**
## 🏢Introduction🏢
### This project, developed in C, as a final project for the "Lab in Systems Programming - 20465" 2024A course, part of the Computer Science degree at the Open University of Israel.
 It encompasses a comprehensive assembler system for a specific assembly language, with 4096 words in it's memory image. The assembler functions in several stages:
 1. Preprocessing: Initially, an assembly source code file is processed to expand macros, producing an intermediary ".am" file.
 2. Two-Pass Assembly: The system then executes a two-pass assembly process on the ".am" file, culminating in the generation of a machine code file (".ob"). Additionally, it creates:
 - A ".ent" file detailing the addresses of all entry points.
 - A ".ext" file listing the addresses of external variables.
This structure not only modularizes the assembly process but also ensures the generation of necessary metadata for linkage and execution.
## 💻Code💻
### The code is divided into several modules:
 1. Preprocessor: This module is responsible for expanding macros in the source code file.
 - The preprocessor reads the source code file line by line, identifying and storing all macro definitions inside a 2d pointer array, which is dynamically allocated. It simultaneously writes the source code to a temporary file, replacing macro definitions with their respective values.
 2. First Pass: This module is responsible for the first pass of the assembly process, generating a symbol table and an intermediate file.
 - The first pass reads the temporary file generated by the preprocessor, processing each line to identify and store all labels and their respective addresses in the symbol table (which is a dynamically allocated linked list). It also converts the first line of each instruction to a machine code representation. It fills an instruction and data arrays of fixed size (3,996 each) with the .data values, and first line instructions. It does this using a calculated IC and DC counters. It finds errors in the source code's syntax and notifies the user using "stderr" stream if any are found. 
 3. Second Pass: This module is responsible for the second pass of the assembly process, generating the machine code file and the entry and external files.
 - The second pass reads the source file again, processing each line to generate the machine code file. Afterwards, it generates the .ob, .ent, and .ext files. It also checks for additional errors in the source code and notifies the user if any are found, halting the assembly process and not generating the resulting files.
## ❗Limitations And Specifics❗
 - The assembler is designed to work with a specific assembly language, with a specific syntax and set of instructions.
 - The assembler is designed to work with a specific memory image size (4096 words, with 100 words reserved for the system. So- 3996 words of memory for data and instruction words in total).
 - The assembler is designed to work with a specific word size (14 bits, with 12 bits for data and instructions, and 2 bits for addressing modes.
 - The assembler is designed to work with macro declarations, with no limit the amount of macros / macro size (as long as it fits in the memory image).
 - The assembler is designed to work with infinite label declarations, with no limit to the amount of labels( as long as it fits in the memory image).
 - The assembler is only working with integers ranging from -2048 to 2047 (the maximum for 12 bits), due to the limitations of the 14 bit word size, with most instructions using 12 bits.
 - The assembler is designed to handle .define directives, which are used to define constant integers. The integers are stored in the symbol table and can be used in the source code. The upper limit for the integers is the maximum that was detailed earlier.
 - The assembler is designed to read .as files, with a specific syntax and set of directives and commands.
 - The assembler is designed to work with a specific set of directives and commands, and will only work with registers r0-r7.
## 📜Documentation📜
 The code is well documented, with each function and module having a detailed description of its purpose and functionality. Feel free to look, there's an explanation for pretty much everything.
## 🔧Usage🔧
 To use the assembler, follow these steps:
 1. Clone the repository.
 2. Compile the code using the provided makefile.
 3. Run the assembler, providing the path to the assembly source code file as an argument.
 4. The assembler will generate the necessary files in the same directory as the source code file.

## ✒️Author✒️
Omri Peretz
