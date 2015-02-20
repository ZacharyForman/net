#if __cplusplus >= 201100L

#include "path.h"

#include <string>
#include <vector>
#include <map>

namespace net {

struct Query {
  ::std::vector<::std::string> components;
  ::std::map<::std::string, ::std::string> get_paramaters;
  ::std::string remainder;
};

Query split_path(::std::string path)
{

}

} // net

#endif // __cplusplus >= 201100L
