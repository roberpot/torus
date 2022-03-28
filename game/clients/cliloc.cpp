#include <game/clients/cliloc.h>
#include <library/string.h>

Cliloc::Cliloc(const udword_t &id, bool personal_data)
    : _id(id), _has_personal_data(personal_data) {}

Cliloc::Cliloc(const Cliloc &other) {
  _id = other._id;
  _has_personal_data = other._has_personal_data;
  for (size_t i = 0; i < other._args.size(); ++i) {
    ClilocVariant variant(other._args[i]);
    _args.push_back(variant);
  }
}

Cliloc Cliloc::operator=(const Cliloc &other) {
  _id = other._id;
  _has_personal_data = other._has_personal_data;
  for (size_t i = 0; i < other._args.size(); ++i) {
    ClilocVariant variant = other._args[i];
    _args.push_back(variant);
  }
  return *this;
}

udword_t Cliloc::get_id() const { return _id; }

void Cliloc::add_arg(const std::string &str) { add_arg(to_wstring(str)); }

void Cliloc::add_arg(const std::wstring &wstr) {
  if (!_warg.empty()) {
    _warg.push_back(L'\t');
  }
  _warg.append(wstr);
}

void Cliloc::add_arg(const dword_t &val) {
  add_arg(to_wstring(std::to_string(val)));
}

uword_t Cliloc::get_arg_count() const { return uword_t(_args.size()); }

const std::wstring &Cliloc::get_args() const { return _warg; }

bool Cliloc::has_personal_data() const { return _has_personal_data; }
