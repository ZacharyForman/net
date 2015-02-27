#if __cplusplus >= 201100L

#include "default_handlers.h"
#include "http_request.h"
#include "http_status.h"
#include "mime.h"

#include <fstream>
#include <sstream>
#include <string>

#include <cstdio>

namespace net {

namespace {

bool read_file(::std::string name, ::std::string *res)
{
  ::std::ifstream f(name, ::std::ios::binary);
  if (!f.is_open()) {
    return false;
  }
  ::std::stringstream buf;
  buf << f.rdbuf();
  *res = buf.str();
  return true;
}

} // namespace

::std::pair< ::std::string, ::std::pair<Handler, bool> >
    FileServer::handle_path(::std::string url,
                            ::std::string path,
                            bool dir)
{
  return {
    url, {
      [url, path](HttpRequest r) {
        std::string path = r.query.path();
        if (url == path) {
          return net::HttpStatus(200, "Index", {}, "Index page...");
        }
        ::std::string file = path.substr(url.size()+1);
        ::std::string content;
        if (!read_file(path+file, &content)) {
          return net::HttpStatus(404, "NOT FOUND", {}, "file not found");
        }
        ::std::stringstream content_length;
        content_length << content.length();
        return net::HttpStatus(200, "OK",{
          {"Content-Length", content_length.str()},
          {"Mime-Version", "1.0"},
          {"Content-Type", get_mime_type(file.substr(file.find_last_of('.')+1))}
        }, content);
      },
      dir
    }
  };
}

} // net

#endif // __cplusplus >= 201100L
