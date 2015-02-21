#if __cplusplus >= 201100L

#include "mime.h"

#include <map>
#include <string>

namespace net {

const ::std::map<::std::string, ::std::string> mime_types {
  // Binary data type
  {"bin", "application/octet-stream"},
  // Text types
  {"css", "text/css"},
  {"csv", "text/csv"},
  {"htm", "text/html"},
  {"html", "text/html"},
  {"txt", "text/plain"},
  {"rtf", "text/rtf"},
  {"xml", "text/xml"},
  // Image types
  {"jpg", "image/jpeg"},
  {"jpeg", "image/jpeg"},
  {"gif", "image/gif"},
  {"png", "image/png"},
  {"svg", "image/svg+xml"},
  {"tiff", "image/tiff"},
  // Audio/Video types
  {"avi", "video/avi"},
  {"mpeg", "video/mpeg"},
  {"mp4", "video/mp4"},
  {"ogg", "video/ogg"},
  {"webm", "video/webm"},
  {"flv", "video/flv"},
  {"flac", "audio/flac"},
  // Misc types
  {"json", "application/json"},
  {"pdf", "application/pdf"},
  {"zip", "application/zip"}
};

MimeType get_mime_type(::std::string extension)
{
  auto it = mime_types.find(extension);
  if (it == mime_types.end()) {
    return "";
  }
  return it->second;
}

} // net

#endif // __cplusplus >= 201100L
