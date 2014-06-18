#ifndef _NODE_FFMPEG_AVFORMAT_H_
#define _NODE_FFMPEG_AVFORMAT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
  namespace AVFormat {

    void Initialize(v8::Handle<v8::Object> target);

    class AVOutputFormatWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVOutputFormat *oformat=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVOutputFormat *This() { return _this; }
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
      explicit AVOutputFormatWrapper(::AVOutputFormat *oformat=nullptr);
      virtual ~AVOutputFormatWrapper();
      ::AVOutputFormat *_this;
      bool _allocated;
    };

    class AVInputFormatWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVInputFormat *iformat=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVInputFormat *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_METHOD(FindInputFormat);
      static NAN_GETTER(GetName);
      static NAN_GETTER(GetLongName);
      explicit AVInputFormatWrapper(::AVInputFormat *iformat=nullptr);
      virtual ~AVInputFormatWrapper();
      ::AVInputFormat *_this;
      bool _allocated;
    };

    class AVStreamWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVStream *stream=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVStream *This() { return _this; }
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
      static NAN_SETTER(SetDiscard);
      explicit AVStreamWrapper(::AVStream *stream=nullptr);
      virtual ~AVStreamWrapper();
      ::AVStream *_this;
      bool _allocated;
    };

    class AVProgramWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVProgram *program=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVProgram *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_GETTER(GetId);
      static NAN_GETTER(GetDiscard);
      static NAN_GETTER(GetStreamIndexes);
      static NAN_GETTER(GetProgramNum);
      static NAN_GETTER(GetStartTime);
      static NAN_GETTER(GetEndTime);
      explicit AVProgramWrapper(::AVProgram *program=nullptr);
      virtual ~AVProgramWrapper();
      ::AVProgram *_this;
      bool _allocated;
    };

    class AVChapterWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVChapter *chapter=nullptr);
      static bool HasInstance(v8::Handle<v8::Object> obj);
      inline ::AVChapter *This() { return _this; }
    private:
      static v8::Persistent<v8::FunctionTemplate> constructor;
      static NAN_METHOD(New);
      static NAN_GETTER(GetId);
      static NAN_GETTER(GetTimeBase);
      static NAN_GETTER(GetStart);
      static NAN_GETTER(GetEnd);
      explicit AVChapterWrapper(::AVChapter *chapter=nullptr);
      virtual ~AVChapterWrapper();
      ::AVChapter *_this;
      bool _allocated;
    };

    class AVReadFrameWorker : public NanAsyncWorker {
    public:
      explicit AVReadFrameWorker(std::list<NanAsyncWorker*> &q,
                                 ::AVFormatContext *ctx, ::AVPacket *pkt,
                                 NanCallback *callback);
      virtual ~AVReadFrameWorker();
      void Execute();
      void HandleOKCallback();
    private:
      std::list<NanAsyncWorker*> &queue;
      ::AVFormatContext *context;
      ::AVPacket *packet;
      int result;
    };

    class AVFormatContextWrapper : public node::ObjectWrap {
    public:
      static void Initialize(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> newInstance(::AVFormatContext *ctx=nullptr);
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
      explicit AVFormatContextWrapper(::AVFormatContext *ctx=nullptr);
      virtual ~AVFormatContextWrapper();
      ::AVFormatContext *_this;
      bool _allocated;
      std::list<NanAsyncWorker*> _async_queue;
    };

  }
}

#endif // _NODE_FFMPEG_AVFORMAT_H_
