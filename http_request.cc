#include "http_request.h"
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
  this->path = path;
  this->headers = headers;
  this->msg = msg;
  this->version = version;
}

::std::string HttpRequest::str() const {
  ::std::stringstream ret;
  ret << method << " " << path << " HTTP/" << version << "\r\n";
  for (const auto &header : headers) {
    ret << header.first << ": " << header.second << "\r\n";
  }
  ret << "\r\n";
  ret << msg;
  return ret.str();
}

Error HttpRequest::write_to_socket(Socket s, int buffer_size) const
{
  ::std::string msg = str();
  int len = s.write(msg.c_str(), msg.length());
  if (len < 0) {
    return s.error();
  }
  return OK;
}

} // net
