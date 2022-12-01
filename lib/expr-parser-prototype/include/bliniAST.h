#pragma once

#include <variant>
#include <string>
#include <vector>
#include <list>
#include <memory>

#include <iostream>

// @note
// we use raw pointers here because bison only recognize POD types.
// Also the pointers are dynamically allocated on the heap so dont pass 
// a pointer that is not on the heap.
// ** unless someone knows how to convert pointer to unique_ptr then we can do that instead **
// For example:
// BAD
// std::string str = "str";
// SingleVariable ast = SingleVarialbe{&str}; <-- DONT DO THIS
// OK
// std::string* str = new std::string{"str"};
// SingleVariable ast = SingleVarialbe{str}; <-- THIS IS OK

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


// using BliniTypeContainer = std::variant<int, bool, std::string>;


// enum BliniDataType {
//     NUMBER,
//     BOOL,
//     STRING,
//     LIST,
//     OBJECT,
// };


class BliniAST;
class SentinelNode;
class BooleanConstant;
class NumberConstant;
class EqExpression;
class NeqExpression;
class GeqExpression;
class LeqExpression;
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
    virtual void visit(BooleanConstant& number) = 0;
    virtual void visit(NumberConstant& number)  = 0;
    virtual void visit(EqExpression& number)    = 0;
    virtual void visit(NeqExpression& number)   = 0;
    virtual void visit(GeqExpression& number)   = 0;
    virtual void visit(LeqExpression& number)   = 0;
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
    virtual ~BliniAST() { }
    virtual void evaluate(ASTVisitor& visitor) = 0;

    // remove later
    virtual void print() { std::cout << "valid\n"; }
};

// sentinel
class SentinelNode : public BliniAST {
public:
    virtual ~SentinelNode() { }

    void evaluate(ASTVisitor& visitor) override { }

    void print() override { std::cout << "NOT valid\n"; }
};

// constants
class NumberConstant : public BliniAST {
public:
    virtual ~NumberConstant() { delete str; }

    NumberConstant(std::string* num) : 
        str{num} 
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
    std::string* str;
};

class BooleanConstant : public BliniAST {
public:
    virtual ~BooleanConstant() { }

    BooleanConstant(bool val) : 
        value{val} 
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool getValue() const {
        return this->value;
    }

private:
    bool value;
};

// bool expression
class EqExpression : public BliniAST {
public:
    virtual ~EqExpression() { delete left; delete right; }

    EqExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

class NeqExpression : public BliniAST {
public:
    virtual ~NeqExpression() { delete left; delete right; }

    NeqExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

class GeqExpression : public BliniAST {
public:
    virtual ~GeqExpression() { delete left; delete right; }

    GeqExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

class LeqExpression : public BliniAST {
public:
    virtual ~LeqExpression() { delete left; delete right; }

    LeqExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

class NotExpression : public BliniAST {
public:
    virtual ~NotExpression() { delete expr; }

    NotExpression(BliniAST* expr) :
        expr{expr}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    BliniAST& getExpr() const {
        return *expr;
    }

private:
    BliniAST* expr;
};

class GtExpression : public BliniAST {
public:
    virtual ~GtExpression() { delete left; delete right; }

    GtExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

class LtExpression : public BliniAST {
public:
    virtual ~LtExpression() { delete left; delete right; }

    LtExpression(BliniAST* left, BliniAST* right) :
        left{left},
        right{right}
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
    BliniAST* left;
    BliniAST* right;
};

// dot expression
class DotExpression : public BliniAST {
public:
    virtual ~DotExpression() { delete left; delete property; }

    DotExpression(BliniAST* left, BliniAST* property) :
        left{left},
        property{property}
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
    BliniAST* left;
    BliniAST* property;
};

class DotProperty : public BliniAST {
public:
    virtual ~DotProperty() { delete name; }

    DotProperty(std::string* name) : 
        name{name}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string& getPropertyName() const {
        return *name;
    }

private:
     std::string* name;
};

class IndexExpression : public BliniAST {
public:
    virtual ~IndexExpression() { delete list; delete index; }

    IndexExpression(BliniAST* listExpr, BliniAST* indexExpr) :
        list{listExpr},
        index{indexExpr}
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
    BliniAST* list;
    BliniAST* index;
};

class SingleVariable : public BliniAST {
public:
    virtual ~SingleVariable() { delete name; }

    SingleVariable(std::string* name) : 
        name{name}
    { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::string& getName() const {
        return *name;
    }

private:
    std::string* name;
};

// method call
class MethodArgs : public BliniAST {
public:
    virtual ~MethodArgs() { for (auto x : args) { delete x; } }

    MethodArgs() { }

    void evaluate(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    void addFront(BliniAST* arg) {
        this->args.push_front(arg);
    }

    bool empty() {
        return args.empty();
    }

    using ArgsType = std::list<BliniAST*>;

    ArgsType::iterator begin() { return this->args.begin(); }
    ArgsType::iterator end()   { return this->args.end(); }

private:
    ArgsType args;
};

class MethodCall : public BliniAST {
public:
    virtual ~MethodCall() { delete name; delete args; }

    MethodCall(std::string* name, BliniAST* args) :
        name{name},
        args{args}
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
    std::string* name;
    BliniAST* args;
};



// visitors


// class Evaluator : public ASTVisitor {
// public:

//     void visit(NumberConstant& number) override {
//         this->result = number.getValue();
//     }

//     // TODO: fill this in later
//     void visit(EqExpression& number) override { }
//     void visit(NeqExpression& number) override { }
//     void visit(NotExpression& number) override { }
//     void visit(GtExpression& number) override { }
//     void visit(DotExpression& number) override { }
//     void visit(DotProperty& number) override { }
//     void visit(IndexExpression& number) override { }
//     void visit(SingleVariable& number) override { }
//     void visit(MethodCall& number) override { }
//     void visit(MethodArgs& number) override { }

//     BliniTypeContainer getResult();

// private:
//     BliniTypeContainer result;
// };

