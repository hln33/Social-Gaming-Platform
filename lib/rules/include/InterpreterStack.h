#pragma once

#include <memory>
#include <stack>
#include <utility>
#include <cassert>

#include "SignalData.h"

//////////////////////////////////////////////////////////////////////
// @FAKE                                                            //
//////////////////////////////////////////////////////////////////////
class SymbolTable{};                                                //
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


// forward declar
class Iter;
class InterpreterState;


using RuleIterPointer = std::unique_ptr<Iter>;

//////////////////////////////////////////////////////////////////////
// Iter
// iter is a bookmark for the game state
//////////////////////////////////////////////////////////////////////
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



//////////////////////////////////////////////////////////////////////
// InterpreterState
//////////////////////////////////////////////////////////////////////
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