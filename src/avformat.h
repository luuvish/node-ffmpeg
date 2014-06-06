#ifndef _NODE_FFMPEG_AVFORMAT_H_
#define _NODE_FFMPEG_AVFORMAT_H_

#include <node.h>
#include <nan.h>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
  class AVOutputFormatWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVOutputFormat *oformat);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(GuessFormat);
    static NAN_METHOD(GuessCodec);
    static NAN_METHOD(QueryCodec);
    static NAN_GETTER(GetName);
    static NAN_GETTER(GetLongName);
    static NAN_GETTER(GetMimeType);
    static NAN_GETTER(GetExtensions);
    static NAN_GETTER(GetAudioCodec);
    static NAN_GETTER(GetVideoCodec);
    static NAN_GETTER(GetSubtitleCodec);
    AVOutputFormatWrapper();
    ~AVOutputFormatWrapper();
    AVOutputFormat *_this;
  };

  class AVInputFormatWrapper : public node::ObjectWrap {
  friend class AVFormatContextWrapper;
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVInputFormat *iformat);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(FindInputFormat);
    static NAN_GETTER(GetName);
    static NAN_GETTER(GetLongName);
    AVInputFormatWrapper();
    ~AVInputFormatWrapper();
    AVInputFormat *_this;
  };

  class AVStreamWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVStream *stream);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetIndex);
    static NAN_GETTER(GetId);
    static NAN_GETTER(GetCodec);
    static NAN_GETTER(GetPts);
    static NAN_GETTER(GetTimeBase);
    static NAN_GETTER(GetStartTime);
    static NAN_GETTER(GetDuration);
    static NAN_GETTER(GetDiscard);
    static NAN_GETTER(GetSampleAspectRatio);
    AVStreamWrapper();
    ~AVStreamWrapper();
    AVStream *_this;
  };

  class AVProgramWrapper : public node::ObjectWrap {
  friend class AVFormatContextWrapper;
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVProgram *program);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetId);
    static NAN_GETTER(GetDiscard);
    static NAN_GETTER(GetStreamIndex);
    static NAN_GETTER(GetProgramNum);
    static NAN_GETTER(GetStartTime);
    static NAN_GETTER(GetEndTime);
    AVProgramWrapper();
    ~AVProgramWrapper();
    AVProgram *_this;
  };

  class AVChapterWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> newInstance(AVChapter *chapter);
  private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static NAN_METHOD(New);
    static NAN_GETTER(GetId);
    static NAN_GETTER(GetTimeBase);
    static NAN_GETTER(GetStart);
    static NAN_GETTER(GetEnd);
    AVChapterWrapper();
    ~AVChapterWrapper();
    AVChapter *_this;
  };

  class AVFormatContextWrapper : public node::ObjectWrap {
  public:
    static void Initialize(v8::Handle<v8::Object> target);
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
    static NAN_GETTER(GetIFormat);
    static NAN_GETTER(GetOFormat);
    static NAN_GETTER(GetStreams);
    static NAN_GETTER(GetFilename);
    static NAN_GETTER(GetStartTime);
    static NAN_GETTER(GetDuration);
    static NAN_GETTER(GetPrograms);
    static NAN_GETTER(GetVideoCodecId);
    static NAN_GETTER(GetAudioCodecId);
    static NAN_GETTER(GetSubtitleCodecId);
    static NAN_GETTER(GetChapters);
    static NAN_GETTER(GetVideoCodec);
    static NAN_GETTER(GetAudioCodec);
    static NAN_GETTER(GetSubtitleCodec);
    static NAN_SETTER(SetVideoCodec);
    static NAN_SETTER(SetAudioCodec);
    static NAN_SETTER(SetSubtitleCodec);
    AVFormatContextWrapper();
    ~AVFormatContextWrapper();
    AVFormatContext *_this;
  };
}

#endif // _NODE_FFMPEG_AVFORMAT_H_
