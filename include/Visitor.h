#pragma once
#include <iostream>
//#include <map>     
#include "AVLTree.h"
#include <string>
#include <stdexcept>

class Number;
class BiOperation;
class Variable;
class Assign;
class Semicolon;


// Визитор, обработчики узлов
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual int visitNumber(Number* num) = 0;
    virtual int visitBiOperation(BiOperation* op) = 0;
    virtual int visitVariable(Variable* var) = 0;
    virtual int visitAssign(Assign* assign) = 0;
    virtual int visitSemicolon(Semicolon* sem) = 0;
};


// Вывод
class PrintVisitor : public Visitor {
public:
    virtual int visitNumber(Number* num) override;
    virtual int visitBiOperation(BiOperation* op) override;
    virtual int visitVariable(Variable* var) override;
    virtual int visitAssign(Assign* assign) override;
    virtual int visitSemicolon(Semicolon* sem) override;
};

// Вычисление и хранение map
class EvalVisitor : public Visitor {
private:
    //std::map<std::string, int> variables;
    AVLTree<std::string, int> variables;
public:
    virtual int visitNumber(Number* num) override;
    virtual int visitBiOperation(BiOperation* op) override;
    virtual int visitVariable(Variable* var) override;
    virtual int visitAssign(Assign* assign) override;
    virtual int visitSemicolon(Semicolon* sem) override;

    void printVariables() const; 
    void clearVariables();       
};