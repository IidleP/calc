#include "Visitor.h"
#include "Expr.h"


int PrintVisitor::visitNumber(Number* num) {
    std::cout << num->getVal(); 
    return 0;
}

int PrintVisitor::visitBiOperation(BiOperation* op) {
    std::cout << "(";                  
    op->left()->accept(this);          
    std::cout << " " << op->getOp() << " "; 
    op->right()->accept(this);         
    std::cout << ")";
    return 0;
}

int PrintVisitor::visitVariable(Variable* var) {
    std::cout << var->getName();
    return 0;
}

int PrintVisitor::visitAssign(Assign* assign) {
    std::cout << assign->getVarName() << " = ";
    assign->getValue()->accept(this);           
    return 0;
}


int EvalVisitor::visitNumber(Number* num) {
    return num->getVal();
}

int EvalVisitor::visitBiOperation(BiOperation* op) {
    int left = op->left()->accept(this);
    int right = op->right()->accept(this);

    switch (op->getOp()) {
    case '+': return left + right;
    case '-': return left - right;
    case '*': return left * right;
    case '/':
        if (right == 0) throw std::runtime_error("Division by zero!");
        return left / right;
    default: throw std::runtime_error("Unknown operator!");
    }
}

int EvalVisitor::visitVariable(Variable* var) {
    std::string name = var->getName();
    auto it = variables.find(name);
    if (it == variables.end()) {
        throw std::runtime_error("Variable not initialized: " + name);
    }
    return it->second;
}

int EvalVisitor::visitAssign(Assign* assign) {
    int val = assign->getValue()->accept(this);
    variables[assign->getVarName()] = val;
    return val;
}

void EvalVisitor::printVariables() const {
    for (auto& pair : variables) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }
}

void EvalVisitor::clearVariables() {
    variables.clear();
}