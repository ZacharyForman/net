#include "../http_request.h"
#include "../http_status.h"
#include "../net_error.h"
#include "../socket.h"

#include <cstdio>
#include <cstdlib>

#define BUF_LEN 16384

int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("usage: %s host port\n", argv[0]);
    return 1;
  }
  net::Socket s = net::Socket(argv[1], atoi(argv[2]));
  if (!s) {
    puts("Error making socket!");
    return 1;
  }
  net::HttpRequest request =
    net::HttpRequest("GET", "/", {
                     {"Host", argv[1]},
                     {"User-Agent", "net-library"}
                    });
  if (request.write_to_socket(s)) {
    puts("Error writing to socket!");
    return 1;
  }
  net::HttpStatus stat;
  if (stat.read_from_socket(s)) {
    puts("Error reading from socket!");
    return 1;
  }

  printf("%s\n", stat.str().c_str());
}
