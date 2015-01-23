#ifndef SRC_ADDONS_AVCODEC_AVPACKET_H_
#define SRC_ADDONS_AVCODEC_AVPACKET_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
namespace AVCodec {

class AVPacketWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVPacket *packet = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVPacket *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Init);
  static NAN_METHOD(Free);
  static NAN_GETTER(GetPts);
  static NAN_GETTER(GetDts);
  static NAN_GETTER(GetSize);
  static NAN_GETTER(GetStreamIndex);
  static NAN_GETTER(GetDuration);
  static NAN_GETTER(GetPos);
  explicit AVPacketWrapper(::AVPacket *packet = nullptr);
  virtual ~AVPacketWrapper();
  ::AVPacket *_this;
  bool _allocated;
};

}  // namespace AVCodec
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVCODEC_AVPACKET_H_
