#ifndef PATH_H
#define PATH_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>
#include <vector>
#include <map>

namespace net {

struct Query {
  ::std::vector<::std::string> components;
  ::std::map<::std::string, ::std::string> parameters;
  ::std::string remainder;
  explicit Query(::std::string path);
  ::std::string str() const;
};

} // net

#endif // __cplusplus >= 201100L

#endif // PATH_H
