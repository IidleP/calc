#include <iostream>
#include <string>
#include "antlr4-runtime.h"
#include "CalcLexer.h"
#include "CalcParser.h"
#include "Expr.h"
#include "Visitor.h"
#include "ASTBuilder.h"

using namespace antlr4;

int main() {
    std::string line;
    PrintVisitor printer;
    EvalVisitor calculator;
    ASTBuilder builder;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "q") break;
        if (line.empty()) continue;
        if (line.back() != ';') {
            line += ';';
        }



        ANTLRInputStream input(line);
        CalcLexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        CalcParser parser(&tokens);
        auto tree = parser.prog();

        if (parser.getNumberOfSyntaxErrors() > 0) {
            std::cout << "Syntax error, skip\n";
            continue;
        }


        builder.visitProg(tree);
        Expr* root = builder.getResult();


        if (root) {
            std::cout << "  ";
            root->accept(&printer);
            std::cout << "\n";
            try {
                root->accept(&calculator);
            }
            catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << "\n";
            }
        }
        builder.clearResult();

        std::cout << "-----------\n";
        calculator.printVariables();
        std::cout << "-----------\n";
    }

    std::cout << "\nRESULT\n";
    calculator.printVariables();
}