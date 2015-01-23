#ifndef SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_
#define SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace FFmpeg {
namespace AVCodec {

class AVDecodeAudioWorker : public NanAsyncWorker {
 public:
  explicit AVDecodeAudioWorker(std::list<NanAsyncWorker*> *q,
                               ::AVCodecContext *ctx,
                               ::AVFrame *pic,
                               ::AVPacket *pkt,
                               NanCallback *callback);
  virtual ~AVDecodeAudioWorker();
  void Execute();
  void HandleOKCallback();

 private:
  std::list<NanAsyncWorker*> *queue;
  ::AVCodecContext *context;
  ::AVFrame *frame;
  ::AVPacket *packet;
  int result;
  int got_frame_ptr;
};

class AVDecodeVideoWorker : public NanAsyncWorker {
 public:
  explicit AVDecodeVideoWorker(std::list<NanAsyncWorker*> *q,
                               ::AVCodecContext *ctx,
                               ::AVFrame *pic,
                               ::AVPacket *pkt,
                               NanCallback *callback);
  virtual ~AVDecodeVideoWorker();
  void Execute();
  void HandleOKCallback();

 private:
  std::list<NanAsyncWorker*> *queue;
  ::AVCodecContext *context;
  ::AVFrame *frame;
  ::AVPacket *packet;
  int result;
  int got_frame_ptr;
};

class AVDecodeSubtitleWorker : public NanAsyncWorker {
 public:
  explicit AVDecodeSubtitleWorker(std::list<NanAsyncWorker*> *q,
                                  ::AVCodecContext *ctx,
                                  ::AVSubtitle *sub,
                                  ::AVPacket *pkt,
                                  NanCallback *callback);
  virtual ~AVDecodeSubtitleWorker();
  void Execute();
  void HandleOKCallback();

 private:
  std::list<NanAsyncWorker*> *queue;
  ::AVCodecContext *context;
  ::AVSubtitle *subtt;
  ::AVPacket *packet;
  int result;
  int got_subtt_ptr;
};

class AVCodecContextWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVCodecContext *ctx = nullptr);
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
  static NAN_GETTER(GetCodecId);
  static NAN_GETTER(GetFlags);
  static NAN_GETTER(GetFlags2);
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
  static NAN_SETTER(SetFlags);
  static NAN_SETTER(SetFlags2);
  static NAN_SETTER(SetWorkaroundBugs);
  static NAN_SETTER(SetErrorConcealment);
  static NAN_SETTER(SetLowres);
  explicit AVCodecContextWrapper(::AVCodecContext *ctx = nullptr);
  virtual ~AVCodecContextWrapper();
  ::AVCodecContext *_this;
  bool _allocated;
  std::list<NanAsyncWorker*> _async_queue;
};

}  // namespace AVCodec
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_
