#ifndef SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_
#define SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_

#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace ffmpeg {
namespace avformat {
/*
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
*/
class AVFormatContext : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Local<v8::Value> NewInstance(v8::Local<v8::Value> arg);
  static bool HasInstance(v8::Handle<v8::Value> value);
  inline ::AVFormatContext *This() { return this_; }

 private:
  explicit AVFormatContext(::AVFormatContext *ref = nullptr);
  virtual ~AVFormatContext();

  static NAN_METHOD(New);
  static NAN_METHOD(OpenInput);
  static NAN_METHOD(CloseInput);
  //static NAN_METHOD(FindStreamInfo);
  //static NAN_METHOD(FindProgramFromStream);
  //static NAN_METHOD(FindBestStream);
  static NAN_METHOD(ReadFrame);
  static NAN_METHOD(SeekFrame);
  static NAN_METHOD(SeekFile);
  static NAN_METHOD(ReadPlay);
  static NAN_METHOD(ReadPause);
  //static NAN_METHOD(WriteHeader);
  static NAN_METHOD(WriteFrame);
  static NAN_METHOD(WriteInterleavedFrame);
  //static NAN_METHOD(WriteUncodedFrame);
  //static NAN_METHOD(WriteUncodedInterleavedFrame);
  //static NAN_METHOD(WriteUncodedFrameQuery);
  static NAN_METHOD(WriteTrailer);
  //static NAN_METHOD(GetOutputTimestamp);
  static NAN_METHOD(DumpFormat);
  //static NAN_METHOD(GuessSampleAspectRatio);
  //static NAN_METHOD(GuessFrameRate);
  //static NAN_METHOD(MatchStreamSpecifier);
  //static NAN_METHOD(QueueAttachedPictures);
  //static NAN_GETTER(GetIFormat);
  //static NAN_GETTER(GetOFormat);
  static NAN_GETTER(GetCtxFlags);
  //static NAN_GETTER(GetStreams);
  static NAN_GETTER(GetFilename);
  static NAN_SETTER(SetFilename);
  static NAN_GETTER(GetStartTime);
  static NAN_SETTER(SetStartTime);
  static NAN_GETTER(GetDuration);
  static NAN_SETTER(SetDuration);
  static NAN_GETTER(GetBitRate);
  static NAN_GETTER(GetPacketSize);
  static NAN_GETTER(GetMaxDelay);
  static NAN_GETTER(GetFlags);
  static NAN_SETTER(SetFlags);
  //static NAN_GETTER(GetPrograms);
  static NAN_GETTER(GetVideoCodecId);
  static NAN_SETTER(SetVideoCodecId);
  static NAN_GETTER(GetAudioCodecId);
  static NAN_SETTER(SetAudioCodecId);
  static NAN_GETTER(GetSubtitleCodecId);
  static NAN_SETTER(SetSubtitleCodecId);
  static NAN_GETTER(GetMaxIndexSize);
  static NAN_SETTER(SetMaxIndexSize);
  static NAN_GETTER(GetMaxPictureBuffer);
  //static NAN_GETTER(GetChapters);
  static NAN_GETTER(GetMetadata);
  static NAN_GETTER(GetStartTimeRealtime);
  static NAN_SETTER(SetStartTimeRealtime);
  static NAN_GETTER(GetFpsProbeSize);
  static NAN_SETTER(SetFpsProbeSize);
  static NAN_GETTER(GetErrorRecognition);
  static NAN_SETTER(SetErrorRecognition);
  static NAN_GETTER(GetMaxInterleaveDelta);
  static NAN_SETTER(SetMaxInterleaveDelta);
  static NAN_GETTER(GetStrictStdCompliance);
  static NAN_SETTER(SetStrictStdCompliance);
  static NAN_GETTER(GetEventFlags);
  static NAN_SETTER(SetEventFlags);
  static NAN_GETTER(GetMaxTsProbe);
  static NAN_SETTER(SetMaxTsProbe);
  static NAN_GETTER(GetAvoidNegativeTs);
  static NAN_SETTER(SetAvoidNegativeTs);
  static NAN_GETTER(GetAudioPreload);
  static NAN_GETTER(GetMaxChunkDuration);
  static NAN_GETTER(GetMaxChunkSize);
  static NAN_GETTER(GetUseWallclockAsTimestamps);
  static NAN_GETTER(GetAvioFlags);
  static NAN_GETTER(GetDurationEstimationMethod);
  static NAN_GETTER(GetSkipInitialBytes);
  static NAN_GETTER(GetCorrectTsOverflow);
  static NAN_GETTER(GetSeek2Any);
  static NAN_GETTER(GetFlushPackets);
  static NAN_GETTER(GetProbeScore);
  static NAN_GETTER(GetFormatProbesize);
  static NAN_GETTER(GetCodecWhitelist);
  static NAN_GETTER(GetFormatWhitelist);
  static NAN_GETTER(GetDataOffset);
  static NAN_GETTER(GetRawPacketBufferRemainingSize);
  static NAN_GETTER(GetOffset);
  static NAN_GETTER(GetOffsetTimebase);
  static NAN_GETTER(GetIoRepositioned);
  //static NAN_GETTER(GetVideoCodec);
  //static NAN_SETTER(SetVideoCodec);
  //static NAN_GETTER(GetAudioCodec);
  //static NAN_SETTER(SetAudioCodec);
  //static NAN_GETTER(GetSubtitleCodec);
  //static NAN_SETTER(SetSubtitleCodec);
  static NAN_GETTER(GetMetadataHeaderPadding);
  static NAN_GETTER(GetOutputTsOffset);
  static NAN_GETTER(GetMaxAnalyzeDuration2);
  static NAN_GETTER(GetProbesize2);
  static NAN_GETTER(GetDumpSeparator);
  static v8::Persistent<v8::FunctionTemplate> constructor;

  ::AVFormatContext *this_;
  bool alloc_;
  //std::list<NanAsyncWorker*> _async_queue;
};

}  // namespace avformat
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVFORMATCONTEXT_H_
