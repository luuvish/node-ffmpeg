#ifndef SRC_ADDONS_AVFORMAT_AVFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVFORMAT_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

extern void Init(v8::Handle<v8::Object> exports);

class AVStreamParseType {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVDurationEstimationMethod {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVFORMAT_H_
