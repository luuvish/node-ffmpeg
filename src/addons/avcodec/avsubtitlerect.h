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
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVSubtitleRect *This() { return this_; }

 private:
  explicit AVSubtitleRect(::AVSubtitleRect *rect = nullptr);
  virtual ~AVSubtitleRect();

  static NAN_METHOD(New);
  static NAN_PROPERTY_GETTER(GetX);
  static NAN_PROPERTY_GETTER(GetY);
  static NAN_PROPERTY_GETTER(GetW);
  static NAN_PROPERTY_GETTER(GetH);
  static NAN_PROPERTY_GETTER(GetNbColors);
  static NAN_PROPERTY_GETTER(GetPict);
  static NAN_PROPERTY_GETTER(GetType);
  static NAN_PROPERTY_GETTER(GetText);
  static NAN_PROPERTY_GETTER(GetAss);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVSubtitleRect *this_;
  bool alloc_;
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVSUBTITLERECT_H_
