#pragma once

#include <stack>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>

#include "SignalData.h"
#include "InterpreterStack.h"
#include "RuleInterface.h"

/////////////////////////////////////////////////////////////////////////////////
// @FAKE - need to implement later
/////////////////////////////////////////////////////////////////////////////////
// all rules are just collection of rules so instead of naming it rulelist i 
// name it rule. i think thats clear enough

class Object {}; // the type of the user defined variables
using ObjectGenerator = std::vector<Object>::const_iterator;//custom variables
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



// using RuleIterPointer = std::unique_ptr<Iter>;

// every rule needs to have a getIter function so it can create a bookmark for itself
// rulelist needs to derive from this interface
class RuleListInterface {
public:
    using RuleIter = std::vector<std::unique_ptr<Rule>>::iterator;

    RuleIterPointer getIter() { return getIterImpl(); }
    RuleIter begin() { return beginImpl(); }
    RuleIter end() { return endImpl(); }
    void appendRule(std::unique_ptr<Rule> rule) { appendRuleImpl(std::move(rule)); }

private:
    virtual RuleIterPointer getIterImpl() = 0;
    virtual void appendRuleImpl(std::unique_ptr<Rule> rule) = 0;
    virtual RuleIter beginImpl() = 0;
    virtual RuleIter endImpl() = 0;

};

/////////////////////////////////////////////////////////////////////////////////
// @NOTE
// we dont need bookmarks for the rules that can complete in single call 
// to execute because it does not need to store any tracking information
/////////////////////////////////////////////////////////////////////////////////


// keeps track of location in rule list
class RuleListIter : public Iter {
public:
    RuleListIter(std::vector<std::unique_ptr<Rule>>::iterator begin, std::vector<std::unique_ptr<Rule>>::iterator end) :
        listIter{begin},
        sentinel{end}
    { }

private:
    bool isReachedEndImpl() override {
        return listIter == sentinel;
    }

    void nextImpl() override {
        listIter++;
    }

    SignalData executeImpl(InterpreterState& interpreter) override {
        return (*listIter).get()->execute(interpreter); // <-- call execute on the rule. 
        // the rule can modify interpreter state but it is constant
    }

    std::vector<std::unique_ptr<Rule>>::iterator listIter;
    std::vector<std::unique_ptr<Rule>>::iterator sentinel;
};



class RuleList : public RuleListInterface{
public:
    RuleList() : rules{std::vector<std::unique_ptr<Rule>>{}} { }
    RuleList(const RuleList& r) {
        // need to copy the rule list if needed
    }
    RuleList(std::unique_ptr<Rule> rule) : 
        rules{std::vector<std::unique_ptr<Rule>>{}} 
    {
        rules.push_back(std::move(rule));
    }

private:
    std::vector<std::unique_ptr<Rule>> rules;

    RuleIterPointer getIterImpl() override { 
        return std::make_unique<RuleListIter>(rules.begin(), rules.end());
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

// we also need a bookmark for the foreach rule to track the iteration of the loop
class ForeachIter : public Iter {
public:
    ForeachIter(ObjectGenerator begin, ObjectGenerator end, RuleListInterface* body) :
        listIter{begin},
        sentinel{end},
        ruleList{body}
    { }

private:

    bool isReachedEndImpl() override {
        return listIter == sentinel;
    }

    void nextImpl() override {
        listIter++;
    }

    SignalData executeImpl(InterpreterState& interpreter) override {
        interpreter.push(ruleList->getIter());
        return SignalData{Signal::COMPLETE};
    }

    // list iterator
    // conditions:
    // - assume that the list is not modified inside the foreach rule
    ObjectGenerator listIter;
    ObjectGenerator sentinel;

    // rule-list generator
    RuleListInterface* ruleList;
};

// we also need a bookmark for the loop rule to track the iteration of the loop
class LoopIter : public Iter {
public:
    LoopIter(Rule* boolCondition, RuleListInterface* body) :
        condition{boolCondition},
        ruleList{body}
    { }

private:

    bool isReachedEndImpl() override {
        // @Vy need rules plz :-)
        // Visitor visitor;
        // condition->evaluate(visitor);

        // throw exception if vistor.result is not bool type

        // return visitor.result;
        return true;
    }

    void nextImpl() override { }

    SignalData executeImpl(InterpreterState& interpreter) override {
        interpreter.push(ruleList->getIter());
        return SignalData{Signal::COMPLETE};
    }

    Rule* condition;

    // rule-list generator
    RuleListInterface* ruleList;
};

// * this is the 'rule trees' we talked about in the meeting
// * think of it as a list of InterpreterState objects because if
// 1 InterpreterState object represents 1 thread of execution
// then many should, in theory, represent *in parallel* executions

// we also need a bookmark for the inparallel rule to track the iterations because it 
// could take more than 1 iteration to complete this instruction
class InparallelIter : public Iter {
public:
    InparallelIter(RuleListInterface& ruleList)
    {
        // initialize parallel execution stacks
        std::transform(ruleList.begin(), ruleList.end(), std::back_inserter(parallelRules),
            [](std::unique_ptr<Rule>& r) { 
                // create new rule list with 1 rule
                RuleList rules = RuleList{std::move(r)};
                // auto rules = RuleList{};

                return InterpreterState{rules.getIter()}; 
            }
        );
    }

private:

    bool isReachedEndImpl() override {
        return parallelRules.size() == 0;
    }

    void nextImpl() override { }

    SignalData executeImpl(InterpreterState& interpreter) override {
        std::vector<Bundle> batch;
        std::vector<std::list<InterpreterState>::iterator> completed;

        for (auto it = parallelRules.begin(); it != parallelRules.end(); it++) {
            SignalData commData = it->run();

            switch (commData.signal)
            {
            case Signal::COMPLETE:
                completed.push_back(it);
                break;

            case Signal::INTERRUPT: // pause for interuption
                batch.insert(batch.end(), commData.data.begin(), commData.data.end());
                break;
            
            default:
                break;
            }
        }

        // clear the completed
        for (auto x : completed) {
            parallelRules.erase(x);
        }

        // send batch request to clients up to interrupt rule
        if (!isReachedEnd()) {
            return {Signal::INTERRUPT, batch}; // send batch signaldatas -> includes globalmessage
        }

        return {Signal::COMPLETE, batch};
    }

    std::list<InterpreterState> parallelRules;
};

class ParallelforIter : public Iter {
public:
    ParallelforIter(std::vector<Object>& itemList, RuleListInterface* body)
    {
        // initialize parallel execution stacks
        std::transform(itemList.begin(), itemList.end(), std::back_inserter(parallelRules),
            [&body](auto x) { return InterpreterState{body->getIter()}; }
        );
    }
    ParallelforIter(std::vector<Object>&& itemList, RuleListInterface* body)
    {
        // initialize parallel execution stacks
        std::transform(itemList.begin(), itemList.end(), std::back_inserter(parallelRules),
            [&body](auto x) { return InterpreterState{body->getIter()}; }
        );
    }

private:

    bool isReachedEndImpl() override {
        return parallelRules.size() == 0;
    }

    void nextImpl() override { }

    SignalData executeImpl(InterpreterState& interpreter) override {
        std::vector<Bundle> batch;
        std::vector<std::list<InterpreterState>::iterator> completed; // keep track of completed execution to remove later

        for (auto it = parallelRules.begin(); it != parallelRules.end(); it++) {
            SignalData commData = it->run();

            switch (commData.signal)
            {
            case Signal::COMPLETE:
                completed.push_back(it);
                break;

            case Signal::INTERRUPT: // pause for interuption
                batch.insert(batch.end(), commData.data.begin(), commData.data.end());
                break;
            
            default:
                // this is an error
                break;
            }
        }

        // clear the completed
        for (auto x : completed) {
            parallelRules.erase(x);
        }

        // send batch request to clients up to interrupt rule
        if (!isReachedEnd()) {
            return {Signal::INTERRUPT, batch};
        }

        return {Signal::COMPLETE, batch};
    }

    std::list<InterpreterState> parallelRules;
};

// for global message we dont need to return immediately.
// just return signal::complete and add the message to the SignalData batch
