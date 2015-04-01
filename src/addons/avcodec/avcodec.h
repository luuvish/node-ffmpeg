#ifndef SRC_ADDONS_AVCODEC_AVCODEC_H_
#define SRC_ADDONS_AVCODEC_AVCODEC_H_

#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {

extern void Init(v8::Handle<v8::Object> exports);

class AVCodecID {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetBitsPerSample);
  static NAN_METHOD(GetPcmCodec);
  static NAN_METHOD(GetExactBitsPerSample);
};

class AVDiscard {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVAudioServiceType {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVFieldOrder {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVSubtitleType {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVPictureStructure {
 public:
  static void Init(v8::Handle<v8::Object> exports);
};

class AVCodec : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVCodec* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVCodec* This(::AVCodec* wrap = nullptr);

 private:
  explicit AVCodec() : this_(nullptr) {}
  ~AVCodec() {}
  ::AVCodec* this_;

  static NAN_METHOD(GetCodecs);
  static NAN_METHOD(FindDecoder);
  static NAN_METHOD(FindEncoder);
  static NAN_METHOD(GetMediaType);
  static NAN_METHOD(GetCodecName);

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetIsEncoder);
  static NAN_GETTER(GetIsDecoder);
  static NAN_GETTER(GetName);
  static NAN_GETTER(GetLongName);
  static NAN_GETTER(GetType);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetCapabilities);
  static NAN_GETTER(GetSupportedFramerates);
  static NAN_GETTER(GetPixFmts);
  static NAN_GETTER(GetSupportedSamplerates);
  static NAN_GETTER(GetSampleFmts);
  static NAN_GETTER(GetChannelLayouts);
  static NAN_GETTER(GetMaxLowres);
  static NAN_GETTER(GetProfiles);
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVCODEC_H_
