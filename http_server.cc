#if __cplusplus >= 201100L

#include "http_request.h"
#include "http_server.h"
#include "net_error.h"
#include "serversocket.h"
#include "socket.h"

#include <future>
#include <thread>

namespace net {

// TODO: lazy matching => (make trie like thing)

HttpServer::HttpServer(const HandlerMap &handlers,
                       Handler default_handler,
                       const Options &options)
    : ss(options.port, options.queue_length)
{
  this->handlers = handlers;
  this->default_handler = default_handler;
  die_on_error = options.die_on_error;
  err = ss.error();
}

Error HttpServer::error() const {
  return err;
}

::std::future<Error> HttpServer::start() {
  return ::std::async([=]() {
    if (!ss) return ss.error();

    for (;;) {
      Socket s = ss.accept();

      if (s) {
        ::std::thread([=]() {
          HttpRequest request;

          request.read_from_socket(s);

          Handler h = default_handler;

          HttpStatus response = h(request);

          response.write_to_socket(s);
        }).detach();
      } else if (die_on_error) {
        return s.error();
      }
    }
    return OK;
  });
}

} // net

#endif // __cplusplus >= 201100L
