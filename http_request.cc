#if __cplusplus >= 201100L

#include "http_request.h"
#include "http_utils.h"
#include "net_error.h"
#include "socket.h"

#include <algorithm>
#include <atomic>
#include <map>
#include <sstream>
#include <string>

namespace net {

HttpRequest::HttpRequest(
    const ::std::string &method,
    const ::std::string &path,
    const ::std::map<::std::string, ::std::string> &headers,
    const ::std::string &msg,
    const ::std::string &version)
{
  this->method = method;
  this->query = Query(path);
  this->headers = headers;
  this->msg = msg;
  this->version = version;
}

::std::string HttpRequest::str() const {
  ::std::stringstream ret;
  ret << method << " " << query.str() << " " << version << "\r\n";
  for (const auto &header : headers) {
    ret << header.first << ": " << header.second << "\r\n";
  }
  ret << "\r\n";
  ret << msg;
  return ret.str();
}

Error HttpRequest::write_to_socket(Socket s) const
{
  ::std::string msg = str();
  int len = s.write(msg.c_str(), msg.length());
  if (len < 0) {
    return s.error();
  }
  return OK;
}

Error HttpRequest::read_from_socket(Socket s)
{
  ::std::string request;
  internals::get_header(s, request);
  if (s.error() != OK) {
    return s.error();
  }
  ::std::string path;
  const char *req = request.c_str();
  while (*req && *req != ' ') method += *req++;
  if (*req) req++;
  while (*req && *req != ' ') path += *req++;
  if (*req) req++;
  while (*req && *req != '\r') version += *req++;
  if (*req) req++;
  if (*req && *req == '\n') req++;

  if (*req == 0) {
    return BAD_HEADERS;
  }

  query = Query(path);

  const char *end;
  headers = read_headers(req, &end);

  if (headers.size() == 0) {
    return BAD_HEADERS;
  }

  msg = end;
  int remaining_length = headers.count("Content-Length") ?
    ::atoi(headers["Content-Length"].c_str()) - msg.length() :
    0;

  if (remaining_length > 0) {
    internals::read_remainder(s, msg, remaining_length);
  }
  if (s.error() != OK) {
    return s.error();
  }

  return OK;
}

} // net

#endif // __cplusplus >= 201100L
