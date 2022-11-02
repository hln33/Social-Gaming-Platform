#pragma once

#include <string_view>

// unique id for anything
class IUniqueId {
public:
    virtual std::string_view getValue() const;
};

class UniqueId : public IUniqueId {
public:
    UniqueId(std::string_view s) : value(s) { }

    std::string_view getValue() const override;

private:
    std::string_view value;
};

