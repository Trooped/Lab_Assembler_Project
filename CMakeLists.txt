cmake_minimum_required(VERSION 3.27)
project(. C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-ansi -pedantic -Wall -g)


include_directories(.)

add_executable(main
        main/main.c
        PreAsmblr/PreAsmblr.c
        PreAsmblr/PreAsmblr.h
        Assembler/firstPass.c
        Assembler/firstPass.h
        Assembler/assembler.c
        Assembler/assembler.h
        Assembler/utilities.c
        Assembler/utilities.h
        Assembler/syntaxAnalyzer.c
        Assembler/syntaxAnalyzer.h
        main/main.h
        Assembler/dataStructuresAndConstants.h
)
