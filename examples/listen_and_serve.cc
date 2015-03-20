#include "../listen_and_serve.h"
#include "../socket.h"
#include "../http_status.h"
#include "../http_request.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <future>

namespace patch {
template<typename T>
::std::string to_string(T v)
{
  std::stringstream ss;
  ss << v;
  return ss.str();
}
}

int main(int argc, char **argv)
{
  int port = 8080;
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  const auto handler = [argv](net::Socket s) {
    net::HttpRequest r;
    net::Error e = r.read_from_socket(s);
    if (e != net::OK) {
      printf("Error - %d\n", e);
      return;
    }
    printf("%s\n", r.str().c_str());
    std::string msg ="<html><body><p>Hello World!</p></body></html>";
    net::HttpStatus response =
      net::HttpStatus(200, "OK",
                    {
                      {"Content-Type", "text/html"},
                      {"Content-Length", patch::to_string(msg.length())},
                      {"Server", argv[0]}
                    },
                    msg);
    e = response.write_to_socket(s);
    if (e != net::OK) {
      printf("Error - %d\n", e);
      return;
    }
  };

  std::future<net::Error> f = net::listen_and_serve(port, 5, handler);
  printf("Result was: %d\n", f.get());
}
