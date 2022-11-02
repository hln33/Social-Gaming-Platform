#include <string>

#include "../includes/unique_id.h"

std::string_view UniqueId::getValue() const {
    return this->value;
}
