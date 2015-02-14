#if __cplusplus >= 201100L

#include "http_request.h"
#include "http_server.h"
#include "net_error.h"
#include "serversocket.h"
#include "socket.h"

#include <algorithm>
#include <atomic>
#include <map>
#include <sstream>
#include <string>

namespace net {

HttpServer::HttpServer(const HandlerMap &handlers,
                       Handler default_handler,
                       const Options &options = Options::default_options)
{
  this->handlers = handlers;
  this->default_handler = default_handler;
}

} // net

#endif // __cplusplus >= 201100L
