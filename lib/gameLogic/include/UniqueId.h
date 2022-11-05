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

private:
    std::string value;
};


