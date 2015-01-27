#ifndef SRC_ADDONS_AVFORMAT_AVPROGRAM_H_
#define SRC_ADDONS_AVFORMAT_AVPROGRAM_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVProgram : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVProgram *This() { return this_; }

 private:
  explicit AVProgram(::AVProgram *ref = nullptr);
  virtual ~AVProgram();

  static NAN_METHOD(New);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static NAN_GETTER(GetDiscard);
  static NAN_SETTER(SetDiscard);
  static NAN_GETTER(GetStreamIndexes);
  static NAN_GETTER(GetMetadata);
  static NAN_GETTER(GetProgramNum);
  static NAN_SETTER(SetProgramNum);
  static NAN_GETTER(GetPmtPid);
  static NAN_SETTER(SetPmtPid);
  static NAN_GETTER(GetPcrPid);
  static NAN_SETTER(SetPcrPid);
  static NAN_GETTER(GetStartTime);
  static NAN_GETTER(GetEndTime);
  static NAN_GETTER(GetPtsWrapReference);
  static NAN_GETTER(GetPtsWrapBehavior);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVProgram *this_;
  bool alloc_;
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVPROGRAM_H_
