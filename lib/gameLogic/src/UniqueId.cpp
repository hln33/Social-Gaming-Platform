#include <string>
#include <string_view>

#include "../includes/UniqueId.h"

std::string UniqueId::getValue() const {
    return this->value;
}
