#pragma once

#include <variant>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include <iostream>

// visitor pattern

/**
 * 
 * return value of an expression can be:
 * - bool
 * - number
 * - string
 * - list
 *      - list of anything
 * - json object
 * 
*/


using BliniTypeContainer = std::variant<int, bool, std::string>;


enum BliniDataType {
    NUMBER,
    BOOL,
    STRING,
    LIST,
    OBJECT,
};


class BliniAST;
class SentinelNode;
class NumberConstant;
class EqExpression;
class NeqExpression;
class NotExpression;
class GtExpression;
class LtExpression;
class DotExpression;
class DotProperty;
class IndexExpression;
class SingleVariable;
class MethodArgs;
class MethodCall;

class ASTVisitor {
public:
    virtual void visit(NumberConstant& number)  = 0;
    virtual void visit(EqExpression& number)    = 0;
    virtual void visit(NeqExpression& number)   = 0;
    virtual void visit(NotExpression& number)   = 0;
    virtual void visit(GtExpression& number)    = 0;
    virtual void visit(LtExpression& number)    = 0;
    virtual void visit(DotExpression& number)   = 0;
    virtual void visit(DotProperty& number)     = 0;
    virtual void visit(IndexExpression& number) = 0;
    virtual void visit(SingleVariable& number)  = 0;
    virtual void visit(MethodArgs& number)      = 0;
    virtual void visit(MethodCall& number)      = 0;
};


class BliniAST {
public:
    virtual void evaluate(ASTVisitor& visitor) = 0;

    // remove later
    virtual void print() { std::cout << "valid\n"; }
};

// sentinel
class SentinelNode : public BliniAST {
public:
    void evaluate(ASTVisitor& visitor) override { }

    void print() override { std::cout << "NOT valid\n"; }
};

// constants
class NumberConstant : public BliniAST {
public:
    NumberConstant(std::string* num) : 
        str{std::unique_ptr<std::string>(num)} 
    {
        this->value = std::stoi(*str);
    }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int getValue() const {
        return this->value;
    }

private:
    int value;
    std::unique_ptr<std::string> str;
};

// bool expression
class EqExpression : public BliniAST {
public:
    EqExpression(BliniAST* left, BliniAST* right) :
        left{std::unique_ptr<BliniAST>(left)},
        right{std::unique_ptr<BliniAST>(right)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getLeft() const {
        return *left;
    }
    BliniAST& getRight() const {
        return *right;
    }

private:
    std::unique_ptr<BliniAST> left;
    std::unique_ptr<BliniAST> right;
};

class NeqExpression : public BliniAST {
public:
    NeqExpression(BliniAST* left, BliniAST* right) :
        left{std::unique_ptr<BliniAST>(left)},
        right{std::unique_ptr<BliniAST>(right)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getLeft() const {
        return *left;
    }
    BliniAST& getRight() const {
        return *right;
    }

private:
    std::unique_ptr<BliniAST> left;
    std::unique_ptr<BliniAST> right;
};

class NotExpression : public BliniAST {
public:
    NotExpression(BliniAST* expr) :
        expr{std::unique_ptr<BliniAST>(expr)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getExpr() const {
        return *expr;
    }

private:
    std::unique_ptr<BliniAST> expr;
};

class GtExpression : public BliniAST {
public:
    GtExpression(BliniAST* left, BliniAST* right) :
        left{std::unique_ptr<BliniAST>(left)},
        right{std::unique_ptr<BliniAST>(right)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getLeft() const {
        return *left;
    }
    BliniAST& getRight() const {
        return *right;
    }

private:
    std::unique_ptr<BliniAST> left;
    std::unique_ptr<BliniAST> right;
};

class LtExpression : public BliniAST {
public:
    LtExpression(BliniAST* left, BliniAST* right) :
        left{std::unique_ptr<BliniAST>(left)},
        right{std::unique_ptr<BliniAST>(right)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getLeft() const {
        return *left;
    }
    BliniAST& getRight() const {
        return *right;
    }

private:
    std::unique_ptr<BliniAST> left;
    std::unique_ptr<BliniAST> right;
};

// dot expression
class DotExpression : public BliniAST {
public:
    DotExpression(BliniAST* left, BliniAST* property) :
        left{std::unique_ptr<BliniAST>(left)},
        property{std::unique_ptr<BliniAST>(property)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getLeft() const {
        return *left;
    }
    BliniAST& getProperty() const {
        return *property;
    }

private:
    std::unique_ptr<BliniAST> left;
    std::unique_ptr<BliniAST> property;
};

class DotProperty : public BliniAST {
public:
    DotProperty(std::string* name) : 
        name{std::unique_ptr<std::string>(name)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string& getPropertyName() const {
        return *name;
    }

private:
     std::unique_ptr<std::string> name;
};

class IndexExpression : public BliniAST {
public:
    IndexExpression(BliniAST* listExpr, BliniAST* indexExpr) :
        list{std::unique_ptr<BliniAST>(listExpr)},
        index{std::unique_ptr<BliniAST>(indexExpr)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getListExpr() const {
        return *list;
    }
    BliniAST& getIndexExpr() const {
        return *index;
    }

private:
    std::unique_ptr<BliniAST> list;
    std::unique_ptr<BliniAST> index;
};

class SingleVariable : public BliniAST {
public:
    SingleVariable(std::string* name) : 
        name{std::unique_ptr<std::string>(name)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string& getName() const {
        return *name;
    }

private:
     std::unique_ptr<std::string> name;
};

// method call
class MethodArgs : public BliniAST {
public:

    MethodArgs() { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    void addFront(BliniAST* arg) {
        this->args.push_front(std::unique_ptr<BliniAST>(arg));
    }

    bool empty() {
        return args.empty();
    }

    using ArgsType = std::list<std::unique_ptr<BliniAST>>;

    ArgsType::iterator begin() { return this->args.begin(); }
    ArgsType::iterator end() { return this->args.end(); }

private:
    ArgsType args;
};

class MethodCall : public BliniAST {
public:
    MethodCall(std::string* name, BliniAST* args) :
        name{std::unique_ptr<std::string>(name)},
        args{std::unique_ptr<BliniAST>(args)}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string& getName() const {
        return *name;
    }

    BliniAST& getArgs() const {
        return *args;
    }

private:
    std::unique_ptr<std::string> name;
    std::unique_ptr<BliniAST> args;
};



// visitors


class Evaluator : public ASTVisitor {
public:

    void visit(NumberConstant& number) override {
        this->result = number.getValue();
    }

    // TODO: fill this in later
    void visit(EqExpression& number) override { }
    void visit(NeqExpression& number) override { }
    void visit(NotExpression& number) override { }
    void visit(GtExpression& number) override { }
    void visit(DotExpression& number) override { }
    void visit(DotProperty& number) override { }
    void visit(IndexExpression& number) override { }
    void visit(SingleVariable& number) override { }
    void visit(MethodCall& number) override { }
    void visit(MethodArgs& number) override { }

    BliniTypeContainer getResult();

private:
    BliniTypeContainer result;
};

