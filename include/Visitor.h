#pragma once
#include <iostream>
#include <map>     
#include <string>
#include <stdexcept>

class Number;
class BiOperation;
class Variable;
class Assign;



class Visitor {
public:
    virtual ~Visitor() = default;
    virtual int visitNumber(Number* num) = 0;
    virtual int visitBiOperation(BiOperation* op) = 0;
    virtual int visitVariable(Variable* var) = 0;
    virtual int visitAssign(Assign* assign) = 0;
};



class PrintVisitor : public Visitor {
public:
    virtual int visitNumber(Number* num) override;
    virtual int visitBiOperation(BiOperation* op) override;
    virtual int visitVariable(Variable* var) override;
    virtual int visitAssign(Assign* assign) override;
};


class EvalVisitor : public Visitor {
private:
    std::map<std::string, int> variables;
public:
    virtual int visitNumber(Number* num) override;
    virtual int visitBiOperation(BiOperation* op) override;
    virtual int visitVariable(Variable* var) override;
    virtual int visitAssign(Assign* assign) override;

    void printVariables() const; 
    void clearVariables();       
};