#ifndef NET_H
#define NET_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include "http_request.h"
#include "http_status.h"
#include "net_error.h"
#include "socket.h"
#include "serversocket.h"
#include "listen_and_serve.h"

#endif // __cplusplus < 201100L

#endif // NET_H
