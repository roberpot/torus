#include <game/clients/tooltip.h>


Tooltip::Tooltip() :
    _has_personal_data(false),
    _version(0) {
}

Tooltip::Tooltip(const Tooltip& other) {
    _has_personal_data = false;
    _version = other._version;
    for (auto& it : other._clilocs) {
        add_cliloc(it.second);
    }
}

void Tooltip::add_cliloc(const Cliloc& cliloc) {
    if (cliloc.has_personal_data()) {
        _has_personal_data = true;
    }
    _clilocs[cliloc.get_id()] = cliloc;
}

size_t Tooltip::get_cliloc_count() {
    return _clilocs.size();
}

std::vector<Cliloc> Tooltip::get_clilocs() const {
    std::vector<Cliloc> vec;
    for (auto& it : _clilocs) {
      Cliloc cliloc(it.second);
        vec.push_back(cliloc);
    }
    return vec;
}

bool Tooltip::has_personal_data() const {
    return _has_personal_data; }

udword_t Tooltip::get_version() const {
    return _version;
}
