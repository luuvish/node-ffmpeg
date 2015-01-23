#ifndef SRC_ADDONS_AVCODEC_AVPICTURE_H_
#define SRC_ADDONS_AVCODEC_AVPICTURE_H_

#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {

class AVPicture : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg[3]);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVPicture *This() { return this_; }

 private:
  explicit AVPicture(::AVPicture *ref = nullptr, int w = 0, int h = 0);
  virtual ~AVPicture();

  static NAN_METHOD(New);
  static NAN_METHOD(Alloc);
  static NAN_METHOD(Free);
  static NAN_METHOD(Fill);
  static NAN_METHOD(Layout);
  static NAN_METHOD(GetSize);
  static NAN_METHOD(Copy);
  static NAN_METHOD(Crop);
  static NAN_METHOD(Pad);
  static NAN_PROPERTY_GETTER(GetData);
  static NAN_PROPERTY_GETTER(GetLinesize);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVPicture *this_;
  bool alloc_;
  int w_, h_;
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVPICTURE_H_
