#ifndef HANDLER_H
#define HANDLER_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "http_request.h"
#include "http_status.h"

#include <memory>

namespace net {

namespace internals {

// Base class that all handler internals are derived from.
// We do this for polymorphism on the operator().
class BaseHandler {
public:
  virtual HttpStatus operator()(const HttpRequest &req) = 0;
};

// Wraps anything that can be called as though it was a function
// HttpStatus func(const HttpRequest &req);
template<typename T>
class DerivedHandler : public BaseHandler {
public:
  DerivedHandler(const T t) : h(t) { }
  HttpStatus operator()(const HttpRequest& req) {
    return h(req);
  }
private:
  T h;
};

// Utility method to make type deduction easier
template<typename T>
BaseHandler *make_handler(T t) {
  return new DerivedHandler<T>(t);
}

} // internals

// Wraps anything that can be called as though it was a function
// HttpStatus func(const HttpRequest &req);
// Contains a base handler, and has the same type regardless
// of what it's constructed from.
class Handler {
public:
  template<typename T>
  Handler(const T &f) : h(internals::make_handler(f)) { }
  Handler() = default;

  HttpStatus operator()(const HttpRequest &req) {
    return (*h)(req);
  }

private:
  std::shared_ptr<internals::BaseHandler> h;
};


} // net

#endif // __cplusplus >= 201100L

#endif // HANDLER_H
