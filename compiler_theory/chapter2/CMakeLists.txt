cmake_minimum_required(VERSION 3.14)
project(calc)

set(CMAKE_CXX_STANDARD 14)

add_executable(calc main.cpp Lexer.cpp Lexer.h Token.cpp Token.h TokenNode.cpp TokenNode.h Parser.cpp Parser.h Runner.cpp Runner.h Symbol.cpp Symbol.h Emitter.cpp Emitter.h Opcode.cpp Opcode.h)