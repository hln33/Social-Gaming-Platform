#include <string>

#include "unique_id.h"

std::string_view UniqueId::getValue() const {
    return this->value;
}
