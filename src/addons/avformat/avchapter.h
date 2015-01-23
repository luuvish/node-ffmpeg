#ifndef SRC_ADDONS_AVFORMAT_AVCHAPTER_H_
#define SRC_ADDONS_AVFORMAT_AVCHAPTER_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVChapterWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVChapter *chapter = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVChapter *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_GETTER(GetId);
  static NAN_GETTER(GetTimeBase);
  static NAN_GETTER(GetStart);
  static NAN_GETTER(GetEnd);
  static NAN_GETTER(GetMetadata);
  explicit AVChapterWrapper(::AVChapter *chapter = nullptr);
  virtual ~AVChapterWrapper();
  ::AVChapter *_this;
  bool _allocated;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVCHAPTER_H_
