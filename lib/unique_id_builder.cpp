
#include <string_view>
#include <cstdint>
#include <stdexcept>

#include "unique_id.h"
#include "unique_id_builder.h"

char convert_to_abc(uint32_t x) {
    return 'A' + (char) x;
}

char convert_to_num(uint32_t x) {
    return '0' + (char) x;
}

char convert_a(uint32_t x) {
    if (x <= 25) {
      return convert_to_abc(x);
    }
  
    return convert_to_num(x);

}


IUniqueId StrUUIDBuilder::createUniqueId() {

    if (this->reusable_ids.size() > 0) {
        auto x = this->reusable_ids.back();
        this->reusable_ids.pop_back();
        return x;
    }

    uint32_t useSeed = this->seed++;
    if (useSeed == this->base) {
        throw std::overflow_error("ran out of unique ids.");
    }

    char id[6];

    id[0] = convert_a(useSeed);

    useSeed <<= 5;
    id[1] = convert_a(useSeed);

    useSeed <<= 5;
    id[2] = convert_a(useSeed);

    useSeed <<= 5;
    id[3] = convert_a(useSeed);

    useSeed <<= 5;
    id[4] = convert_a(useSeed);

    useSeed <<= 5;
    id[5] = convert_a(useSeed);

    return UniqueId(id);
}

void StrUUIDBuilder::disposeId(IUniqueId &uid) {
    this->reusable_ids.push_back(uid);
}
