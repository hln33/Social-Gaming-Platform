#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <iterator>

#include "bliniAST.h"


class PrettyPrinter : public ASTVisitor {
public:
    void visit(NumberConstant& number) override {
        std::cout << "Number(" << number.getValue() << ")";
    }
    void visit(EqExpression& expr) override {

        std::cout << "EqualTo(";

        expr.getLeft().evaluate(*this);

        std::cout << ",";

        expr.getRight().evaluate(*this);
        
        std::cout << ")";
    }
    void visit(NeqExpression& expr) override {
        std::cout << "NotEqualTo(";

        expr.getLeft().evaluate(*this);

        std::cout << ",";

        expr.getRight().evaluate(*this);
        
        std::cout << ")";
    }
    void visit(NotExpression& expr) override {
        std::cout << "Not(";

        expr.getExpr().evaluate(*this);

        std::cout << ")";
    }
    void visit(GtExpression& expr) override {
        std::cout << "GreaterThan(";

        expr.getLeft().evaluate(*this);

        std::cout << ",";

        expr.getRight().evaluate(*this);
        
        std::cout << ")";
    }
    void visit(DotExpression& expr) override {
        std::cout << "Dot(";

        expr.getLeft().evaluate(*this);

        std::cout << ",";

        expr.getProperty().evaluate(*this);
        
        std::cout << ")";
    }
    void visit(DotProperty& expr) {
        std::cout << "Property(" << expr.getPropertyName() << ")";
    }
    void visit(IndexExpression& expr) override {
        std::cout << "IndexOf(";

        expr.getListExpr().evaluate(*this);

        std::cout << ",";

        expr.getIndexExpr().evaluate(*this);
        
        std::cout << ")";
    }
    void visit(SingleVariable& expr) override {
        std::cout << "Var(" << expr.getName() << ")";
    }
    void visit(MethodArgs& margs) override {
        std::cout << "Args(";

        if (margs.begin() != margs.end()) {
            margs.begin()->get()->evaluate(*this);
            // (*(*(margs.begin()))).evaluate(*this);

            for (auto it = std::next(margs.begin()); it != margs.end(); it++) {
                std::cout << ",";
                it->get()->evaluate(*this);
            }
        } else {
            std::cout << "VOID";
        }
        
        std::cout << ")";
    }
    void visit(MethodCall& expr) override {
        std::cout << "Method(" << expr.getName() << ",";

        expr.getArgs().evaluate(*this);
        
        std::cout << ")";
    }
};


