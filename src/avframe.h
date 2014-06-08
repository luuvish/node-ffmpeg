#ifndef _NODE_FFMPEG_AVFRAME_H_
#define _NODE_FFMPEG_AVFRAME_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavutil/frame.h"
}

namespace FFmpeg {
  class AVFrameWrapper : public node::ObjectWrap {
  friend class AVFormatContextWrapper;
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVFrame *frame=nullptr);
    static bool HasInstance(v8::Handle<v8::Object> obj);
    inline AVFrame *This() { return _this; }
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetWidth);
    static NAN_GETTER(GetHeight);
    explicit AVFrameWrapper(AVFrame *frame=nullptr);
    virtual ~AVFrameWrapper();
    AVFrame *_this;
  };
}

#endif // _NODE_FFMPEG_AVFRAME_H_
