#include "net_error.h"
#include "socket.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>

#include <atomic>
#include <mutex>

#if __cplusplus >= 201100L

namespace net {

Socket::Socket(const Socket &s)
{
  sock = s.sock;
  ref = s.ref;
  err = s.err;
  if (ref != nullptr) (*ref)++;
}

namespace {
int connect_socket(int *sock, const char *host, const char *port)
{
  int sockfd;
  struct addrinfo hints;
  struct addrinfo *result, *a;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  if (getaddrinfo(host, port, &hints, &result)) {
    return -1;
  }
  for (a = result; a != nullptr; a = a->ai_next) {
    sockfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
    int opt;
    if (sockfd == -1) continue;
    if (!setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
      if (connect(sockfd, a->ai_addr, a->ai_addrlen) != -1) break;
    close(sockfd);
  }
  freeaddrinfo(result);
  if (a == nullptr) {
    return -1;
  }
  *sock = sockfd;
  return 0;
}
} // namespace

Socket::Socket(const char *host, int port)
{
  char buf[30];
  ::sprintf(buf, "%d", port);
  if (connect_socket(&sock, host, buf)) goto err;
  ref = new ::std::atomic<int>(1);
  err = OK;
  return;
err:
  err = net_err();
  sock = -1;
  ref = nullptr;
}

Socket::~Socket()
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }
}

int Socket::write(const char *buf, int len)
{
  ::std::lock_guard<::std::mutex> lock(m);
  len = ::write(sock, buf, len);
  if (len < 0) {
    err = net_err();
  }
  return len;
}

int Socket::read(char *buf, int len)
{
  ::std::lock_guard<::std::mutex> lock(m);
  len = ::read(sock, buf, len);
  if (len < 0) {
    err = net_err();
  }
  return len;
}

Socket::operator bool() const
{
  return !err;
}

Socket &Socket::operator=(const Socket &s)
{
  if (ref != nullptr) (*ref)--;
  if (ref != nullptr && *ref == 0) {
    delete ref;
    close(sock);
  }

  sock = s.sock;
  ref = s.ref;
  err = s.err;
  if (ref != nullptr) (*ref)++;
}

Socket::Socket(int s, Error e)
{
  if (s < 0) goto err;
  sock = s;
  ref = new ::std::atomic<int>(1);
  err = OK;
  return;
err:
  err = e;
  sock = -1;
  ref = nullptr;
}

Error Socket::error() const
{
  return err;
}

} //net

#endif // __cplusplus >= 201100L
