#pragma once

#include <string>

#include "SignalData.h"
#include "InterpreterStack.h"

class Rule {  
    public: 
        Rule() = default;
        Rule(const Rule&) = delete;
        Rule(Rule&&) = delete;

        SignalData execute(InterpreterState& interpreter){
            return executeImpl(interpreter);          
        }

        std::string getName() const { return getNameImpl(); }
    
    private:
        virtual SignalData executeImpl(InterpreterState& interpreter) = 0;
        virtual std::string getNameImpl() const = 0;

      
};