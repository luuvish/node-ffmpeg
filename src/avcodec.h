#ifndef _NODE_FFMPEG_AVCODEC_H_
#define _NODE_FFMPEG_AVCODEC_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
  namespace AVCodec {

    void Initialize(v8::Handle<v8::Object> target);

    class AVPacketWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVPacket *packet=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVPacket *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_METHOD(Init);
      static NAN_METHOD(Free);
      static NAN_GETTER(GetPts);
      static NAN_GETTER(GetDts);
      static NAN_GETTER(GetSize);
      static NAN_GETTER(GetStreamIndex);
      static NAN_GETTER(GetDuration);
      static NAN_GETTER(GetPos);
      explicit AVPacketWrapper(::AVPacket *packet=nullptr);
      virtual ~AVPacketWrapper();
      ::AVPacket *_this;
      bool _allocated;
    };

    class AVCodecContextWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVCodecContext *ctx=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVCodecContext *This() { return _this; }
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
      explicit AVCodecContextWrapper(::AVCodecContext *ctx=nullptr);
      virtual ~AVCodecContextWrapper();
      ::AVCodecContext *_this;
      bool _allocated;
    };

    class AVCodecWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVCodec *codec=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVCodec *This() { return _this; }
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
      explicit AVCodecWrapper(::AVCodec *codec=nullptr);
      virtual ~AVCodecWrapper();
      ::AVCodec *_this;
      bool _allocated;
    };

    class AVPictureWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVPicture *picture=nullptr, int w=0, int h=0);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVPicture *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_GETTER(GetData);
      static NAN_GETTER(GetLinesize);
      explicit AVPictureWrapper(::AVPicture *picture=nullptr, int w=0, int h=0);
      virtual ~AVPictureWrapper();
      ::AVPicture *_this;
      bool _allocated;
      int _w, _h;
    };

    class AVSubtitleRectWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVSubtitleRect *rect=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVSubtitleRect *This() { return _this; }
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
      explicit AVSubtitleRectWrapper(::AVSubtitleRect *rect=nullptr);
      virtual ~AVSubtitleRectWrapper();
      ::AVSubtitleRect *_this;
      bool _allocated;
    };

    class AVSubtitleWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVSubtitle *subtitle=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVSubtitle *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_METHOD(Free);
      static NAN_GETTER(GetFormat);
      static NAN_GETTER(GetStartDisplayTime);
      static NAN_GETTER(GetEndDisplayTime);
      static NAN_GETTER(GetRects);
      static NAN_GETTER(GetPts);
      explicit AVSubtitleWrapper(::AVSubtitle *subtitle=nullptr);
      virtual ~AVSubtitleWrapper();
      ::AVSubtitle *_this;
      bool _allocated;
    };

  }
}

#endif // _NODE_FFMPEG_AVCODEC_H_
