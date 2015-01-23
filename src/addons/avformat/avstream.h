#ifndef SRC_ADDONS_AVFORMAT_AVSTREAM_H_
#define SRC_ADDONS_AVFORMAT_AVSTREAM_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVStreamWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVStream *stream = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVStream *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetIndex);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetCodec);
  static NAN_GETTER(GetTimeBase);
  static NAN_GETTER(GetStartTime);
  static NAN_GETTER(GetDuration);
  static NAN_GETTER(GetDiscard);
  static NAN_GETTER(GetSampleAspectRatio);
  static NAN_GETTER(GetMetadata);
  static NAN_SETTER(SetDiscard);
  explicit AVStreamWrapper(::AVStream *stream = nullptr);
  virtual ~AVStreamWrapper();
  ::AVStream *_this;
  bool _allocated;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVSTREAM_H_
