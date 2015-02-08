#if __cplusplus >= 201100L

#include "http_status.h"
#include "net_error.h"
#include "socket.h"

#include <algorithm>
#include <atomic>
#include <map>
#include <sstream>
#include <string>

namespace net {

HttpStatus::HttpStatus(
    int status,
    const ::std::string &reason,
    const ::std::map<::std::string, ::std::string> &headers,
    const ::std::string &msg,
    const ::std::string &version)
{
  this->status = status;
  this->reason = reason;
  this->headers = headers;
  this->msg = msg;
  this->version = version;
}

::std::string HttpStatus::str() const {
  ::std::stringstream ret;
  ret << version << " " << status << " " << reason << "\r\n";
  for (const auto &header : headers) {
    ret << header.first << ": " << header.second << "\r\n";
  }
  ret << "\r\n";
  ret << msg;
  return ret.str();
}

Error HttpStatus::write_to_socket(Socket s) const
{
  ::std::string msg = str();
  int len = s.write(msg.c_str(), msg.length());
  if (len < 0) {
    return s.error();
  }
  return OK;
}

} // net

#endif // __cplusplus >= 201100L
