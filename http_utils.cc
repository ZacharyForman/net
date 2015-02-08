#if __cplusplus >= 201100L

#include "http_utils.h"
#include <map>
#include <string>

namespace net {

namespace {

enum State {
  q1, q2, q3, q4, q5
};

}

::std::map<::std::string, ::std::string> read_headers(const char *str, const char **end)
{
  ::std::map<::std::string, ::std::string> ret;
  State st = q1;
  ::std::string key, val;
  char *start;
  for (;*str || st == q5;) {
    switch (st) {
      case q1: {
        if (*str == ':') {
          st = q2;
        } else if (*str == '\r' && key.length() == 0) {
          st = q4;
        } else if (*str == '\r' || *str == '\n') {
          goto err;
        } else {
          key += *str;
        }
        break;
      }
      case q2: {
        if (*str == '\r') {
          ret[key] = val;
          key.clear();
          val.clear();
          st = q3;
        } else {
          val += *str;
        }
        break;
      }
      case q3: {
        if (*str == '\n') {
          st = q1;
        } else {
          goto err;
        }
        break;
      }
      case q4: {
        if (*str == '\n') {
          st = q5;
        } else {
          goto err;
        }
        break;
      }
      case q5: {
        *end = str;
        return ret;
      }
    }
    str++;
  }
err:
  ret.clear();
  *end = nullptr;
  return ret;
}

} // net

#endif
