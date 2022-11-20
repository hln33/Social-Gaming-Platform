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

        //object evaluate();
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

/***
 Extend
{ "rule": "extend",
  "target": << variable name of a list to extend with another list >>
  "list": << either an immediate list or a name of a constant variable or list >>
}

THIS CODE MIGHT BE VOLATILE

Mathew and I discussed about this and it might not be the best way to do it. Template is here so we can accept different types of vectors.
As Mathew suggested it might be smarter to a general object class as the vector type. {Ex: vector<GameObject> array} This game object would act as a wrapper for the rest of our types.
We can figure it out on Monday.

-Shawn
*/
template <typename T>
    class ExtendRule : public Rule {
    {
    private:
        std::vector<T> list;
        std::vector<T> target;
    public:
        ExtendRule :(std::vector<T>& target, std::vector<T>& list) : target{target}, list{list} {};
        ~ExtendRule :();

        std::string getNameImpl() const override{
            return "extend";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            target.insert(target.end(), list.begin(), list.end());
            return SignalData{Signal::COMPLETE};
        }
    };
};

/***
Reverse
{ "rule": "reverse",
  "list": << variable name of a list to reverse >>
}

THIS CODE MIGHT BE VOLATILE

Mathew and I discussed about this and it might not be the best way to do it. Template is here so we can accept different types of vectors.
As Mathew suggested it might be smarter to a general object class as the vector type. {Ex: vector<GameObject> array} This game object would act as a wrapper for the rest of our types.
We can figure it out on Monday.

-Shawn
*/
template <typename T>
    class ReverseRule : public Rule {
        {
    public:
        ReverseRule(std::vector<T>& list) : list{list} { };
    private:
        std::vector<T> list;

        std::string getNameImpl() const override{
            return "reverse";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            std::reverse(list.begin(), list.end());
            return SignalData{Signal::COMPLETE};
        }
    };
};

/**
{ "rule": "shuffle",
  "list": << variable name of a list to shuffle >>
}
*/
template <typename T>
    class ShuffleRule : public Rule {
        {
    public:
        ShuffleRule(std::vector<T>& list) : list{list} { };
    private:
        std::vector<T> list;

        std::string getNameImpl() const override{
            return "suffle";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            std::random_shuffle(list.begin(), list.end());
            return SignalData{Signal::COMPLETE};
        }
    };
};

/***
Sort
{ "rule": "sort",
  "list": << variable name of a list to sort >>,

  OPTIONALLY
  "key": << Attribute of list elements to use for comparison.
            Only valid when the list contains maps. >>

  list = [{name:player1, score:10}]
  list = [10]
}
*/
template <typename T>
    class SortRule : public Rule {
    {
    public:
        SortRule(std::vector<T>& list) : list{list} { };
        SortRule(std::vector<T>& list, std::vector<T>& key) : list{list}, key{key} { };
    private:
        std::vector<T> list;
        std::vector<T> key;

        std::string getNameImpl() const override{
            return "sort";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{

            if(key.empty()){
                std::sort(list.begin(), list.end());
            }
            else{
               //hold place if key is occupied
            }
            return SignalData{Signal::COMPLETE};
        }
    };
};

/**
{ "rule": "deal",
  "from": << variable name of a list to deal from >>,
  "to": << variable name of a list or list attribute to deal to >>,
  "count": << number of elements to deal >>
}
*/
template <typename T>
    class DealRule : public Rule {
        {
    public:
        DealRule(std::vector<T>& from, std::vector<T>& to, std::size_t count) : 
            from{from},
            to{to},
            count{count}
        { };
    private:
        std::vector<T> from;
        std::vector<T> to;
        std::size_t count;

        std::string getNameImpl() const override{
            return "deal";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            assert(count <= from.size());
            
            std::copy_n(from.begin(), count, std::back_inserter(to));
            return SignalData{Signal::COMPLETE};
        }
    };
};

/**
 Discard
{ "rule": "discard",
  "from": << variable name of a list to discard from >>,
  "count": << number of elements to discard >>
}

THIS DISCARDS THE LAST N ELEMENTS OF THE LIST, WE MIGHT BE INTERPRETING THIS WRONG

*/

template <typename T>
    class DiscardRule : public Rule {
        {
    public:
        DiscardRule(std::vector<T>& list, int count) : list{list}, count{count} { };
    private:
        std::vector<T> list;
        int count;

        std::string getNameImpl() const override{
            return "discard";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            list.erase(list.end() - count, list.end()); //erases from size - n to size
            return SignalData{Signal::COMPLETE};
        }
    };
};

/**
 Add
{ "rule": "add",
  "to": << variable name of an integer to add to >>,
  "value": << integer literal or name of a variable or constant containing the value to add >>,
}
*/
class AddRule: public Rule {
    
    public:
        AddRule(Expression& to, Expression& value) : to{to}, value{value} { };
    private:
        Expression to;
        Expression value;

        std::string getNameImpl() const override{
            return "add";
        }

        SignalData executeImpl(InterpreterState& interpreter) override{
            to.evaluate(AddVisitor{});
            return SignalData{Signal::COMPLETE};
        }
    };

/***
Timer
{ "rule": "timer",
  "duration": << seconds >>,
  "mode": "exact" OR "at most" OR "track",
  "rules": [
    << sequence of rules to execute with respect to the timer.
       An "at most" timer will stop executing and time out input requests.
       An "exact" timer will pad the execution time to the given duration.
       A "track" timer will continue executing but set a flag. >>
  ],

  OPTIONAL
  "flag": << variable that evaluates to false when a "track" timer has not expired and false afterward. >>
}
*/


template <typename T>
    class Timer: public Rule {
        {
        public:
            Timer(int seconds, std::string mode, std::vector<Rule>& rules) : seconds{seconds}, mode{mode}, rules{rules} { };
            Timer(int seconds, std::string mode, std::vector<Rule>& rules, T& flag) : seconds{seconds}, mode{mode}, rules{rules}, flag{flag} { };

        private:
            int seconds;
            std::string mode;
            std::vector<Rule> rules;
            T flag;
            
            std::string getNameImpl() const override{
                return "timer";
            }
            
            SignalData executeImpl(InterpreterState& interpreter) override{
                return SignalData{Signal::COMPLETE};
            }
    };
};