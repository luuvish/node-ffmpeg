#ifndef SRC_ADDONS_AVUTIL_CHANNEL_LAYOUT_H_
#define SRC_ADDONS_AVUTIL_CHANNEL_LAYOUT_H_

#include <nan.h>

extern "C" {
#include "libavutil/channel_layout.h"
}

namespace ffmpeg {
namespace avutil {

class ChannelLayout {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetChannelLayout);
  static NAN_METHOD(GetChannelLayoutString);
  static NAN_METHOD(GetChannelLayoutNbChannels);
  static NAN_METHOD(GetDefaultChannelLayout);
  static NAN_METHOD(GetChannelLayoutChannelIndex);
  static NAN_METHOD(ExtractChannelLayout);
  static NAN_METHOD(GetChannelName);
  static NAN_METHOD(GetChannelDescription);
  static NAN_METHOD(GetStandardChannelLayout);
};

class AVMatrixEncoding {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

}  // namespace avutil
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVUTIL_CHANNEL_LAYOUT_H_
