cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)

# Папки с ANTLR runtime (подставьте свои пути)
set(ANTLR_RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ANTLR/antlr4-cpp-runtime-4.13.2-source)
set(ANTLR_JAR ${CMAKE_CURRENT_SOURCE_DIR}/antlr-4.13.2-complete.jar)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Генерация парсера из Calc.g4
add_custom_command(
    OUTPUT ${GENERATED_DIR}/CalcLexer.cpp ${GENERATED_DIR}/CalcParser.cpp ${GENERATED_DIR}/CalcBaseVisitor.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
    COMMAND java -jar ${ANTLR_JAR} -Dlanguage=Cpp -no-listener -visitor -o ${GENERATED_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Calc.g4
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Calc.g4
    COMMENT "Generating ANTLR parser"
)

set(ANTLR_GEN_SOURCES
    ${GENERATED_DIR}/CalcLexer.cpp
    ${GENERATED_DIR}/CalcParser.cpp
    ${GENERATED_DIR}/CalcBaseVisitor.cpp
)

# Исполняемый файл
add_executable(calculator
    src/main.cpp
    src/Expr.cpp
    src/Visitor.cpp
    src/ASTBuilder.cpp
    ${ANTLR_GEN_SOURCES}
)

# Подключаем заголовочные пути
target_include_directories(calculator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GENERATED_DIR}
    ${ANTLR_RUNTIME_DIR}/runtime/src
)

target_link_directories(calculator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ANTLR/antlr-runtime/lib
)

target_link_libraries(calculator PRIVATE antlr4-runtime)

# Копируем DLL только на Windows (коротко)
if(WIN32)
    add_custom_command(TARGET calculator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/ANTLR/antlr-runtime/bin/antlr4-runtime.dll
            $<TARGET_FILE_DIR:calculator>
    )
endif()