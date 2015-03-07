#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "path.h"
#include "socket.h"

#include <map>
#include <string>

namespace net {

// Essentially a value class that encapsulates a HTTP request.
// Contains a bunch of strings - try to pass by reference wherever possible.
class HttpRequest {
public:
  HttpRequest() = default;
  
  // Constructs the HttpRequest with the given parameters
  HttpRequest(const ::std::string &method,
              const ::std::string &path,
              const ::std::map<::std::string, ::std::string> &headers,
              const ::std::string &msg = "",
              const ::std::string &version = "HTTP/1.1");
  HttpRequest(HttpRequest &&request) = default;
  HttpRequest(const HttpRequest &request) = default;

  ::std::string str() const;
  
  // Writes the HttpRequest to the given Socket, returning an Error if the Socket
  // experiences an error. Returns OK on success.
  Error write_to_socket(Socket s) const;
  // Reads the HttpRequest from the given Socket, returning an Error if the Socket
  // experiences an error. Returns OK on success.
  Error read_from_socket(Socket s);

  ::std::string method;
  Query query;
  ::std::map<::std::string, ::std::string> headers;
  ::std::string version;
  ::std::string msg;
};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_REQUEST_H
