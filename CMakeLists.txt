cmake_minimum_required(VERSION 3.27)
project(LAB_PROJECT_C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-ansi -pedantic -Wall -g)


include_directories(.)

add_executable(assembler
        src/assembler.c
        include/assembler.h
        src/preAsmblr.c
        include/preAsmblr.h
        src/firstPass.c
        include/firstPass.h
        src/syntaxAnalyzer.c
        include/syntaxAnalyzer.h
        include/dataStructuresAndConstants.h
        src/secondPass.c
        include/secondPass.h
        src/error.c
        include/error.h
        src/dataConversion.c
        include/dataConversion.h
        src/initializationFunctions.c
        include/initializationFunctions.h
        src/listFunctions.c
        include/listFunctions.h
        src/fileFunctions.c
        include/fileFunctions.h
        tests/testFunctions.c
        tests/testFunctions.h
)
