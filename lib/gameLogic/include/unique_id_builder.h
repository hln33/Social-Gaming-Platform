#pragma once

#include <string_view>
#include <cstdint>
#include <vector>


#include "UniqueId.h"

class IUniqueIdBuilder {
public:
    virtual IUniqueId createUniqueId();

    virtual void disposeId(IUniqueId &);

};

class StrUUIDBuilder : public IUniqueIdBuilder {
public:
    StrUUIDBuilder() { }

    StrUUIDBuilder(uint32_t s) : seed(s), base(s) { }

    IUniqueId createUniqueId() override;

    void disposeId(IUniqueId &) override;

private:
    uint32_t seed = 0;
    uint32_t base = 0;

    std::vector<IUniqueId> reusable_ids;
};
