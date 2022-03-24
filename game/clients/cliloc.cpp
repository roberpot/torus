#include <game/clients/cliloc.h>


Cliloc::Cliloc(const udword_t& id, bool personal_data) :
    _id(id),
    _has_personal_data(personal_data) {
}

Cliloc::Cliloc(const Cliloc& other) {
    _id = other._id;
    _has_personal_data = other._has_personal_data;
    for (size_t i = 0; i < other._args.size(); ++i) {
        ClilocVariant variant(other._args[i]);
        _args.push_back(variant);
    }
}

Cliloc Cliloc::operator=(const Cliloc& other) {
    _id = other._id;
    _has_personal_data = other._has_personal_data;
    for (size_t i = 0; i < other._args.size(); ++i) {
        ClilocVariant variant = other._args[i];
        _args.push_back(variant);
    }
    return *this;
}

udword_t Cliloc::get_id() const {
    return _id;
}

void Cliloc::add_arg(const std::string& str) {
    ClilocVariant var(str);
    _args.push_back(var);
}

void Cliloc::add_arg(const dword_t& val) {
    ClilocVariant var(val);
    _args.push_back(var);
}

uword_t Cliloc::get_arg_count() const {
    return uword_t(_args.size());
}

std::string Cliloc::get_args() const {
    std::string str;
    if (_args.size() > 0) {
        for (size_t i = 0; i < _args.size(); ++i) {
            ClilocVariant var = _args[i];
            if (var.is_type_integer()) {
                str.append(std::to_string(var.get_integer()).c_str());
            }
            else if (var.is_type_string()) {
                str.append(var.get_string());
            }
            else {
                continue;
            }
            str.push_back('\t');
        }
        str.pop_back();
    }
    return str;
}

bool Cliloc::has_personal_data() const {
    return false;
}
