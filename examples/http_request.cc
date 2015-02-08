#include "../http_request.h"
#include "../net_error.h"
#include "../socket.h"

#include <string>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#define BUF_LEN 16384

int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("usage: %s host port\n", argv[0]);
    return 1;
  }
  net::HttpRequest request =
    net::HttpRequest("GET", "/", {
                     {"Host", argv[1]},
                     {"User-Agent", "net-library"}
                    });
  net::Socket s = net::Socket(argv[1], atoi(argv[2]));
  if (!s) {
    puts("Error!1");
    printf("%d %s\n", s.error(), strerror(errno));
    return 1;
  }
  if (request.write_to_socket(s)) {
    puts("Error!");
    return 1;
  }
  char buf[BUF_LEN];
  int len;
  len = s.read(buf, BUF_LEN - 1);
  if (len < 0) {
    puts("Error!3");
    return 1;
  }
  buf[len] = 0;
  printf("%s\n", buf);

}
