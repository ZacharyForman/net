#ifndef DEFAULT_HANDLERS_H
#define DEFAULT_HANDLERS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>

#include "handler.h"
#include "http_status.h"
#include "http_request.h"

namespace net {

class FileServer {
public:
  explicit FileServer(const ::std::string &root);
  HttpStatus operator()(HttpRequest request) const;
private:
  ::std::string root;
};

} // net

#endif // __cplusplus >= 201100L

#endif // DEFAULT_HANDLERS_H
