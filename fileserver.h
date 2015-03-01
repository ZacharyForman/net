#ifndef FILESERVER_H
#define FILESERVER_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>
#include <utility>

#include "handler.h"

namespace net {

// Provides a handler that serves files.
// Simply provide net::FileServer::handle_path("/path/to/handle",
//                                             "/directory/to/serve")
// to handle serving a directory of files, or
// net::FileServer::handle_path("/path/to/handle", "/file/to/serve", false)
// to serve a single file.
struct FileServer {
  static ::std::pair< ::std::string, ::std::pair<Handler, bool> >
      handle_path(::std::string url, ::std::string path, bool dir = true);
};

} // net

#endif // __cplusplus >= 201100L

#endif // FILESERVER_H
