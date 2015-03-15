#ifndef HANDLER_H
#define HANDLER_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "net_error.h"
#include "serversocket.h"
#include <future>
#include <thread>

namespace net {

// Given a function like thing that takes a Socket as a parameter,
// listen on the given port and call that function on it.
template<typename Handler>
::std::future<Error> listen_and_serve(int port, int queue_length, Handler f)
{
  return ::std::async([=]() {
    ServerSocket ss(port, queue_length);

    if (!ss) {
      return ss.error();
    }

    for (;;) {
      Socket s = ss.accept();
      if (s) {
        // Handle the connection
        ::std::thread(f, s).detach();
      }
    }
    return OK;
  });
}

} // net

#endif // __cplusplus < 201100L

#endif // HANDLER_H
