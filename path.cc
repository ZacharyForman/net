#if __cplusplus >= 201100L

#include "path.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace net {

namespace {

enum State {
  start_state,
  directory_char,
  directory_separator,
  get_param_key,
  get_param_key_hex,
  get_param_val,
  get_param_val_hex
};

char hex_to_char(::std::string hex)
{
  char c = 0;
  for (int i = 0; i < 2 && i < hex.length(); i++) {
    char d = hex[i];
    switch (d) {
      case '0': // FALLTHROUGH
      case '1': // FALLTHROUGH
      case '2': // FALLTHROUGH
      case '3': // FALLTHROUGH
      case '4': // FALLTHROUGH
      case '5': // FALLTHROUGH
      case '6': // FALLTHROUGH
      case '7': // FALLTHROUGH
      case '8': // FALLTHROUGH
      case '9': {
        c *= 16;
        c += d-'0';
        break;
      }
      case 'A': // FALLTHROUGH
      case 'B': // FALLTHROUGH
      case 'C': // FALLTHROUGH
      case 'D': // FALLTHROUGH
      case 'E': // FALLTHROUGH
      case 'F': {
        c *= 16;
        c += 10+(d-'A');
      }
      case 'a': // FALLTHROUGH
      case 'b': // FALLTHROUGH
      case 'c': // FALLTHROUGH
      case 'd': // FALLTHROUGH
      case 'e': // FALLTHROUGH
      case 'f': {
        c *= 16;
        c += 10+(d-'a');
      }
    }
  }
  return c;
}

} // namespace

Query::Query(const ::std::string &path)
{
  full_path = path;
  const char *str = path.c_str();
  State st = start_state;
  std::string component;
  std::string key;
  std::string val;
  std::string hex;
  for (;;) {
    switch (st) {
      case start_state: {
        switch (*str) {
          case '/': {
            st = directory_char;
            break;
          }
          default: {
            return;
          }
        }
        break;
      }
      case directory_char: {
        switch (*str) {
          case 0: {
            if (component.length()) {
              components.push_back(component);
              component.clear();
            }
            return;
          }
          case '/': {
            if (component.length()) {
              components.push_back(component);
              component.clear();
            }
            st = directory_separator;
            break;
          }
          case '?': {
            if (component.length()) {
              components.push_back(component);
              component.clear();
            }
            st = get_param_key;
            break;
          }
          default: {
            component += *str;
            break;
          }
        }
        break;
      }
      case directory_separator: {
        switch (*str) {
          case 0: {
            return;
          }
          case '/': {
            break;
          }
          case '?': {
            st = get_param_key;
            break;
          }
          default: {
            component += *str;
            st = directory_char;
            break;
          }
        }
        break;
      }
      case get_param_key: {
        switch (*str) {
          case 0: {
            return;
          }
          case '=': {
            st = get_param_val;
            break;
          }
          case '%': {
            st = get_param_key_hex;
            break;
          }
          default: {
            key += *str;
            break;
          }
        }
        break;
      }
      case get_param_key_hex: {
        switch (*str) {
          case 0: {
            return;
          }
          case '1': // FALLTHROUGH
          case '2': // FALLTHROUGH
          case '3': // FALLTHROUGH
          case '4': // FALLTHROUGH
          case '5': // FALLTHROUGH
          case '6': // FALLTHROUGH
          case '7': // FALLTHROUGH
          case '8': // FALLTHROUGH
          case '9': // FALLTHROUGH
          case '0': // FALLTHROUGH
          case 'a': // FALLTHROUGH
          case 'A': // FALLTHROUGH
          case 'b': // FALLTHROUGH
          case 'B': // FALLTHROUGH
          case 'c': // FALLTHROUGH
          case 'C': // FALLTHROUGH
          case 'd': // FALLTHROUGH
          case 'D': // FALLTHROUGH
          case 'e': // FALLTHROUGH
          case 'E': // FALLTHROUGH
          case 'f': // FALLTHROUGH
          case 'F': {
            hex += *str;
            break;
          }
          case '=': {
            key += hex_to_char(hex);
            if (hex.length() > 2) {
              key += hex.substr(2);
            }
            hex.clear();
            st = get_param_key_hex;
            break;
          }
          case '%': {
            key += hex_to_char(hex);
            if (hex.length() > 2) {
              key += hex.substr(2);
            }
            hex.clear();
            break;
          }
          default: {
            key += hex_to_char(hex);
            if (hex.length() > 2) {
              key += hex.substr(2);
            }
            hex.clear();
            key += *str;
            st = get_param_key;
            break;
          }
        }
        break;
      }
      case get_param_val: {
        switch (*str) {
          case '%': {
            st = get_param_val_hex;
            break;
          }
          case '&': {
            parameters[key] = val;
            key.clear();
            val.clear();
            st = get_param_key;
            break;
          }
          case 0: {
            parameters[key] = val;
            return;
          }
          default: {
            val += *str;
            break;
          }
        }
        break;
      }
      case get_param_val_hex: {
        switch (*str) {
          case '1': // FALLTHROUGH
          case '2': // FALLTHROUGH
          case '3': // FALLTHROUGH
          case '4': // FALLTHROUGH
          case '5': // FALLTHROUGH
          case '6': // FALLTHROUGH
          case '7': // FALLTHROUGH
          case '8': // FALLTHROUGH
          case '9': // FALLTHROUGH
          case '0': // FALLTHROUGH
          case 'a': // FALLTHROUGH
          case 'A': // FALLTHROUGH
          case 'b': // FALLTHROUGH
          case 'B': // FALLTHROUGH
          case 'c': // FALLTHROUGH
          case 'C': // FALLTHROUGH
          case 'd': // FALLTHROUGH
          case 'D': // FALLTHROUGH
          case 'e': // FALLTHROUGH
          case 'E': // FALLTHROUGH
          case 'f': // FALLTHROUGH
          case 'F': {
            hex += *str;
            break;
          }
          case '&': {
            val += hex_to_char(hex);
            if (hex.length() > 2) {
              val += hex.substr(2);
            }
            hex.clear();
            parameters[key] = val;
            key.clear();
            val.clear();
            st = get_param_key;
            break;
          }
          case 0: {
            val += hex_to_char(hex);
            if (hex.length() > 2) {
              val += hex.substr(2);
            }
            hex.clear();
            parameters[key] = val;
            return;
          }
          case '%': {
            val += hex_to_char(hex);
            if (hex.length() > 2) {
              val += hex.substr(2);
            }
            hex.clear();
            break;
          }
          default: {
            val += hex_to_char(hex);
            if (hex.length() > 2) {
              val += hex.substr(2);
            }
            hex.clear();
            val += *str;
            st = get_param_val;
            break;
          }
        }
        break;
      }
    }
    str++;
  }
}

::std::string Query::str() const
{
  return full_path;
}

::std::string Query::path() const
{
  ::std::string p;
  for (const auto &s : components) {
    p += "/" + s;
  }
  return p;
}

} // net

#endif // __cplusplus >= 201100L
