#pragma once
#include<string>
#include<vector>
#include <variant>
enum RuleStatus{
    SUCCESS, 
    FAIL 
};

class RuleInterface {
    public: 
        RuleStatus execute(){
            return executeImpl();          
        }
    private:
        virtual RuleStatus executeImpl() = 0;
        virtual std::string getName() const = 0;
};

struct ListExpression{ 
    std::string expression;
};

struct Condition{
    std::string expression;
};

struct ListObject{
    std::string expression;
};

struct Rules{
    std::vector<RuleInterface> rules;
};

struct Cases {
    // Clarification: for when and switch, each case contain a condition and rules
   std::vector< std::pair<Condition, Rules> > cases;
   
};

class RuleList{
    // Clarification: a vector that contains variants, each variant can either be rules or cases 
    // E.g: For each contains rules but when contains cases,
    public: 
    RuleList(const RuleList& rl){
        // this->list = rl.list;
    };
    RuleList(RuleList&&) = default;
    RuleList(std::vector< std::variant<Rules, Cases> > list){
        // this->list = list;
    }
    std::vector< std::variant<Rules, Cases> > list;  
};


