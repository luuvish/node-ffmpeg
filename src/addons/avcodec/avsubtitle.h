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
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVSubtitle *This() { return this_; }

 private:
  explicit AVSubtitle(::AVSubtitle *ref = nullptr);
  virtual ~AVSubtitle();

  static NAN_METHOD(New);
  static NAN_METHOD(Free);
  static NAN_PROPERTY_GETTER(GetFormat);
  static NAN_PROPERTY_GETTER(GetStartDisplayTime);
  static NAN_PROPERTY_GETTER(GetEndDisplayTime);
  static NAN_PROPERTY_GETTER(GetRects);
  static NAN_PROPERTY_GETTER(GetPts);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVSubtitle *this_;
  bool alloc_;
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVSUBTITLE_H_
