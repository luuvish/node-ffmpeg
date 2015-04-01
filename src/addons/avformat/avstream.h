#ifndef SRC_ADDONS_AVFORMAT_AVSTREAM_H_
#define SRC_ADDONS_AVFORMAT_AVSTREAM_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVStream : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVStream* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVStream* This(::AVStream* wrap = nullptr);

 private:
  explicit AVStream() : this_(nullptr) {}
  ~AVStream() {}
  ::AVStream* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(SearchIndexTimestamp);
  static NAN_METHOD(AddIndexEntry);
  static NAN_METHOD(GetEndPts);
  static NAN_GETTER(GetIndex);
  static NAN_SETTER(SetIndex);
  static NAN_GETTER(GetId);
  static NAN_SETTER(SetId);
  static NAN_GETTER(GetCodec);
  static NAN_GETTER(GetTimeBase);
  static NAN_SETTER(SetTimeBase);
  static NAN_GETTER(GetStartTime);
  static NAN_GETTER(GetDuration);
  static NAN_GETTER(GetNbFrames);
  static NAN_GETTER(GetDisposition);
  static NAN_SETTER(SetDisposition);
  static NAN_GETTER(GetDiscard);
  static NAN_SETTER(SetDiscard);
  static NAN_GETTER(GetSampleAspectRatio);
  static NAN_SETTER(SetSampleAspectRatio);
  static NAN_GETTER(GetMetadata);
  static NAN_GETTER(GetAvgFrameRate);
  static NAN_SETTER(SetAvgFrameRate);
  static NAN_GETTER(GetEventFlags);
  static NAN_GETTER(GetPtsWrapBits);
  static NAN_GETTER(GetFirstDts);
  static NAN_GETTER(GetCurDts);
  static NAN_GETTER(GetLastIPPts);
  static NAN_GETTER(GetLastIPDuration);
  static NAN_GETTER(GetRFrameRate);
  static NAN_SETTER(SetRFrameRate);
  static NAN_GETTER(GetRecommendedEncoderConfiguration);
  static NAN_SETTER(SetRecommendedEncoderConfiguration);
  static NAN_GETTER(GetDisplayAspectRatio);
  static NAN_SETTER(SetDisplayAspectRatio);
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVSTREAM_H_
