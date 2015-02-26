#if __cplusplus >= 201100L

#include "path.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace net {

namespace {

enum State {
  q0, q1, q2, q3, q4, q5, q6
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
  State st = q0;
  std::string component;
  std::string key;
  std::string val;
  std::string hex;
  for (;;) {
    switch (st) {
      case q0: {
        switch (*str) {
          case '/': {
            st = q1;
            break;
          }
          default: {
            return;
          }
        }
        break;
      }
      case q1: {
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
            st = q2;
            break;
          }
          case '?': {
            if (component.length()) {
              components.push_back(component);
              component.clear();
            }
            st = q3;
            break;
          }
          default: {
            component += *str;
            break;
          }
        }
        break;
      }
      case q2: {
        switch (*str) {
          case 0: {
            return;
          }
          case '/': {
            break;
          }
          case '?': {
            st = q3;
            break;
          }
          default: {
            component += *str;
            st = q1;
            break;
          }
        }
        break;
      }
      case q3: {
        switch (*str) {
          case 0: {
            return;
          }
          case '=': {
            st = q5;
            break;
          }
          case '%': {
            st = q4;
            break;
          }
          default: {
            key += *str;
            break;
          }
        }
        break;
      }
      case q4: {
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
            st = q4;
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
            st = q3;
            break;
          }
        }
        break;
      }
      case q5: {
        switch (*str) {
          case '%': {
            st = q6;
            break;
          }
          case '&': {
            parameters[key] = val;
            key.clear();
            val.clear();
            st = q3;
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
      case q6: {
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
            st = q3;
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
            st = q5;
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
