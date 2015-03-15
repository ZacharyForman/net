#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "socket.h"

#include <map>
#include <string>

namespace net {

// Reads headers from the given string.
// Sets *end to point to the end of the headers.
::std::map<::std::string, ::std::string>
read_headers(const char *str, const char **end);

namespace internals {
// Gets the headers from a given socket into a request string.
int get_header(Socket s, ::std::string &req);
// Given a socket and a length remaining, gets the remainder
// of the message from the socket.
void read_remainder(Socket s, ::std::string &rem, int length);
} // internals

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_UTILS_H
