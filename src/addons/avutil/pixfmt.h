#ifndef SRC_ADDONS_AVUTIL_PIXFMT_H_
#define SRC_ADDONS_AVUTIL_PIXFMT_H_

#include <nan.h>

extern "C" {
#include "libavutil/pixfmt.h"
}

namespace ffmpeg {
namespace avutil {

class AVPixelFormat {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVColorPrimaries {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVColorTransferCharacteristic {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVColorSpace {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetColorspaceName);
};

class AVColorRange {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVChromaLocation {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

}  // namespace avutil
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVUTIL_PIXFMT_H_
