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
  static v8::Local<v8::Object> NewInstance(::AVChapter* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVChapter* This(::AVChapter* wrap = nullptr);

 private:
  explicit AVChapter() : this_(nullptr) {}
  ~AVChapter() {}
  ::AVChapter* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetTimeBase);
  static NAN_GETTER(GetStart);
  static NAN_GETTER(GetEnd);
  static NAN_GETTER(GetMetadata);
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVCHAPTER_H_
