#ifndef SRC_ADDONS_AVCODEC_AVSUBTITLE_H_
#define SRC_ADDONS_AVCODEC_AVSUBTITLE_H_

#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {

class AVSubtitle : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVSubtitle* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVSubtitle* This(::AVSubtitle* wrap = nullptr);

 private:
  explicit AVSubtitle();
  ~AVSubtitle();
  ::AVSubtitle* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Free);
  static NAN_GETTER(GetFormat);
  static NAN_SETTER(SetFormat);
  static NAN_GETTER(GetStartDisplayTime);
  static NAN_SETTER(SetStartDisplayTime);
  static NAN_GETTER(GetEndDisplayTime);
  static NAN_SETTER(SetEndDisplayTime);
  static NAN_GETTER(GetRects);
  static NAN_GETTER(GetPts);
  static NAN_SETTER(SetPts);
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVSUBTITLE_H_
