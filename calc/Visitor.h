// ============================================
// Visitor.h
// ============================================
// Этот файл содержит все классы визиторов:
//   1. Visitor        - абстрактный базовый класс (интерфейс)
//   2. PrintVisitor   - визитор для вывода выражений
//   3. CalcVisitor    - визитор для вычисления выражений
//
// Почему всё в одном файле?
//   - Так меньше файлов (один вместо трёх-четырёх)
//   - Все визиторы в одном месте, удобно смотреть
//   - Легче подключать (один #include "Visitor.h")
// ============================================

#pragma once

#include <iostream>    // Для std::cout (PrintVisitor)
#include <map>         // Для std::map (CalcVisitor)
#include <string>      // Для std::string
#include <stdexcept>   // Для std::runtime_error (исключения)

// Forward declarations (объявляем, что эти классы существуют)
// Полные определения находятся в Expr.h
class Number;
class BiOperation;
class Variable;
class Assign;


// ============================================================
// ЧАСТЬ 1: АБСТРАКТНЫЙ КЛАСС VISITOR (ИНТЕРФЕЙС)
// ============================================================
// Содержит чисто виртуальные методы для каждого типа выражения.
// Все конкретные визиторы (PrintVisitor, CalcVisitor) наследуются от него
// и реализуют эти методы.
//
// Зачем нужен Visitor Pattern?
//   Позволяет добавлять новые операции к классам, не меняя сами классы.
//   Например, чтобы добавить операцию "вычисление", мы не трогаем классы
//   Number, Variable, BiOperation, Assign — просто создаём новый визитор.
// ============================================================

class Visitor {
public:
    // Виртуальный деструктор — обязателен для полиморфного удаления
    // Если его не будет, при delete Visitor* удалятся только поля Visitor,
    // а поля наследника останутся неудалёнными.
    virtual ~Visitor() = default;

    // Методы для каждого типа выражения.
    // = 0 означает "чисто виртуальный" — наследники ОБЯЗАНЫ его реализовать.
    virtual int visitNumber(Number* num) = 0;          // что делать с числом?
    virtual int visitBiOperation(BiOperation* op) = 0; // что делать с операцией?
    virtual int visitVariable(Variable* var) = 0;      // что делать с переменной?
    virtual int visitAssign(Assign* assign) = 0;       // что делать с присваиванием?
};


// ============================================================
// ЧАСТЬ 2: PRINT VISITOR (ДЛЯ ВЫВОДА ВЫРАЖЕНИЙ)
// ============================================================
// PrintVisitor обходит дерево выражений и печатает его в консоль
// в удобочитаемом виде.
//
// Пример: для выражения (10 + 20) + 30 выведет: ((10 + 20) + 30)
//
// Особенности:
//   - Все методы возвращают 0 (не используется, но нужно для совместимости)
//   - Использует рекурсивный обход: для вывода операндов вызывает accept()
//   - Для бинарных операций добавляет скобки для ясности
// ============================================================

class PrintVisitor : public Visitor {
public:
    // Число → печатаем его значение
    // Пример: Number(5) → выведет "5"
    virtual int visitNumber(Number* num) override {
        std::cout << num->getVal();
        return 0;
    }

    // Бинарная операция → печатаем в виде (лево оператор право)
    // Пример: BiOperation('+', num(10), num(20)) → выведет "(10 + 20)"
    virtual int visitBiOperation(BiOperation* op) override {
        std::cout << "(";
        op->left()->accept(this);   // печатаем левую часть (рекурсивно)
        std::cout << " " << op->getOp() << " ";
        op->right()->accept(this);  // печатаем правую часть (рекурсивно)
        std::cout << ")";
        return 0;
    }

    // Переменная → печатаем её имя
    // Пример: Variable("a") → выведет "a"
    virtual int visitVariable(Variable* var) override {
        std::cout << var->getName();
        return 0;
    }

    // Присваивание → печатаем "имя = выражение"
    // Пример: Assign("a", num(5)) → выведет "a = 5"
    virtual int visitAssign(Assign* assign) override {
        std::cout << assign->getVarName() << " = ";
        assign->getValue()->accept(this);
        return 0;
    }
};


// ============================================================
// ЧАСТЬ 3: CALC VISITOR (ДЛЯ ВЫЧИСЛЕНИЯ ВЫРАЖЕНИЙ)
// ============================================================
// CalcVisitor обходит дерево выражений и вычисляет результат.
//
// Особенности:
//   - Хранит таблицу переменных (сейчас std::map, потом заменим на AVL-дерево)
//   - При вычислении переменной ищет её значение в таблице
//   - При присваивании сохраняет значение в таблице
//   - Поддерживает операции +, -, *, /
//   - Проверяет деление на ноль и инициализацию переменных
// ============================================================

class CalcVisitor : public Visitor {
private:
    // ХРАНИЛИЩЕ ПЕРЕМЕННЫХ
    // Пока используем std::map (встроенная структура).
    // ЭТАП 1 (сейчас): std::map
    // ЭТАП 4 (потом): заменим на AVL-дерево или хеш-таблицу
    std::map<std::string, int> variables;

public:
    // Число → возвращаем его значение
    // Пример: Number(5) → вернёт 5
    virtual int visitNumber(Number* num) override {
        return num->getVal();
    }

    // Бинарная операция:
    //   1. Вычисляем левую часть
    //   2. Вычисляем правую часть
    //   3. Применяем оператор
    //   4. Возвращаем результат
    virtual int visitBiOperation(BiOperation* op) override {
        int left = op->left()->accept(this);   // вычисляем левый операнд
        int right = op->right()->accept(this); // вычисляем правый операнд

        switch (op->getOp()) {
        case '+':
            return left + right;

        case '-':
            return left - right;

        case '*':
            return left * right;

        case '/':
            if (right == 0) {
                throw std::runtime_error("Error: Division by zero!");
            }
            return left / right;

        default:
            throw std::runtime_error("Error: Unknown operator!");
        }
    }

    // Переменная:
    //   1. Ищем её значение в таблице
    //   2. Если не нашли → ошибка
    //   3. Иначе возвращаем значение
    virtual int visitVariable(Variable* var) override {
        std::string name = var->getName();

        auto it = variables.find(name);
        if (it == variables.end()) {
            throw std::runtime_error("Error: Variable not initialized: " + name);
        }

        return it->second;
    }

    // Присваивание:
    //   1. Вычисляем правую часть
    //   2. Сохраняем значение в таблице по имени переменной
    //   3. Возвращаем значение
    virtual int visitAssign(Assign* assign) override {
        int val = assign->getValue()->accept(this);      // вычисляем значение
        variables[assign->getVarName()] = val;           // сохраняем
        return val;                                      // возвращаем
    }

    // Выводит все переменные и их значения в консоль.
    // Формат: "имя = значение"
    void printVariables() const {
        for (const auto& [name, val] : variables) {
            std::cout << name << " = " << val << std::endl;
        }
    }

    // Очищает таблицу переменных.
    // Нужно для переиспользования визитора между разными вычислениями.
    void clearVariables() {
        variables.clear();
    }
};