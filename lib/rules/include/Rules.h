#pragma once

#include <stack>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>

#include "SignalData.h"

class InterpreterState;

// iter is a bookmark for the game state

class Iter {
public:
    bool isReachedEnd() { return isReachedEndImpl(); }
    void next() { nextImpl(); }
    SignalData execute(InterpreterState& interpreter) { return executeImpl(interpreter); }

private:
    virtual bool isReachedEndImpl() = 0;
    virtual void nextImpl() = 0;
    virtual SignalData executeImpl(InterpreterState&) = 0;
};

using RuleIterPointer = std::unique_ptr<Iter>;


/////////////////////////////////////////////////////////////////////////////////
// @FAKE - need to implement later
/////////////////////////////////////////////////////////////////////////////////
// all rules are just collection of rules so instead of naming it rulelist i 
// name it rule. i think thats clear enough

class Object {}; // the type of the user defined variables

// every rule needs to have a getIter function so it can create a bookmark for itself
// this needs to be implemented in another file
// all rules need to derive from this interface plz
class RuleList {
public:
    RuleIterPointer getIter() { return nullptr; }
    SignalData execute(InterpreterState&) const { 
        return SignalData{Signal::NOSIGNAL, std::vector<Bundle>{}};
    }
};
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

using ObjectGenerator = std::vector<Object>::const_iterator;//custom variables


/////////////////////////////////////////////////////////////////////////////////
// @FAKE - need to implement later
/////////////////////////////////////////////////////////////////////////////////

class SymbolTable {}; // holds local variables
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class InterpreterState {
public:
    InterpreterState(RuleIterPointer instr) {
        push(std::move(instr));
    }

    void push(RuleIterPointer instr) {
        state.push(std::move(instr));
    }

    SignalData run() {
        SignalData commData;
        commData.signal = Signal::NOSIGNAL;
        while (!state.empty() && commData.signal != Signal::INTERRUPT) {

            // get the first instruction from the stack
            // assume no null iterators
            assert(state.top() != nullptr && "null pointer");

            Iter* instr = state.top().get();

            // assume no empty iterators
            assert(!instr->isReachedEnd() && "Empty iterator was not popped from stack");

            // run the instruction and increment to the next one
            SignalData commData = instr->execute(*this);
            instr->next();

            // if instruction iterator doesnt have anymore stuff to run we can pop it
            if (instr->isReachedEnd()) {
                state.pop();
            }
        }
        return commData;
    }

private:
    std::stack<RuleIterPointer> state;
    // @note 
    // in addition i think interpreterstate should hold it's own local variables so the rules
    // can access them. the table should handle scope variables too
    SymbolTable localVariables;
};


/////////////////////////////////////////////////////////////////////////////////
// @NOTE
// we dont need bookmarks for the rules that can complete in single call 
// to execute because it does not need to store any tracking information
/////////////////////////////////////////////////////////////////////////////////


// keeps track of location in rule list
class RuleListIter : public Iter {
public:
    RuleListIter(std::vector<RuleList*>::const_iterator begin, std::vector<RuleList*>::const_iterator end) :
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
        return (*listIter)->execute(interpreter); // <-- call execute on the rule. 
        // the rule can modify interpreter state but it is constant
    }

    std::vector<RuleList*>::const_iterator listIter;
    std::vector<RuleList*>::const_iterator sentinel;
};

// we also need a bookmark for the foreach rule to track the iteration of the loop
class ForeachIter : public Iter {
public:
    ForeachIter(ObjectGenerator begin, ObjectGenerator end, RuleList* body) :
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
    RuleList* ruleList;
};

// we also need a bookmark for the loop rule to track the iteration of the loop
class LoopIter : public Iter {
public:
    LoopIter(RuleList* boolCondition, RuleList* body) :
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

    RuleList* condition;

    // rule-list generator
    RuleList* ruleList;
};

// * this is the 'rule trees' we talked about in the meeting
// * think of it as a list of InterpreterState objects because if
// 1 InterpreterState object represents 1 thread of execution
// then many should, in theory, represent *in parallel* executions

// we also need a bookmark for the inparallel rule to track the iterations because it 
// could take more than 1 iteration to complete this instruction
class InparallelIter : public Iter {
public:
    InparallelIter(std::vector<RuleList*>& ruleList)
    {
        // initialize parallel execution stacks
        std::transform(ruleList.begin(), ruleList.end(), std::back_inserter(parallelRules),
            [](RuleList* r) { return InterpreterState{r->getIter()}; }
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
    ParallelforIter(std::vector<Object>& itemList, RuleList* body)
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
