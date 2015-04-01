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
  static v8::Local<v8::Object> NewInstance(::AVPicture* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVPicture* This(::AVPicture* wrap = nullptr);

 private:
  explicit AVPicture();
  ~AVPicture();
  ::AVPicture* this_;
  enum ::AVPixelFormat pix_fmt_;
  int width_;
  int height_;

  static NAN_METHOD(GetSize);

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Alloc);
  static NAN_METHOD(Free);
  static NAN_METHOD(Fill);
  static NAN_METHOD(Layout);
  static NAN_METHOD(Copy);
  static NAN_METHOD(Crop);
  static NAN_METHOD(Pad);
  static NAN_GETTER(GetData);
  static NAN_GETTER(GetLinesize);
  static NAN_GETTER(GetPixFmt);
  static NAN_SETTER(SetPixFmt);
  static NAN_GETTER(GetWidth);
  static NAN_SETTER(SetWidth);
  static NAN_GETTER(GetHeight);
  static NAN_SETTER(SetHeight);
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVPICTURE_H_
