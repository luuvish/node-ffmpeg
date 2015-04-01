#ifndef SRC_ADDONS_AVCODEC_AVSUBTITLERECT_H_
#define SRC_ADDONS_AVCODEC_AVSUBTITLERECT_H_

#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {

class AVSubtitleRect : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVSubtitleRect* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVSubtitleRect* This(::AVSubtitleRect* wrap = nullptr);

 private:
  explicit AVSubtitleRect() : this_(nullptr) {}
  ~AVSubtitleRect() {}
  ::AVSubtitleRect* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetX);
  static NAN_GETTER(GetY);
  static NAN_GETTER(GetW);
  static NAN_GETTER(GetH);
  static NAN_GETTER(GetNbColors);
  static NAN_GETTER(GetPict);
  static NAN_GETTER(GetType);
  static NAN_GETTER(GetText);
  static NAN_GETTER(GetAss);
  static NAN_GETTER(GetFlags);
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVSUBTITLERECT_H_
