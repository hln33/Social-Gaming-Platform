
#include <string>

// unique id for anything
class IUniqueId {
public:
    virtual std::string getValue() const;
};

class UniqueId : public IUniqueId {
public:
    virtual std::string getValue() const override;

private:
    std::string value;
};

