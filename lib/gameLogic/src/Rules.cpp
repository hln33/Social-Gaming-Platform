#include "Rules.h"
#include <string>


class ForEachRule : public RuleInterface{
    public:
        ForEachRule(ListExpression expression, ListObject object) : 
        listExpression{expression}, 
        listObj{object}
        { }

        std::string getName() const override{
            return this->ruleName;
        }

        RuleStatus executeImpl() override{
            return RuleStatus::FAIL;
        }
    private:
        std::string ruleName = "foreach";
        ListExpression listExpression;
        ListObject listObj; 
        // RuleList ruleList;
};

