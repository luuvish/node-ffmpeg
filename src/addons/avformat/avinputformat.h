#ifndef SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVInputFormatWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVInputFormat *iformat = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVInputFormat *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(FindInputFormat);
  static NAN_GETTER(GetName);
  static NAN_GETTER(GetLongName);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  explicit AVInputFormatWrapper(::AVInputFormat *iformat = nullptr);
  virtual ~AVInputFormatWrapper();
  ::AVInputFormat *_this;
  bool _allocated;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVINPUTFORMAT_H_
