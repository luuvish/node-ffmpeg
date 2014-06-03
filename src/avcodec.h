#ifndef _NODE_FFMPEG_AVCODEC_H_
#define _NODE_FFMPEG_AVCODEC_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
  class AVCodecWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVCodec *codec);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetName);
    static NAN_GETTER(GetLongName);
    static NAN_GETTER(GetType);
    static NAN_GETTER(GetId);
    AVCodecWrapper();
    ~AVCodecWrapper();
    AVCodec *_this;
  };

  class AVPictureWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVPicture *picture);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetData);
    static NAN_GETTER(GetLineSize);
    AVPictureWrapper();
    ~AVPictureWrapper();
    AVPicture *_this;
  };

  class AVSubtitleWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVSubtitle *subtitle);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetFormat);
    static NAN_GETTER(GetStartDisplayTime);
    static NAN_GETTER(GetEndDisplayTime);
    static NAN_GETTER(GetRects);
    static NAN_GETTER(GetPts);
    AVSubtitleWrapper();
    ~AVSubtitleWrapper();
    AVSubtitle *_this;
  };

  class AVCodecContextWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVCodecContext *ctx);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(Open);
    static NAN_METHOD(FindDecoder);
    static NAN_METHOD(FindDecoderByName);
    static NAN_GETTER(GetTimeBase);
    static NAN_GETTER(GetWidth);
    static NAN_GETTER(GetHeight);
    static NAN_GETTER(GetCodedWidth);
    static NAN_GETTER(GetCodedHeight);
    static NAN_GETTER(GetPixFmt);
    AVCodecContextWrapper();
    ~AVCodecContextWrapper();
    AVCodecContext *_this;
  };
}

#endif // _NODE_FFMPEG_AVCODEC_H_
