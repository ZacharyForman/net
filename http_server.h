#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "handler.h"
#include "http_request.h"
#include "http_status.h"
#include "net_error.h"
#include "serversocket.h"

#include <future>
#include <memory>
#include <string>
#include <thread>

namespace net {

namespace internals {

class HandlerMap;

} // internals

// Utility class to serve a variety of functions (through the
// Handler interface) over HTTP.
// Construct the HttpServer using an initializer list for a
// HandlerConfiguration
// For example:
// HttpServer s = HttpServer({
//                {"/", {[](HttpRequest r) {
//                        //...
//                        return HttpStatus(...);
//                      }, true}
//                 }
//              }, default_handler, options);
class HttpServer {
public:
  // Options available to configure the server.
  struct Options;
  // Mapping of path to handler. If the boolean is true,
  // perform partial matching.
  typedef ::std::map< ::std::string, ::std::pair<Handler, bool> >
          HandlerConfiguration;
  HttpServer(const HandlerConfiguration &handlers,
             Handler default_handler,
             const Options &options);
  ~HttpServer();
  Error error() const;
  ::std::future<Error> start();
private:
  // The handlers associated with this server.
  internals::HandlerMap *handlers;
  // The listening socket.
  ServerSocket ss;
  Error err;
  bool die_on_error;
};

struct HttpServer::Options {
  int port = 8080;
  int queue_length = 4;
  bool die_on_error = true;
};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_SERVER_H
