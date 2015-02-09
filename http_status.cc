#if __cplusplus >= 201100L

#include "http_utils.h"
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


Error HttpStatus::read_from_socket(Socket s)
{
  ::std::string stat;
  ::std::string statmsg;
  int pos = internals::get_header(s, stat);
  if (s.error() != OK) {
    return s.error();
  }

  const char *req = stat.c_str();
  while (*req && *req != ' ') version += *req++;
  if (*req) req++;
  while (*req && *req != ' ') statmsg += *req++;
  status = ::atoi(statmsg.c_str());
  if (*req) req++;
  while (*req && *req != '\r') reason += *req++;
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
  int remaining_length =
    ::atoi(headers["Content-Length"].c_str()) - msg.length();

  internals::read_remainder(s, msg, remaining_length);
  if (s.error() != OK) {
    return s.error();
  }

  return OK;
}

} // net

#endif // __cplusplus >= 201100L
