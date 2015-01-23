#ifndef SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVOutputFormatWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVOutputFormat *oformat = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVOutputFormat *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
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
  explicit AVOutputFormatWrapper(::AVOutputFormat *oformat = nullptr);
  virtual ~AVOutputFormatWrapper();
  ::AVOutputFormat *_this;
  bool _allocated;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVOUTPUTFORMAT_H_
