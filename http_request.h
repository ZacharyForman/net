#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "socket.h"

#include <map>
#include <string>

namespace net {

class HttpRequest {
public:
  HttpRequest(const ::std::string &method,
              const ::std::string &path,
              const ::std::map<::std::string, ::std::string> &headers,
              const ::std::string &msg = "",
              const ::std::string &version = "1.1");
  HttpRequest(HttpRequest &&request) = default;
  HttpRequest(const HttpRequest &request) = default;

  ::std::string str() const;
  Error write_to_socket(Socket s) const;

  ::std::string method;
  ::std::string path;
  ::std::map<::std::string, ::std::string> headers;
  ::std::string version;
  ::std::string msg;
};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_REQUEST_H
