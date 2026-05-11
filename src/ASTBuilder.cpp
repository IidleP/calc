#include "ASTBuilder.h"

ASTBuilder::ASTBuilder()
    : result(nullptr), tempLeft(nullptr), tempRight(nullptr),
    tempOp(0), tempVarName(""), tempValue(nullptr) {
}

antlrcpp::Any ASTBuilder::visitProg(CalcParser::ProgContext* ctx) {
    allAssigns.clear();
    for (auto* assignCtx : ctx->assign()) {
        visit(assignCtx);
    }
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitAssign(CalcParser::AssignContext* ctx) {
    tempVarName = ctx->ID()->getText(); 
    visit(ctx->expr());                 
    tempValue = result;

    result = assign(tempVarName, tempValue);
    allAssigns.push_back(result);
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitMulDiv(CalcParser::MulDivContext* ctx) {
    visit(ctx->expr(0));      
    tempLeft = result;        
    visit(ctx->expr(1));      
    tempRight = result;       
    tempOp = ctx->op->getText()[0]; 
    result = binOp(tempOp, tempLeft, tempRight);
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitAddSub(CalcParser::AddSubContext* ctx) {
    visit(ctx->expr(0)); 
    tempLeft = result;
    visit(ctx->expr(1)); 
    tempRight = result;
    tempOp = ctx->op->getText()[0];
    result = binOp(tempOp, tempLeft, tempRight);
    return nullptr;
}


antlrcpp::Any ASTBuilder::visitInt(CalcParser::IntContext* ctx) {
    int value = std::stoi(ctx->INT()->getText());
    result = num(value);
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitVar(CalcParser::VarContext* ctx) {
    std::string name = ctx->ID()->getText();
    result = var(name); 
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitParens(CalcParser::ParensContext* ctx) {
    visit(ctx->expr()); 
    return nullptr;    
}