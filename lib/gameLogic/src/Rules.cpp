#include "Rules.h"



class ForEachRule : public RuleInterface{
    public:
        ForEachRule()
        {}

        std::string getName() const override{
            return this->ruleName;
        }

        ExecutionStatus executeImpl() override{
            return ExecutionStatus::FAIL;
        }

        void setListExpression(std::string str) {
            this->listExpression = str;
        }
        void setElement(std::string str){
            this->element = str;
        }
        void addRule(RuleVariant rule){
            list.push_back(rule);
        }
    private:
        std::string ruleName = "foreach";
        std::string listExpression;
        std::string element; 
        std::vector< RuleVariant > list;        
};

