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
  Query() = default;
  explicit Query(const ::std::string &path);
  ::std::string str() const;
  ::std::string path() const;

  ::std::vector<::std::string> components;
  ::std::string section;
  ::std::map<::std::string, ::std::string> parameters;
  ::std::string full_path;
};

} // net

#endif // __cplusplus >= 201100L

#endif // PATH_H
