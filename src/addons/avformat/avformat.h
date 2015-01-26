#ifndef SRC_ADDONS_AVFORMAT_AVFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVFORMAT_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVFormat {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  static NAN_METHOD(Version);
  static NAN_METHOD(Configuration);
  static NAN_METHOD(License);
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVFORMAT_H_
