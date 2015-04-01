#ifndef SRC_ADDONS_AVUTIL_AVFRAME_H_
#define SRC_ADDONS_AVUTIL_AVFRAME_H_

#include <nan.h>

extern "C" {
#include "libavutil/frame.h"
}

namespace ffmpeg {
namespace avutil {

class AVFrame : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVFrame* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVFrame* This(::AVFrame* wrap = nullptr);

 private:
  explicit AVFrame();
  ~AVFrame();
  ::AVFrame* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(Ref);
  static NAN_METHOD(Unref);
  static NAN_METHOD(MoveRef);
  static NAN_METHOD(GetBuffer);
  static NAN_METHOD(Copy);
  static NAN_METHOD(CopyProps);
  static NAN_GETTER(GetIsWritable);
  static NAN_SETTER(SetIsWritable);
  static NAN_GETTER(GetData);
  static NAN_GETTER(GetLinesize);
  static NAN_GETTER(GetWidth);
  static NAN_SETTER(SetWidth);
  static NAN_GETTER(GetHeight);
  static NAN_SETTER(SetHeight);
  static NAN_GETTER(GetNbSamples);
  static NAN_SETTER(SetNbSamples);
  static NAN_GETTER(GetFormat);
  static NAN_SETTER(SetFormat);
  static NAN_GETTER(GetKeyFrame);
  static NAN_GETTER(GetPictType);
  static NAN_SETTER(SetPictType);
  static NAN_GETTER(GetSampleAspectRatio);
  static NAN_SETTER(SetSampleAspectRatio);
  static NAN_GETTER(GetPts);
  static NAN_SETTER(SetPts);
  static NAN_GETTER(GetPktPts);
  static NAN_GETTER(GetPktDts);
  static NAN_GETTER(GetCodedPictureNumber);
  static NAN_GETTER(GetDisplayPictureNumber);
  static NAN_GETTER(GetQuality);
  static NAN_SETTER(SetQuality);
  static NAN_GETTER(GetRepeatPict);
  static NAN_SETTER(SetRepeatPict);
  static NAN_GETTER(GetInterlacedFrame);
  static NAN_SETTER(SetInterlacedFrame);
  static NAN_GETTER(GetTopFieldFirst);
  static NAN_SETTER(SetTopFieldFirst);
  static NAN_GETTER(GetPaletteHasChanged);
  static NAN_GETTER(GetReorderedOpaque);
  static NAN_GETTER(GetSampleRate);
  static NAN_SETTER(SetSampleRate);
  static NAN_GETTER(GetChannelLayout);
  static NAN_SETTER(SetChannelLayout);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static NAN_GETTER(GetColorRange);
  static NAN_SETTER(SetColorRange);
  static NAN_GETTER(GetColorPrimaries);
  static NAN_GETTER(GetColorTrc);
  static NAN_GETTER(GetColorspace);
  static NAN_SETTER(SetColorspace);
  static NAN_GETTER(GetChromaLocation);
  static NAN_GETTER(GetBestEffortTimestamp);
  static NAN_SETTER(SetBestEffortTimestamp);
  static NAN_GETTER(GetPktPos);
  static NAN_SETTER(SetPktPos);
  static NAN_GETTER(GetPktDuration);
  static NAN_SETTER(SetPktDuration);
  static NAN_GETTER(GetMetadata);
  static NAN_SETTER(SetMetadata);
  static NAN_GETTER(GetDecodeErrorFlags);
  static NAN_SETTER(SetDecodeErrorFlags);
  static NAN_GETTER(GetChannels);
  static NAN_SETTER(SetChannels);
  static NAN_GETTER(GetPktSize);
  static NAN_SETTER(SetPktSize);
};

}  // namespace avutil
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVUTIL_AVFRAME_H_
