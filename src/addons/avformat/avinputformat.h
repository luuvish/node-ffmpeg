#ifndef SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVInputFormat : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVInputFormat* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVInputFormat* This(::AVInputFormat* wrap = nullptr);

 private:
  explicit AVInputFormat() : this_(nullptr) {}
  ~AVInputFormat() {}
  ::AVInputFormat* this_;

  static NAN_METHOD(GetIFormats);
  static NAN_METHOD(FindInputFormat);

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetName);
  static NAN_GETTER(GetLongName);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static NAN_GETTER(GetExtensions);
  static NAN_GETTER(GetMimeType);
  static NAN_GETTER(GetRawCodecId);
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_
