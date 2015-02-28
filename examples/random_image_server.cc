#include "../default_handlers.h"
#include "../http_server.h"

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " port" << std::endl;
    return 1;
  }
  std::vector<std::string> images = {"cat.jpg", "dog.jpg", "mouse.jpg"};
  int i = 0;
  net::HttpServer::HandlerConfiguration m = {
    {"/", {
      [images, &i] (const net::HttpRequest& r) {
        return net::HttpStatus(200, "OK", {},
                               "<!DOCTYPE html>"
                               "<html><body><img src=\"/img/"
                             + images[i++%3]
                             + "\" /></body></html>");
      },
      true
    }},
    net::FileServer::handle_path("/img", "/tmp/img")
  };

  auto def = [](const net::HttpRequest& r) {
    return net::HttpStatus(404, "NOT FOUND", {}, "404 not found");
  };

  net::HttpServer::Options opts;
  opts.port = atoi(argv[1]);

  net::HttpServer http_server(m, def, opts);

  auto res = http_server.start();

  std::cout << "Finished serving; result was " << res.get() << std::endl;
}
