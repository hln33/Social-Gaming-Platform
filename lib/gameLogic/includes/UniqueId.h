#pragma once

#include <string>
#include <string_view>

// unique id for anything
class IUniqueId {
public:
    virtual std::string getValue() const;
};

class UniqueId : public IUniqueId {
public:
    UniqueId(std::string s) : value(s) { }

    std::string getValue() const override;

private:
    std::string value;
};


