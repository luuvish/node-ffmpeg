#ifndef SRC_ADDONS_AVCODEC_AVPACKET_H_
#define SRC_ADDONS_AVCODEC_AVPACKET_H_

#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {

class AVPacket : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVPacket *This() { return this_; }

 private:
  explicit AVPacket(::AVPacket *ref = nullptr);
  virtual ~AVPacket();

  static NAN_METHOD(New);
  static NAN_METHOD(Init);
  static NAN_METHOD(Free);
  static NAN_GETTER(GetPts);
  static NAN_GETTER(GetDts);
  static NAN_GETTER(GetData);
  static NAN_GETTER(GetSize);
  static NAN_GETTER(GetStreamIndex);
  static NAN_GETTER(GetFlags);
  static NAN_GETTER(GetDuration);
  static NAN_GETTER(GetPos);
  static NAN_GETTER(GetConvergenceDuration);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVPacket *this_;
  bool alloc_;
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVPACKET_H_
