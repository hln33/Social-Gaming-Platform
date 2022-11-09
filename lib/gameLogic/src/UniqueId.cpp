
#include "UniqueId.h"

std::string UniqueId::getValue() const {
    return this->value;
}

std::string UniqueId::getPrefix() const {
    return this->prefix;
}

std::string UniqueId::getSuffix() const {
    return this->suffix;
}

std::string UniqueId::getFullValue() const {
    return this->prefix + this->value + this->suffix;
}

std::string UniqueId::getFullValueWithPrefix() const {
    return this->prefix + this->value;
}

std::string return_value(const std::string& value) {
    return value;
}

