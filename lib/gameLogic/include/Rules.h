#pragma once
#include<string>
#include<vector>
#include <variant>
#include <cctype>
#include <memory>
#include <algorithm>
enum ExecutionStatus{
    SUCCESS, 
    FAIL 
};



class RuleInterface {
    private:
        virtual ExecutionStatus executeImpl() = 0;
        virtual std::string getName() const = 0;
    public: 
        RuleInterface() = default;
        RuleInterface(const RuleInterface&) = delete;
        RuleInterface(RuleInterface&&) = delete;
        ExecutionStatus execute(){
            return executeImpl();          
        }
    
};


struct Condition{
    std::string expression;
};
struct Cases {
    // Clarification: for when and switch, each case contain a condition and rules
   std::vector< std::pair<Condition, std::unique_ptr<RuleInterface>> > cases;
   
};

class RuleVariant{
    private:
        // Clarification: a vector that contains variants, each variant can either be rules or cases 
        // E.g: in the json file, FOREACH contains rules but WHEN contains cases,
        std::variant< std::unique_ptr<RuleInterface>, Cases > rulesOrCases; 
    public:
        RuleVariant(std::unique_ptr<RuleInterface> only_rule) : rulesOrCases{std::move(only_rule)}
        {}
        RuleVariant(Cases only_case) : rulesOrCases{std::move(only_case)} 
        {}
        RuleVariant() = default;
        RuleVariant(const RuleVariant& variant);
        RuleVariant(RuleVariant&&) = delete;
        RuleVariant& operator=(const RuleVariant& o) = delete;
        RuleVariant& operator=(RuleVariant&&) = delete;
    
};

