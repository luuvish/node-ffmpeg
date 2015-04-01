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
  static v8::Local<v8::Object> NewInstance(::AVPacket* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVPacket* This(::AVPacket* wrap = nullptr);

 private:
  explicit AVPacket();
  ~AVPacket();
  ::AVPacket* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Dup);
  static NAN_METHOD(Copy);
  static NAN_METHOD(CopySideData);
  static NAN_METHOD(NewSideData);
  static NAN_METHOD(GetSideData);
  static NAN_METHOD(MergeSideData);
  static NAN_METHOD(SplitSideData);
  static NAN_METHOD(RescaleTs);
  static NAN_GETTER(GetPts);
  static NAN_SETTER(SetPts);
  static NAN_GETTER(GetDts);
  static NAN_SETTER(SetDts);
  static NAN_GETTER(GetData);
  static NAN_GETTER(GetSize);
  static NAN_GETTER(GetStreamIndex);
  static NAN_SETTER(SetStreamIndex);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static NAN_GETTER(GetDuration);
  static NAN_SETTER(SetDuration);
  static NAN_GETTER(GetPos);
  static NAN_SETTER(SetPos);
  static NAN_GETTER(GetConvergenceDuration);
  static NAN_SETTER(SetConvergenceDuration);
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVPACKET_H_
