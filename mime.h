#ifndef MIME_H
#define MIME_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>

namespace net {

typedef ::std::string MimeType;

MimeType get_mime_type(::std::string extension);

} // net

#endif // __cplusplus >= 201100L

#endif // MIME_H
