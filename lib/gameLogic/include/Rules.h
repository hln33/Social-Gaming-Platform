#pragma once
#include<string>
#include<vector>
#include <variant>
#include <cctype>
#include <memory>
#include <algorithm>
#include <iostream>
#include <optional>

enum ExecutionStatus{
    SUCCESS, 
    FAIL,
    WAITING_FOR_INPUT
};

class RuleInterface {
    private:
        virtual ExecutionStatus executeImpl() = 0;
        
    public: 
        RuleInterface() = default;
        RuleInterface(const RuleInterface&) = delete;
        RuleInterface(RuleInterface&&) = delete;
        ExecutionStatus execute(){
            return executeImpl();          
        }
        virtual std::string getName() const = 0;
    
};
struct Condition{
    std::string expression;
    Condition() {}
    Condition(std::string ex){this->expression = ex;}
    Condition(const Condition&other){this->expression = other.expression;};
    Condition(Condition&&) = delete;
};

struct Rules {
    // Clarification: for foreach and parallel, they contain rules
    std::vector<std::unique_ptr<RuleInterface>> rules;
   
    Rules() {}
    Rules(const Rules&){};
    Rules(Rules&&) = delete;
};

struct Cases {
    // Clarification: for when and switch, they contain cases, each case contains a condition tied with some rules
    std::vector< std::unique_ptr<std::pair<Condition, Rules>> >  cases;

    Cases() = default;
    Cases(const Cases&) = delete;
    Cases(Cases&&) = delete;
};

class ForEachRule : public RuleInterface{
    public:
        ForEachRule(){}
        
        ForEachRule(const ForEachRule& other){ 
            this->element = other.element;
            this->listExpression = other.listExpression;
        }

        ForEachRule(std::string expression, std::string elem) : listExpression{expression}, element{elem} {}
        
        std::string getName() const override{
            return this->ruleName;
        }

        ExecutionStatus executeImpl() override{
            // TODO: implement this
            return ExecutionStatus::WAITING_FOR_INPUT;
        }

        void setListExpression(std::string str) {
            this->listExpression = str;
        }
        void setElement(std::string str){
            this->element = str;
        }
        
        void addForEachRule(std::unique_ptr<ForEachRule> foreachrule){
           rules_obj.rules.push_back(std::move(foreachrule));
        }

        RuleInterface* getRuleAt(size_t index){
            if (index < rules_obj.rules.size()){
                return rules_obj.rules.at(index).get();
            }
            return nullptr;
        }
    private:
        std::string ruleName = "foreach";
        std::string listExpression;
        std::string element; 
        Rules rules_obj;
};



class WhenRule : public RuleInterface{
    public:
        WhenRule(){}
        
        std::string getName() const override{
            return this->ruleName;
        }

        ExecutionStatus executeImpl() override{
            // TODO: implement this
            return ExecutionStatus::WAITING_FOR_INPUT;
        }
        
        void addCase(std::unique_ptr<std::pair<Condition, Rules>> _case){
            case_obj.cases.push_back(std::move(_case));
        }

        Condition getFirstCaseCondition(){
            return case_obj.cases.at(0).get()->first;   
        }
    private:
        std::string ruleName = "when";
        Cases case_obj;
};




// Comment out for now because it is not completed

//class RuleVariant{
//     public:
//         // Clarification: a vector that contains variants, each variant can either be rules or cases 
//         // E.g: FOREACH contains rules but WHEN contains cases,
//         std::variant< Rules, Cases > rulesOrCases; 
//     // public:
//         RuleVariant(Rules only_rule) : rulesOrCases{only_rule}
//         {
//             std::cout << "RuleVariant cons called" << "\n";
//         }

//         RuleVariant(Cases only_case) : rulesOrCases{only_case} 
//         {}

//         RuleVariant() = default;
//         RuleVariant(const RuleVariant& variant) = delete;
//         RuleVariant(RuleVariant&&) = delete;
//         RuleVariant& operator=(const RuleVariant& o) = delete;
//         RuleVariant& operator=(RuleVariant&&) = delete;
    
// };
