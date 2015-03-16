#ifndef MIME_H
#define MIME_H

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

#include <string>

namespace net {

typedef ::std::string MimeType;

// Given a file extension returns the MimeType.
// It might be nice if we could also do it for a
// filename - e.g. the leading 2 bits of an executable
// on windows are always MZ.
MimeType get_mime_type(::std::string extension);

} // net

#endif // __cplusplus >= 201100L

#endif // MIME_H
