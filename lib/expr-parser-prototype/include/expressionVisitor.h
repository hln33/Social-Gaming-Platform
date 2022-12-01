#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <iterator>
#include <any>
#include "bliniAST.h"

class ExpressionVisitor : public ASTVisitor {
    public:
    ExpressionVisitor(){}

    void visit(NumberConstant& number) override {
        this.result = number;
    }
    void visit(EqExpression& expr) override {
        expr.getLeft().evaluate(*this);
        std::any tmp1 = this.result;
        expr.getRight().evaluate(*this);
        std::any tmp2 = this.result;

        this.result = tmp1 == tmp2;
    }
    void visit(NeqExpression& expr) override {
        expr.getLeft().evaluate(*this);
        std::any tmp1 = this.result;
        expr.getRight().evaluate(*this);
        std::any tmp2 = this.result;

        this.result = tmp1 != tmp2;
    }
    void visit(NotExpression& expr) override {
        expr.getExpr().evaluate(*this);
        std::any tmp = this.result;

        this.result = !tmp;
    }
    void visit(GtExpression& expr) override {
        expr.getLeft().evaluate(*this);
        std::any tmp1 = this.result;
        expr.getRight().evaluate(*this);
        std::any tmp2 = this.result;

        this.result = tmp1 > tmp2;
    } 
    
    void visit(LtExpression& expr) override {
        expr.getLeft().evaluate(*this);
        std::any tmp1 = this.result;
        expr.getRight().evaluate(*this);
        std::any tmp2 = this.result;

        this.result = tmp1 < tmp2;
    }
    void visit(DotExpression& expr) override {
        expr.getLeft().evaluate(*this);
        expr.getProperty().evaluate(*this);
    }
    void visit(DotProperty& expr) {
    }
    void visit(IndexExpression& expr) override {
        expr.getListExpr().evaluate(*this);
        expr.getIndexExpr().evaluate(*this);
    }
    void visit(SingleVariable& expr) override {
    }
    void visit(MethodArgs& margs) override {

        if (margs.begin() != margs.end()) {
            margs.begin()->get()->evaluate(*this);

            for (auto it = std::next(margs.begin()); it != margs.end(); it++) {
                it->get()->evaluate(*this);
            }
        } else {
        }
        
    }
    void visit(MethodCall& expr) override {
        expr.getArgs().evaluate(*this);
    }
    private:
        std::any result;
};