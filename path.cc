#if __cplusplus >= 201100L

#include "path.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace net {

namespace internals {

enum state {

}

} // internals

Query::Query(const ::std::string &path)
{

}

::std::string Query::str() const
{
  return full_path;
}

::std::string Query::path() const
{
  ::std::string p;
  for (const auto &s : components) {
    p += "/" + s;
  }
  return p;
}

} // net

#endif // __cplusplus >= 201100L
