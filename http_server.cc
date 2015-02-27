#if __cplusplus >= 201100L

#include "http_request.h"
#include "http_server.h"
#include "path.h"
#include "net_error.h"
#include "serversocket.h"
#include "socket.h"

#include <future>
#include <thread>

namespace net {

namespace internals {

namespace {

struct Node {
  Node(Handler h, bool b) : handler(h), capture_children(b) {}
  Handler handler;
  bool capture_children;
  ::std::map<::std::string, Node> children;
};

} // namespace

class HandlerMap {
public:
  HandlerMap(HttpServer::HandlerConfiguration config, Handler default_handler);
  Handler get_handler(Query query) const;
private:
  Node root;
};

HandlerMap::HandlerMap(HttpServer::HandlerConfiguration config, Handler default_handler)
  : root(default_handler, true)
{
  // Populate node's children.
}

Handler HandlerMap::get_handler(Query query) const
{
  return root.handler;
}

} // internals

HttpServer::HttpServer(const HandlerConfiguration &handlers,
                       Handler default_handler,
                       const Options &options)
    : ss(options.port, options.queue_length)
{
  this->handlers = new internals::HandlerMap(handlers, default_handler);
  die_on_error = options.die_on_error;
  err = ss.error();
}

HttpServer::~HttpServer()
{
  delete handlers;
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

          Handler h = handlers->get_handler(request.query);

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
