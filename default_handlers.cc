#if __cplusplus >= 201100L

#include "http_request.h"
#include "http_status.h"

#include <string>

namespace net {

FileServer::FileServer(const ::std::string &root)
{

}

HttpStatus FileServer::operator()(HttpRequest request) const
{

}

} // net

#endif // __cplusplus >= 201100L
