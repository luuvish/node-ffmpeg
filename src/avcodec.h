#ifndef _NODE_FFMPEG_AVCODEC_H_
#define _NODE_FFMPEG_AVCODEC_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
  class AVPacketWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVPacket *packet);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetPts);
    static NAN_GETTER(GetDts);
    static NAN_GETTER(GetSize);
    static NAN_GETTER(GetStreamIndex);
    static NAN_GETTER(GetDuration);
    static NAN_GETTER(GetPos);
    AVPacketWrapper();
    ~AVPacketWrapper();
    AVPacket *_this;
  };

  class AVCodecContextWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVCodecContext *ctx);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(Open);
    static NAN_METHOD(Close);
    static NAN_METHOD(DecodeAudio);
    static NAN_METHOD(DecodeVideo);
    static NAN_METHOD(DecodeSubtitle);
    static NAN_METHOD(EncodeAudio);
    static NAN_METHOD(EncodeVideo);
    static NAN_METHOD(EncodeSubtitle);
    static NAN_METHOD(FlushBuffers);
    static NAN_GETTER(GetIsOpen);
    static NAN_GETTER(GetCodecType);
    static NAN_GETTER(GetCodec);
    static NAN_GETTER(GetCodecName);
    static NAN_GETTER(GetCodecId);
    static NAN_GETTER(GetTimeBase);
    static NAN_GETTER(GetWidth);
    static NAN_GETTER(GetHeight);
    static NAN_GETTER(GetPixFmt);
    static NAN_GETTER(GetSampleRate);
    static NAN_GETTER(GetChannels);
    static NAN_GETTER(GetSampleFmt);
    static NAN_GETTER(GetChannelLayout);
    static NAN_GETTER(GetWorkaroundBugs);
    static NAN_GETTER(GetErrorConcealment);
    static NAN_GETTER(GetLowres);
    static NAN_SETTER(SetWorkaroundBugs);
    static NAN_SETTER(SetErrorConcealment);
    static NAN_SETTER(SetLowres);
    AVCodecContextWrapper();
    ~AVCodecContextWrapper();
    AVCodecContext *_this;
  };

  class AVCodecWrapper : public node::ObjectWrap {
  friend class AVCodecContextWrapper;
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVCodec *codec);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(FindDecoder);
    static NAN_METHOD(FindEncoder);
    static NAN_METHOD(GetMediaType);
    static NAN_METHOD(GetCodecName);
    static NAN_GETTER(GetIsEncoder);
    static NAN_GETTER(GetIsDecoder);
    static NAN_GETTER(GetName);
    static NAN_GETTER(GetLongName);
    static NAN_GETTER(GetType);
    static NAN_GETTER(GetId);
    static NAN_GETTER(GetMaxLowres);
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

  class AVSubtitleRectWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVSubtitleRect *rect);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetX);
    static NAN_GETTER(GetY);
    static NAN_GETTER(GetW);
    static NAN_GETTER(GetH);
    static NAN_GETTER(GetNbColors);
    static NAN_GETTER(GetPict);
    static NAN_GETTER(GetType);
    static NAN_GETTER(GetText);
    static NAN_GETTER(GetAss);
    AVSubtitleRectWrapper();
    ~AVSubtitleRectWrapper();
    AVSubtitleRect *_this;
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
}

#endif // _NODE_FFMPEG_AVCODEC_H_
