#include "Expr.h"


Number::Number(int _val) : val(_val) {} 
int Number::getVal() const { return val; } 

int Number::accept(Visitor* v) { return v->visitNumber(this); }




Variable::Variable(const std::string& _name) : name(_name) {}
std::string Variable::getName() const { return name; }
int Variable::accept(Visitor* v) { return v->visitVariable(this); }


BiOperation::BiOperation(char _op, Expr* _l, Expr* _r) : op(_op), l(_l), r(_r) {}
Expr* BiOperation::left() const { return l; }
Expr* BiOperation::right() const { return r; }
char BiOperation::getOp() const { return op; }
int BiOperation::accept(Visitor* v) { return v->visitBiOperation(this); }



Assign::Assign(const std::string& name, Expr* val) : varName(name), value(val) {}
std::string Assign::getVarName() const { return varName; }
Expr* Assign::getValue() const { return value; }
int Assign::accept(Visitor* v) { return v->visitAssign(this); }



Expr* num(int val) { return new Number(val); }
Expr* var(const std::string& name) { return new Variable(name); }
Expr* binOp(char op, Expr* left, Expr* right) { return new BiOperation(op, left, right); }
Expr* assign(const std::string& name, Expr* val) { return new Assign(name, val); }