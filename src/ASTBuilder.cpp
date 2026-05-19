#include "ASTBuilder.h"

ASTBuilder::ASTBuilder()
    : result(nullptr), tempLeft(nullptr), tempRight(nullptr),
    tempOp(0), tempVarName(""), tempValue(nullptr) {
}

// ID = expr
antlrcpp::Any ASTBuilder::visitProg(CalcParser::ProgContext* ctx) {
    Expr* tree = nullptr;
    for (auto* assignCtx : ctx->assign()) {
        visit(assignCtx);
        if (tree == nullptr) {
            tree = result;
        }
        else {
            tree = semicolon(tree, result);
        }
    }
    result = tree;
    return nullptr;
}

// ID = expr
antlrcpp::Any ASTBuilder::visitAssign(CalcParser::AssignContext* ctx) {
    tempVarName = ctx->ID()->getText(); 
    visit(ctx->expr());                 
    tempValue = result;

    result = assign(tempVarName, tempValue);

    return nullptr;
}

// expr ('*'|'/') expr
antlrcpp::Any ASTBuilder::visitMulDiv(CalcParser::MulDivContext* ctx) {
    visit(ctx->expr(0));      
    tempLeft = result;        
    visit(ctx->expr(1));      
    tempRight = result;       
    tempOp = ctx->op->getText()[0]; 
    result = binOp(tempOp, tempLeft, tempRight);
    return nullptr;
}

// expr('+' | '-') expr
antlrcpp::Any ASTBuilder::visitAddSub(CalcParser::AddSubContext* ctx) {
    visit(ctx->expr(0)); 
    tempLeft = result;
    visit(ctx->expr(1)); 
    tempRight = result;
    tempOp = ctx->op->getText()[0];
    result = binOp(tempOp, tempLeft, tempRight);
    return nullptr;
}

// Число
antlrcpp::Any ASTBuilder::visitInt(CalcParser::IntContext* ctx) {
    int value = std::stoi(ctx->INT()->getText());
    result = num(value);
    return nullptr;
}

// Переменная
antlrcpp::Any ASTBuilder::visitVar(CalcParser::VarContext* ctx) {
    std::string name = ctx->ID()->getText();
    result = var(name); 
    return nullptr;
}

antlrcpp::Any ASTBuilder::visitParens(CalcParser::ParensContext* ctx) {
    visit(ctx->expr()); 
    return nullptr;    
}