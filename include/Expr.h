#pragma once          
#include "Visitor.h"  
#include <string>     


// Узлы
class Expr {
public:
    virtual ~Expr() = default;
    virtual int accept(Visitor* v) = 0; 
};


// Число
class Number : public Expr {
    int val; 
public:
    Number(int _val); 
    int getVal() const; 
    virtual int accept(Visitor* v) override;
};



// Переменная
class Variable : public Expr {
    std::string name;
public:
    Variable(const std::string& _name);
    std::string getName() const; 
    virtual int accept(Visitor* v) override;
};


// Операции
class BiOperation : public Expr {
    char op;     
    Expr* l;
    Expr* r;
public:
    BiOperation(char _op, Expr* _l, Expr* _r);
    Expr* left() const; 
    Expr* right() const; 
    char getOp() const;  
    virtual int accept(Visitor* v) override;
};


// Присваивание
class Assign : public Expr {
    std::string varName; 
    Expr* value;         
public:
    Assign(const std::string& name, Expr* val);
    std::string getVarName() const;
    Expr* getValue() const;
    virtual int accept(Visitor* v) override;
};


// Точка с запятой
class Semicolon : public Expr {
    Expr* left;
    Expr* right;
public:
    Semicolon(Expr* _left, Expr* _right);
    Expr* getLeft() const;
    Expr* getRight() const;
    virtual int accept(Visitor* v) override;
};



// Создание
Expr* num(int val);
Expr* var(const std::string& name);
Expr* binOp(char op, Expr* left, Expr* right);
Expr* assign(const std::string& name, Expr* val);
Expr* semicolon(Expr* left, Expr* right);
