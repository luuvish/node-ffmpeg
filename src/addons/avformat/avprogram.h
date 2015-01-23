#ifndef SRC_ADDONS_AVFORMAT_AVPROGRAM_H_
#define SRC_ADDONS_AVFORMAT_AVPROGRAM_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVProgramWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVProgram *program = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVProgram *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetDiscard);
  static NAN_GETTER(GetStreamIndexes);
  static NAN_GETTER(GetMetadata);
  static NAN_GETTER(GetProgramNum);
  static NAN_GETTER(GetStartTime);
  static NAN_GETTER(GetEndTime);
  explicit AVProgramWrapper(::AVProgram *program = nullptr);
  virtual ~AVProgramWrapper();
  ::AVProgram *_this;
  bool _allocated;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVPROGRAM_H_
