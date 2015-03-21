#ifndef NET_ERROR_H
#define NET_ERROR_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>

namespace net {

// Error codes that can be returned from net's functions.
enum Error {
  OK = 0,
  BAD,
  BAD_PORT,
  IN_USE,
  ALREADY_CONNECTED,
  CONNECTION_REFUSED,
  BAD_SOCKET,
  INTERRUPTED,
  NO_NETWORK,
  TIMED_OUT,
  NO_MEMORY,
  NO_FDS,
  INVALID_ARGS,
  CONN_TERMINATED,
  BAD_HEADERS,
  BAD_METHOD
};

// Extracts the current error from errno.
Error net_err();

} // net

#endif // NET_ERROR_H
