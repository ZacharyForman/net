#include "../net.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <future>

#define BUF_LEN 4096

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
    net::HttpStatus response =
    net::HttpStatus(200, "OK",
                    {
                      {"Content-Type", "text/html"},
                      {"Server", argv[0]}
                    },
                    "<html>"
                      "<body>"
                        "<p>"
                          "Hello World!"
                        "</p>"
                      "</body>"
                    "</html>");
    e = response.write_to_socket(s);
    if (e != net::OK) {
      printf("Error - %d\n", e);
      return;
    }
  };

  std::future<net::Error> f = net::listen_and_serve(port, 5, handler);
  printf("Result was: %d\n", f.get());
}
