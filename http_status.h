#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "socket.h"

#include <map>
#include <string>

namespace net {

class HttpStatus {
public:
  HttpStatus() = default;
  HttpStatus(int status, const ::std::string &reason,
             const ::std::map<::std::string, ::std::string> &headers,
             const ::std::string &msg = "",
             const ::std::string &version = "HTTP/1.1");
  HttpStatus(HttpStatus &&request) = default;
  HttpStatus(const HttpStatus &request) = default;

  ::std::string str() const;
  Error write_to_socket(Socket s) const;

  int status;
  ::std::string reason;
  ::std::map<::std::string, ::std::string> headers;
  ::std::string version;
  ::std::string msg;
};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_STATUS_H
