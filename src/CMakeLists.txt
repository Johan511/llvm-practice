project(ToyCompiler)

add_library(compiler lexer.cpp parser.cpp)

add_executable(toy main.cpp)
target_link_libraries(toy compiler)

