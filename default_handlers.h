#ifndef DEFAULT_HANDLERS_H
#define DEFAULT_HANDLERS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>
#include <utility>

#include "handler.h"
#include "http_status.h"
#include "http_request.h"

namespace net {

class FileServer {
public:
  static ::std::pair< ::std::string, ::std::pair<Handler, bool> >
      handle_path(::std::string url, ::std::string path, bool dir = true);
};

} // net

#endif // __cplusplus >= 201100L

#endif // DEFAULT_HANDLERS_H
