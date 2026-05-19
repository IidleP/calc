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

int PrintVisitor::visitSemicolon(Semicolon* sem) {
    sem->getLeft()->accept(this);
    std::cout << " ; ";
    sem->getRight()->accept(this);
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
        if (right == 0) throw - 1;
        return left / right;
    default: 
        throw -1;
    }
}

int EvalVisitor::visitVariable(Variable* var) {
    int* val = variables.find(var->getName());
    if (!val) throw - 1;
    return *val;
}

int EvalVisitor::visitAssign(Assign* assign) {
    int val = assign->getValue()->accept(this);
    variables.insert(assign->getVarName(), val);
    return val;
}

int EvalVisitor::visitSemicolon(Semicolon* sem) {
    sem->getLeft()->accept(this);
    return sem->getRight()->accept(this);
}

void EvalVisitor::printVariables() const {
    variables.printInOrder();
}

void EvalVisitor::clearVariables() {
    variables.clear();
}