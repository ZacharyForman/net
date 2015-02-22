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

::std::map<::std::string, ::std::string>
    read_headers(const char *str, const char **end)
{
  ::std::map<::std::string, ::std::string> ret;
  State st = q1;
  ::std::string key, val;
  while(*str || st == q5) {
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
        } else if (val != "" || *str != ' ') {
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
namespace internals {

#define BUF_LEN 1024

int get_header(Socket s, ::std::string &req)
{
  int total_len = 0;
  char buf[BUF_LEN];
  int len = 0;

  while ((len = s.read(buf, BUF_LEN - 1)) > 0) {
    buf[len] = 0;
    req += buf;

    auto it = req.find("\r\n\r\n", total_len);
    total_len += len;

    if (it != std::string::npos) return it;
  }
  return total_len;
}

void read_remainder(Socket s, ::std::string &rem, int length)
{
  char buf[BUF_LEN];
  int len = 0;
  while (length && (len = s.read(buf, BUF_LEN - 1)) > 0) {
    buf[len] = 0;
    rem += buf;
    length -= len;
  }
}

#undef BUF_LEN

} // internal

} // net

#endif
