#ifndef SRC_ADDONS_AVFORMAT_AVCHAPTER_H_
#define SRC_ADDONS_AVFORMAT_AVCHAPTER_H_

#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {

class AVChapter : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVChapter *This() { return this_; }

 private:
  explicit AVChapter(::AVChapter *ref = nullptr);
  virtual ~AVChapter();

  static NAN_METHOD(New);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetTimeBase);
  static NAN_GETTER(GetStart);
  static NAN_GETTER(GetEnd);
  static NAN_GETTER(GetMetadata);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVChapter *this_;
  bool alloc_;
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVCHAPTER_H_
