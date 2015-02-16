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

class HttpServer {
public:
  struct Options;
  typedef ::std::map<::std::string, Handler> HandlerMap;
  HttpServer(const HandlerMap &handlers,
             Handler default_handler,
             const Options &options);
  ~HttpServer() = default;
  Error error() const;
  ::std::future<Error> start();
private:
  HandlerMap handlers;
  Handler default_handler;
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
