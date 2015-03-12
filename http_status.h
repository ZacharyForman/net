#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "socket.h"

#include <map>
#include <string>

namespace net {

// Essentially a value class that encapsulates a HTTP status response.
// Contains a bunch of strings - try to pass by reference wherever possible.
class HttpStatus {
public:
  HttpStatus() = default;

  // Constructs the HttpStatus with the given parameters.
  HttpStatus(int status, const ::std::string &reason,
             const ::std::map<::std::string, ::std::string> &headers,
             const ::std::string &msg = "",
             const ::std::string &version = "HTTP/1.1");
  HttpStatus(HttpStatus &&request) = default;
  HttpStatus(const HttpStatus &request) = default;

  ::std::string str() const;
  
  // Writes the HttpStatus to the given Socket, returning an Error if the Socket
  // experiences an error. Returns OK on success.
  Error write_to_socket(Socket s) const;
  // Reads the HttpStatus from the given Socket, returning an Error if the Socket
  // experiences an error. Returns OK on success.
  Error read_from_socket(Socket s);

  int status;
  ::std::string reason;
  ::std::map<::std::string, ::std::string> headers;
  ::std::string version;
  ::std::string msg;
};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_STATUS_H
