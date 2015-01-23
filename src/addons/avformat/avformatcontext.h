#ifndef SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_
#define SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

class AVReadFrameWorker : public NanAsyncWorker {
 public:
  explicit AVReadFrameWorker(std::list<NanAsyncWorker*> *q,
                             ::AVFormatContext *ctx, ::AVPacket *pkt,
                             NanCallback *callback);
  virtual ~AVReadFrameWorker();
  void Execute();
  void HandleOKCallback();

 private:
  std::list<NanAsyncWorker*> *queue;
  ::AVFormatContext *context;
  ::AVPacket *packet;
  int result;
};

class AVFormatContextWrapper : public node::ObjectWrap {
 public:
  static void Initialize(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> newInstance(::AVFormatContext *ctx = nullptr);
  static bool HasInstance(v8::Handle<v8::Object> obj);
  inline ::AVFormatContext *This() { return _this; }

 private:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(OpenInput);
  static NAN_METHOD(CloseInput);
  static NAN_METHOD(FindStreamInfo);
  static NAN_METHOD(FindProgramFromStream);
  static NAN_METHOD(FindBestStream);
  static NAN_METHOD(ReadFrame);
  static NAN_METHOD(SeekFrame);
  static NAN_METHOD(SeekFile);
  static NAN_METHOD(ReadPlay);
  static NAN_METHOD(ReadPause);
  static NAN_METHOD(WriteHeader);
  static NAN_METHOD(WriteFrame);
  static NAN_METHOD(WriteTrailer);
  static NAN_METHOD(DumpFormat);
  static NAN_METHOD(GuessSampleAspectRatio);
  static NAN_METHOD(GuessFrameRate);
  static NAN_GETTER(GetIFormat);
  static NAN_GETTER(GetOFormat);
  static NAN_GETTER(GetStreams);
  static NAN_GETTER(GetFilename);
  static NAN_GETTER(GetStartTime);
  static NAN_GETTER(GetDuration);
  static NAN_GETTER(GetFlags);
  static NAN_GETTER(GetPrograms);
  static NAN_GETTER(GetVideoCodecId);
  static NAN_GETTER(GetAudioCodecId);
  static NAN_GETTER(GetSubtitleCodecId);
  static NAN_GETTER(GetChapters);
  static NAN_GETTER(GetMetadata);
  static NAN_GETTER(GetVideoCodec);
  static NAN_GETTER(GetAudioCodec);
  static NAN_GETTER(GetSubtitleCodec);
  static NAN_SETTER(SetFlags);
  static NAN_SETTER(SetVideoCodec);
  static NAN_SETTER(SetAudioCodec);
  static NAN_SETTER(SetSubtitleCodec);
  explicit AVFormatContextWrapper(::AVFormatContext *ctx = nullptr);
  virtual ~AVFormatContextWrapper();
  ::AVFormatContext *_this;
  bool _allocated;
  std::list<NanAsyncWorker*> _async_queue;
};

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_
