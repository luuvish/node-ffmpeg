#ifndef SRC_ADDONS_AVUTIL_H_
#define SRC_ADDONS_AVUTIL_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavutil/channel_layout.h"
#include "libavutil/frame.h"
#include "libavutil/samplefmt.h"
}

namespace FFmpeg {
namespace AVUtil {

void Initialize(v8::Handle<v8::Object> target);

NAN_METHOD(GetChannelLayoutNbChannels);
NAN_METHOD(GetDefaultChannelLayout);
NAN_METHOD(GetPackedSampleFmt);
NAN_METHOD(GetPlanarSampleFmt);
NAN_METHOD(GetBytesPerSample);
NAN_METHOD(GetSamplesBufferSize);

class AVFrameWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVFrame *frame = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVFrame *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Unref);
  static NAN_GETTER(GetData);
  static NAN_GETTER(GetLinesize);
  static NAN_GETTER(GetWidth);
  static NAN_GETTER(GetHeight);
  static NAN_GETTER(GetNbSamples);
  static NAN_GETTER(GetFormat);
  static NAN_GETTER(GetSampleAspectRatio);
  static NAN_GETTER(GetPts);
  static NAN_GETTER(GetPktPts);
  static NAN_GETTER(GetPktDts);
  static NAN_GETTER(GetBestEffortTimestamp);
  static NAN_GETTER(GetPktDuration);
  static NAN_GETTER(GetPktPos);
  static NAN_GETTER(GetChannelLayout);
  static NAN_GETTER(GetChannels);
  static NAN_GETTER(GetSampleRate);
  static NAN_GETTER(GetDecodeErrorFlags);
  static NAN_GETTER(GetPktSize);
  static NAN_GETTER(GetColorspace);
  static NAN_GETTER(GetColorRange);
  static NAN_SETTER(SetSampleAspectRatio);
  static NAN_SETTER(SetPts);
  static NAN_SETTER(SetBestEffortTimestamp);
  static NAN_SETTER(SetPktDuration);
  static NAN_SETTER(SetPktPos);
  static NAN_SETTER(SetChannelLayout);
  static NAN_SETTER(SetChannels);
  static NAN_SETTER(SetSampleRate);
  static NAN_SETTER(SetDecodeErrorFlags);
  static NAN_SETTER(SetPktSize);
  static NAN_SETTER(SetColorspace);
  static NAN_SETTER(SetColorRange);
  explicit AVFrameWrapper(::AVFrame *frame = nullptr);
  virtual ~AVFrameWrapper();
  ::AVFrame *_this;
  bool _allocated;
};

}  // namespace AVUtil
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVUTIL_H_
