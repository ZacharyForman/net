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

#define BUF_LEN 1024

namespace net {

HttpRequest::HttpRequest(
    const ::std::string &method,
    const ::std::string &path,
    const ::std::map<::std::string, ::std::string> &headers,
    const ::std::string &msg,
    const ::std::string &version)
{
  this->method = method;
  this->path = path;
  this->headers = headers;
  this->msg = msg;
  this->version = version;
}

::std::string HttpRequest::str() const {
  ::std::stringstream ret;
  ret << method << " " << path << " " << version << "\r\n";
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

namespace {

int get_header(Socket s, ::std::string &req)
{
  int total_len = 0;
  char buf[BUF_LEN];
  int len = 0;

  while ((len = s.read(buf, BUF_LEN - 1)) > 0) {
    buf[len] = 0;
    req += buf;

    auto it = req.find("\r\n\r\n", total_len);
    total_len += len;

    if (it != std::string::npos) return it;
  }
}

void read_remainder(Socket s, ::std::string &rem, int length)
{
  char buf[BUF_LEN];
  int len = 0;
  while (length && (len = s.read(buf, BUF_LEN - 1)) > 0) {
    buf[len] = 0;
    rem += buf;
    length -= len;
  }
}

} // namespace

Error HttpRequest::read_from_socket(Socket s)
{
  ::std::string request;
  int pos = get_header(s, request);
  if (s.error() != OK) {
    return s.error();
  }

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

  const char *end;
  headers = read_headers(req, &end);

  if (headers.size() == 0) {
    return BAD_HEADERS;
  }

  msg = end;
  int remaining_length = atoi(headers["Content-Length"].c_str()) - msg.length();

  read_remainder(s, msg, remaining_length);
  if (s.error() != OK) {
    return s.error();
  }

  return OK;
}

} // net

#endif // __cplusplus >= 201100L
