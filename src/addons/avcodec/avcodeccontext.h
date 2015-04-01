#ifndef SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_
#define SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_

#include <nan.h>
#include <list>

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace ffmpeg {
namespace avcodec {
/*
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
*/
class AVCodecContext : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance(::AVCodecContext* wrap);
  static bool HasInstance(v8::Handle<v8::Value> value);
  ::AVCodecContext* This(::AVCodecContext* wrap = nullptr);

 private:
  explicit AVCodecContext() : this_(nullptr) {}
  ~AVCodecContext() {}
  ::AVCodecContext* this_;

  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(AllocContext3);
  static NAN_METHOD(FreeContext);
  static NAN_METHOD(GetContextDefault3);
  static NAN_METHOD(CopyContext);
  static NAN_METHOD(Open2);
  static NAN_METHOD(Close);
  static NAN_METHOD(DefaultGetBuffer2);
  static NAN_METHOD(DecodeAudio4);
  static NAN_METHOD(DecodeVideo2);
  static NAN_METHOD(DecodeSubtitle2);
  static NAN_METHOD(EncodeAudio2);
  static NAN_METHOD(EncodeVideo2);
  static NAN_METHOD(EncodeSubtitle);
  static NAN_METHOD(FillAudioFrame);
  static NAN_METHOD(FlushBuffers);
  static NAN_METHOD(GetAudioFrameDuration);
  static NAN_GETTER(GetIsOpen);
  static NAN_GETTER(GetCodecType);
  static NAN_SETTER(SetCodecType);
  static NAN_GETTER(GetCodec);
  static NAN_GETTER(GetCodecId);
  static NAN_SETTER(SetCodecId);
  static NAN_GETTER(GetBitRate);
  static NAN_SETTER(SetBitRate);
  static NAN_GETTER(GetBitRateTolerance);
  static NAN_SETTER(SetBitRateTolerance);
  static NAN_GETTER(GetGlobalQuality);
  static NAN_SETTER(SetGlobalQuality);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  static NAN_GETTER(GetFlags2);
  static NAN_SETTER(SetFlags2);
  static NAN_GETTER(GetExtradata);
  static NAN_SETTER(SetExtradata);
  static NAN_GETTER(GetTimeBase);
  static NAN_SETTER(SetTimeBase);
  static NAN_GETTER(GetDelay);
  static NAN_SETTER(SetDelay);
  static NAN_GETTER(GetWidth);
  static NAN_SETTER(SetWidth);
  static NAN_GETTER(GetHeight);
  static NAN_SETTER(SetHeight);
  static NAN_GETTER(GetCodedWidth);
  static NAN_SETTER(SetCodedWidth);
  static NAN_GETTER(GetCodedHeight);
  static NAN_SETTER(SetCodedHeight);
  static NAN_GETTER(GetGopSize);
  static NAN_SETTER(SetGopSize);
  static NAN_GETTER(GetPixFmt);
  static NAN_SETTER(SetPixFmt);
  static NAN_GETTER(GetMaxBFrames);
  static NAN_SETTER(SetMaxBFrames);
  static NAN_GETTER(GetHasBFrames);
  static NAN_GETTER(GetSampleAspectRatio);
  static NAN_SETTER(SetSampleAspectRatio);
  static NAN_GETTER(GetMbDecision);
  static NAN_SETTER(SetMbDecision);
  static NAN_GETTER(GetIntraMatrix);
  static NAN_SETTER(SetIntraMatrix);
  static NAN_GETTER(GetInterMatrix);
  static NAN_SETTER(SetInterMatrix);
  static NAN_GETTER(GetRefs);
  static NAN_SETTER(SetRefs);
  static NAN_GETTER(GetColorPrimaries);
  static NAN_SETTER(SetColorPrimaries);
  static NAN_GETTER(GetColorTrc);
  static NAN_SETTER(SetColorTrc);
  static NAN_GETTER(GetColorspace);
  static NAN_SETTER(SetColorspace);
  static NAN_GETTER(GetColorRange);
  static NAN_SETTER(SetColorRange);
  static NAN_GETTER(GetChromaSampleLocation);
  static NAN_SETTER(SetChromaSampleLocation);
  static NAN_GETTER(GetFieldOrder);
  static NAN_SETTER(SetFieldOrder);
  static NAN_GETTER(GetSampleRate);
  static NAN_SETTER(SetSampleRate);
  static NAN_GETTER(GetChannels);
  static NAN_SETTER(SetChannels);
  static NAN_GETTER(GetSampleFmt);
  static NAN_SETTER(SetSampleFmt);
  static NAN_GETTER(GetFrameSize);
  static NAN_SETTER(SetFrameSize);
  static NAN_GETTER(GetFrameNumber);
  static NAN_GETTER(GetBlockAlign);
  static NAN_SETTER(SetBlockAlign);
  static NAN_GETTER(GetChannelLayout);
  static NAN_SETTER(SetChannelLayout);
  static NAN_GETTER(GetRequestChannelLayout);
  static NAN_SETTER(SetRequestChannelLayout);
  static NAN_GETTER(GetAudioServiceType);
  static NAN_SETTER(SetAudioServiceType);
  static NAN_GETTER(GetRefcountedFrames);
  static NAN_SETTER(SetRefcountedFrames);
  static NAN_GETTER(GetQmin);
  static NAN_SETTER(SetQmin);
  static NAN_GETTER(GetQmax);
  static NAN_SETTER(SetQmax);
  static NAN_GETTER(GetRcBufferSize);
  static NAN_SETTER(SetRcBufferSize);
  static NAN_GETTER(GetRcMaxRate);
  static NAN_GETTER(GetTimecodeFrameStart);
  static NAN_SETTER(SetTimecodeFrameStart);
  static NAN_GETTER(GetStatsOut);
  static NAN_GETTER(GetStatsIn);
  static NAN_SETTER(SetStatsIn);
  static NAN_GETTER(GetStrictStdCompliance);
  static NAN_SETTER(SetStrictStdCompliance);
  static NAN_GETTER(GetBitsPerCodedSample);
  static NAN_SETTER(SetBitsPerCodedSample);
  static NAN_GETTER(GetBitsPerRawSample);
  static NAN_SETTER(SetBitsPerRawSample);
  static NAN_GETTER(GetCodedFrame);
  static NAN_GETTER(GetThreadCount);
  static NAN_SETTER(SetThreadCount);
  static NAN_GETTER(GetThreadType);
  static NAN_SETTER(SetThreadType);
  static NAN_GETTER(GetThreadSafeCallbacks);
  static NAN_SETTER(SetThreadSafeCallbacks);
  static NAN_GETTER(GetProfile);
  static NAN_SETTER(SetProfile);
  static NAN_GETTER(GetLevel);
  static NAN_SETTER(SetLevel);
  static NAN_GETTER(GetSubtitleHeader);
  static NAN_SETTER(SetSubtitleHeader);
  static NAN_GETTER(GetInitialPadding);
  static NAN_SETTER(SetInitialPadding);
  static NAN_GETTER(GetPktTimebase);
  static NAN_SETTER(SetPktTimebase);
  static NAN_GETTER(GetLowres);
  static NAN_SETTER(SetLowres);
  static NAN_GETTER(GetPtsCorrectionNumFaultyPts);
  static NAN_GETTER(GetPtsCorrectionNumFaultyDts);
  static NAN_GETTER(GetPtsCorrectionLastPts);
  static NAN_GETTER(GetPtsCorrectionLastDts);
  static NAN_GETTER(GetSubCharenc);
  static NAN_SETTER(SetSubCharenc);
  static NAN_GETTER(GetSubCharencMode);
  static NAN_SETTER(SetSubCharencMode);
  static NAN_GETTER(GetSkipAlpha);
  static NAN_SETTER(SetSkipAlpha);
  static NAN_GETTER(GetSeekPreroll);
  static NAN_SETTER(SetSeekPreroll);
  static NAN_GETTER(GetChromaIntraMatrix);
  static NAN_SETTER(SetChromaIntraMatrix);
  //std::list<NanAsyncWorker*> _async_queue;
};

}  // namespace avcodec
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVCODEC_AVCODECCONTEXT_H_
