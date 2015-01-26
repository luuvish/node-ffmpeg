#ifndef SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVOutputFormat : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Value> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVOutputFormat *This() { return this_; }

 private:
  explicit AVOutputFormat(::AVOutputFormat *ref = nullptr);
  virtual ~AVOutputFormat();

  static NAN_METHOD(New);
  static NAN_METHOD(GuessFormat);
  static NAN_METHOD(GuessCodec);
  static NAN_METHOD(QueryCodec);
  static NAN_GETTER(GetName);
  static NAN_GETTER(GetLongName);
  static NAN_GETTER(GetMimeType);
  static NAN_GETTER(GetExtensions);
  static NAN_GETTER(GetAudioCodec);
  static NAN_GETTER(GetVideoCodec);
  static NAN_GETTER(GetSubtitleCodec);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVOutputFormat *this_;
  bool alloc_;
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_
