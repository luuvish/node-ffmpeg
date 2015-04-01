#ifndef SRC_ADDONS_AVUTIL_AVUTIL_H_
#define SRC_ADDONS_AVUTIL_AVUTIL_H_

#include <nan.h>

extern "C" {
#include "libavutil/avutil.h"
}

namespace ffmpeg {
namespace avutil {

extern void Init(v8::Handle<v8::Object> exports);

class AVMediaType {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetMediaTypeString);
};

class AVPictureType {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetPictureTypeChar);
};

}  // namespace avutil
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVUTIL_AVUTIL_H_
