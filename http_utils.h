#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <map>
#include <string>

namespace net {

::std::map<::std::string, ::std::string>
read_headers(const char *str, const char **end);

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_UTILS_H
