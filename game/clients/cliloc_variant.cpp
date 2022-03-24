#include <game/clients/cliloc_variant.h>

ClilocVariant::ClilocVariant() :
    _type(VariantType::NONE) {
}

ClilocVariant::ClilocVariant(const std::string& str) {
    add_arg(str);
}

ClilocVariant::ClilocVariant(const dword_t& val) {
    add_arg(val);
}

ClilocVariant::ClilocVariant(const ClilocVariant& other) :
    _type(other._type),
    _value(other._value) {
}

ClilocVariant ClilocVariant::operator=(const ClilocVariant& other) {
    _type = other._type;
    _value = other._value;
    return *this;
}

ClilocVariant::~ClilocVariant() {
}

void ClilocVariant::add_arg(const std::string& str) {
    _value._str = str;
    _type = VariantType::STRING;
}

void ClilocVariant::add_arg(const dword_t& val) {
    _value._val = val;
    _type = VariantType::INTEGER;
}

bool ClilocVariant::is_type_integer() {
    return _type == VariantType::INTEGER;
}

bool ClilocVariant::is_type_string() {
    return _type == VariantType::STRING;
}

dword_t ClilocVariant::get_integer() {
    dword_t val = 0;
    if (is_type_integer()) {
        return _value._val;
    }
    return val;
}

std::string ClilocVariant::get_string() {
    std::string str;
    if (is_type_string()) {
        str = _value._str;
    }
    return str;
}
