#pragma once

#include <string>
#include <string_view>

// unique id for anything
class UniqueIdInterface {
public:
    virtual std::string getValue() const;
};

class UniqueId : public UniqueIdInterface {
public:
    UniqueId(std::string s) : value(s) { }

        std::string getValue() const override;
        std::string getPrefix() const;
        std::string getSuffix() const;
        std::string getFullValue() const;
        std::string getFullValueWithPrefix() const;
        std::string return_value(const std::string& value) const;


    private:
        std::string value;
        std::string prefix;
        std::string suffix;
};


