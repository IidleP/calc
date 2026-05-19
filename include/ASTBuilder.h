#pragma once
#include <string>
#include <vector>     
#include <antlr4-runtime.h>
#include "Expr.h"
#include "CalcBaseVisitor.h" 
#include "CalcParser.h"     


class ASTBuilder : public CalcBaseVisitor {
public:
    ASTBuilder();

    antlrcpp::Any visitProg(CalcParser::ProgContext* ctx) override;
    antlrcpp::Any visitAssign(CalcParser::AssignContext* ctx) override;
    antlrcpp::Any visitMulDiv(CalcParser::MulDivContext* ctx) override;
    antlrcpp::Any visitAddSub(CalcParser::AddSubContext* ctx) override;
    antlrcpp::Any visitInt(CalcParser::IntContext* ctx) override;
    antlrcpp::Any visitVar(CalcParser::VarContext* ctx) override;
    antlrcpp::Any visitParens(CalcParser::ParensContext* ctx) override;

    Expr* getResult() const { return result; }
    void clearResult() { result = nullptr; }

private:
    Expr* result = nullptr;          
    Expr* tempLeft = nullptr;        
    Expr* tempRight = nullptr;       
    char tempOp = 0;                 
    std::string tempVarName;         
    Expr* tempValue = nullptr;       
};