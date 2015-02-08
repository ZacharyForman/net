#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <atomic>
#include <map>
#include <string>

namespace net {

class HttpRequest {
public:
  static const :std::string version = "1.1";
  HttpRequest(const ::std::string &method,
              const ::std::string &url,
              ::std::map<::std::string, ::std::string> headers,
              const ::std::string &msg);
  ~HttpRequest();
  const ::std::string method;
  const ::std::map<::std::string, ::std::string> headers;
  const ::std::string msg;
  explicit operator bool();
};

} // net

#endif // HTTP_REQUEST_H
