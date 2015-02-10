#include "../http_server.h"
#include <map>
#include <string>
#include <iostream>

int main()
{
  std::map<std::string, net::HttpHandler> m =
  {
    {
      "/path1",
      [] (const net::HttpRequest& r) {
        return net::HttpStatus(418,"NOT A TEAPOT",{});
      }
    },
    {
      "/path2",
      [] (const net::HttpRequest& r) {
        return net::HttpStatus(200, "OK", {});
      }
    }
  };

  auto def = [](const net::HttpRequest& r) {
    return net::HttpStatus(404, "NOT FOUND", {});
  };

  std::string s;

  while (std::cin >> s) {
    net::HttpRequest req("GET", s, {});
    if (m.count(s)) {
      std::cout << m[s](req).str();
    } else {
      std::cout << def(req).str();
    }
  }
}
