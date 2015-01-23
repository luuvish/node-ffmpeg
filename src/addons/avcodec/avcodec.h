#ifndef SRC_ADDONS_AVCODEC_AVCODEC_H_
#define SRC_ADDONS_AVCODEC_AVCODEC_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
namespace AVCodec {

void Initialize(v8::Handle<v8::Object> target);

class AVCodecWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVCodec *codec = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVCodec *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(FindDecoder);
  static NAN_METHOD(FindEncoder);
  static NAN_METHOD(GetMediaType);
  static NAN_METHOD(GetCodecName);
  static NAN_GETTER(GetIsEncoder);
  static NAN_GETTER(GetIsDecoder);
  static NAN_GETTER(GetName);
  static NAN_GETTER(GetLongName);
  static NAN_GETTER(GetType);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetCapabilities);
  static NAN_GETTER(GetMaxLowres);
  explicit AVCodecWrapper(::AVCodec *codec = nullptr);
  virtual ~AVCodecWrapper();
  ::AVCodec *_this;
  bool _allocated;
};

}  // namespace AVCodec
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVCODEC_AVCODEC_H_
