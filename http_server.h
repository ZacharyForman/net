#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "http_request.h"
#include "http_status.h"

#include <memory>

namespace net {

class Handler {
public:
  virtual HttpStatus operator()(const HttpRequest &req) = 0;
};

template<typename T>
class DerivedHandler : public Handler {
public:
  DerivedHandler(const T t) : h(t) { }
  HttpStatus operator()(const HttpRequest& req) {
    return h(req);
  }
private:
  T h;
};

template<typename T>
Handler *make_handler(T t) {
  return new DerivedHandler<T>(t);
}

class HttpHandler {
public:
  template<typename T>
  HttpHandler(const T &f) : h(make_handler(f)) { }
  HttpHandler() = default;

  HttpStatus operator()(const HttpRequest &req) {
    return (*h)(req);
  }

private:
  std::shared_ptr<Handler> h;
};

class HttpServer {

};

} // net

#endif // __cplusplus >= 201100L

#endif // HTTP_UTILS_H
