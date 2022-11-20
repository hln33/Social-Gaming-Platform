#pragma once

#include <memory>
#include <string>
#include <cassert>


#include "RuleInterface.h"
#include "InterpreterStack.h"
#include "RuleIter.h"


// contains the list of rules
class RuleList : public RuleListInterface{
public:
    RuleList() { }
    RuleList(std::unique_ptr<Rule> rule) {
        rules.push_back(std::move(rule));
    }

private:
    std::vector<std::unique_ptr<Rule>> rules;

    RuleIterPointer getIterImpl() override { 
        return std::make_unique<RuleListIter>(RuleListIter{rules.begin(), rules.end()});
    }

    void appendRuleImpl(std::unique_ptr<Rule> rule) override {
        rules.push_back(std::move(rule));
    }

    RuleIter beginImpl() override {
        return rules.begin();
    }
    
    RuleIter endImpl() override {
        return rules.end();
    }
};


/**
{ "rule": "foreach",
  "list": <<list, list expression, or name of a list object>>,
  "element": << name for list element object within the rules below >>
  "rules": [
    << Rules to execute on every element of the given list >>
  ]
}
*/

// TODO: parse the expression string
struct Condition{
    std::string expression;
    Condition() {}
    Condition(std::string ex){this->expression = ex;}
    Condition(const Condition&other){this->expression = other.expression;};
    Condition(Condition&&) = delete;
};


// using ObjectGenerator = std::vector<Object>::const_iterator;//custom variables
class Expression {
    public: 
        std::vector<Object> getResult(){}
};




class ForEachRule : public Rule{
    public:
        ForEachRule(Expression ex, RuleList& rules) : listExpr{ex} , ruleList{rules}
        { }


    private:
        RuleList ruleList;
        Expression listExpr;
        std::string getNameImpl() const override{
            return "foreach";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            // TODO: implement this
            auto result = listExpr.getResult();
            interpreter.push(std::make_unique<ForeachIter>(result.begin(), result.end(), &ruleList));
            return SignalData{Signal::COMPLETE};
        }
};

/**
{ "rule": "loop",

  "until": << Condition that may fail >>,
  OR
  "while": << Condition that may fail >>,

  "rules": [
    << Rules to execute on every element of the given list >>
  ]
}
*/


class LoopRule : public Rule{
    public:
        LoopRule(RuleList& rules) : ruleList{rules}
        { }

    private:
        RuleList ruleList;
        Condition condition;

        std::string getNameImpl() const override{
            return "loop";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            // TODO: implement this
            // interpreter.push(std::make_unique<LoopIter>(listExpr.getResult(), &ruleList));
            
            interpreter.push(std::make_unique<LoopIter> (&ruleList));
            return SignalData{Signal::COMPLETE};
        }
};


/**
{ "rule": "inparallel",
  "rules": [
    << Rules to execute *in parallel* rather than *in sequence* >>
  ]
}
*/

class InparallelRule : public Rule {
public:
    InparallelRule(RuleList& rules) : ruleList{rules} { }

private:

    RuleList ruleList;

    std::string getNameImpl() const override{
        return "inparallel";
    }

    SignalData executeImpl(InterpreterState& interpreter) override{
        // TODO: implement this
        interpreter.push(std::make_unique<InparallelIter>(&ruleList));
        return SignalData{Signal::COMPLETE};
    }
};


/**
{ "rule": "parallelfor",
  "list": <<name of a list object>>,
  "element": << name for list element object within the rules below >>,
  "rules": [
    << Rules to execute on every element of the given list. The list is executed sequentially, but each element is processed "in parallel">>
  ]
}
*/

class ParallelforRule : public Rule {
public:
    ParallelforRule(Expression& listExpr, Expression& element, RuleList& rules) :
        listExpr{listExpr},
        elementDecl{element},
        ruleList{rules}
    { }

private:
    Expression listExpr;
    Expression elementDecl;
    RuleList ruleList;
    
    std::string getNameImpl() const override{
        return "parallelfor";
    }

    SignalData executeImpl(InterpreterState& interpreter) override{
        // TODO: implement this

        // TODO: declare the element onto symbol table
        // symboltable.addScope()
        
        interpreter.push(std::make_unique<ParallelforIter>(listExpr.getResult(), &ruleList));
        return SignalData{Signal::COMPLETE};
    }
};

/**
Guard
        { "condition": << Boolean guard for rules >>,
            "rules": [ << Rules to execute when the value and case match >> ]
        },
*/

class GuardRule : public Rule {
public:
    GuardRule(Expression& boolCondition, RuleList& rules) :
        condition{boolCondition},
        ruleList{rules}
    { }
private:
    Expression condition;
    RuleList ruleList;

    std::string getNameImpl() const override{
        return "guard";
    }

    SignalData executeImpl(InterpreterState& interpreter) override{
        // if condition is true
        interpreter.push(std::make_unique<RuleListIter>(ruleList.begin(), ruleList.end()));
        // otherwise do nothing
        return SignalData{Signal::COMPLETE};
    }
};

struct CaseList {
public:
    // Clarification: for when and switch, they contain cases, each case contains a condition tied with a RuleList, which is a GuardRule 
    std::vector<GuardRule>  cases;
    
    CaseList() = default;
    CaseList(const CaseList&) = delete;
    CaseList(CaseList&&) = delete;

    using GuardIter = std::vector<GuardRule>::iterator;
    GuardIter begin() {
        return cases.begin();
    }

    GuardIter end() {
        return cases.end();
    }
};


// { "rule": "when",
//    "cases": [
//         { "condition": << Boolean guard for rules >>,
//            "rules": [ << Rules to execute when the value and case match >> ]
//         },
//         ...
//     ]
// }


class WhenRule : public Rule{
    public:
        WhenRule(CaseList& cases){}


    private:
        std::string ruleName = "when";
        CaseList cases;
        
        std::string getNameImpl() const override{
            return "when";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            // TODO: implement this
            for (auto& rule : cases) {
                rule.execute(interpreter);
            }
            return SignalData{Signal::COMPLETE};
        }
};


// {   "rule": "switch",
//     "value": << value to switch upon >>,
//     "list": << name of a constant list of allowable values >>,
//     "cases": [{ "case": << value >>, "rules": [ << Rules to execute when the value and case match>> ] },
// ...
//     ]   
// }

class Switch : public Rule{
    public:
        Switch(){}
        
        std::string getNameImpl() const override{
            return "switch";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            // TODO: implement this
            for (auto& guard : cases){
                guard.execute(interpreter);
            }
            
            SignalData signalData();
            return SignalData{Signal::COMPLETE};
        }


    private:
        std::string ruleName = "switch";
        CaseList cases;
};


/**
{ "rule": "extend",
  "target": << variable name of a list to extend with another list >>
  "list": << either an immediate list or a name of a constant variable or list >>
}
*/



