#include "avformat/avformatcontext.h"
#include "avformat/avinputformat.h"
#include "avformat/avoutputformat.h"
#include "avformat/avprogram.h"
#include "avformat/avchapter.h"
#include "avformat/avstream.h"
#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


FFmpeg::AVFormat::AVReadFrameWorker::
AVReadFrameWorker(std::list<NanAsyncWorker*> *q,
                  ::AVFormatContext *ctx, ::AVPacket *pkt,
                  NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), packet(pkt) {
}

FFmpeg::AVFormat::AVReadFrameWorker::~AVReadFrameWorker() {
}

void FFmpeg::AVFormat::AVReadFrameWorker::Execute() {
  result = av_read_frame(context, packet);
}

void FFmpeg::AVFormat::AVReadFrameWorker::HandleOKCallback() {
  NanScope();

  queue->pop_front();
  if (queue->size() > 0)
      NanAsyncQueueWorker(queue->front());

  Handle<Value> pkt = AVCodec::AVPacketWrapper::newInstance(packet);

  const int argc = 2;
  Handle<Value> argv[argc] = { NanNew<Number>(result), pkt };
  callback->Call(argc, argv);
}


Persistent<FunctionTemplate>
FFmpeg::AVFormat::AVFormatContextWrapper::constructor;

FFmpeg::AVFormat::AVFormatContextWrapper::
AVFormatContextWrapper(::AVFormatContext *ctx)
  : _this(ctx), _allocated(false) {
  if (!_this) {
    _this = avformat_alloc_context();
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVFormatContextWrapper::~AVFormatContextWrapper() {
  if (_this && _allocated)
    avformat_free_context(_this);
}

void
FFmpeg::AVFormat::AVFormatContextWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVFormatContext"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "openInput", OpenInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "closeInput", CloseInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findStreamInfo", FindStreamInfo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findProgramFromStream",
                            FindProgramFromStream);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findBestStream", FindBestStream);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readFrame", ReadFrame);
  NODE_SET_PROTOTYPE_METHOD(ctor, "seekFrame", SeekFrame);
  NODE_SET_PROTOTYPE_METHOD(ctor, "seekFile", SeekFile);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readPlay", ReadPlay);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readPause", ReadPause);
  NODE_SET_PROTOTYPE_METHOD(ctor, "writeHeader", WriteHeader);
  NODE_SET_PROTOTYPE_METHOD(ctor, "writeFrame", WriteFrame);
  NODE_SET_PROTOTYPE_METHOD(ctor, "writeTrailer", WriteTrailer);
  NODE_SET_PROTOTYPE_METHOD(ctor, "dumpFormat", DumpFormat);
  NODE_SET_PROTOTYPE_METHOD(ctor, "guessSampleAspectRatio",
                            GuessSampleAspectRatio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "guessFrameRate", GuessFrameRate);
  proto->SetAccessor(NanNew<String>("iformat"), GetIFormat);
  proto->SetAccessor(NanNew<String>("oformat"), GetOFormat);
  proto->SetAccessor(NanNew<String>("streams"), GetStreams);
  proto->SetAccessor(NanNew<String>("filename"), GetFilename);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("flags"), GetFlags, SetFlags);
  proto->SetAccessor(NanNew<String>("programs"), GetPrograms);
  proto->SetAccessor(NanNew<String>("video_codec_id"), GetVideoCodecId);
  proto->SetAccessor(NanNew<String>("audio_codec_id"), GetAudioCodecId);
  proto->SetAccessor(NanNew<String>("subtitle_codec_id"), GetSubtitleCodecId);
  proto->SetAccessor(NanNew<String>("chapters"), GetChapters);
  proto->SetAccessor(NanNew<String>("metadata"), GetMetadata);
  proto->SetAccessor(NanNew<String>("video_codec"),
                     GetVideoCodec, SetVideoCodec);
  proto->SetAccessor(NanNew<String>("audio_codec"),
                     GetAudioCodec, SetAudioCodec);
  proto->SetAccessor(NanNew<String>("subtitle_codec"),
                     GetSubtitleCodec, SetSubtitleCodec);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVFormatContext"), creator);
}

Handle<Value>
FFmpeg::AVFormat::AVFormatContextWrapper::newInstance(::AVFormatContext *ctx) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(ctx) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVFormatContextWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVFormatContext *ctx = nullptr;
    if (args[0]->IsExternal())
      ctx = static_cast<::AVFormatContext *>(External::Unwrap(args[0]));
    AVFormatContextWrapper *obj = new AVFormatContextWrapper(ctx);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::OpenInput) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("filename required");

  String::Utf8Value filename(args[0]);
  ::AVInputFormat *iformat = nullptr;
  ::AVDictionary *options = nullptr;
  int argc = 1;

  if (!args[1]->IsUndefined() && args[1]->IsObject()) {
    if (AVInputFormatWrapper::HasInstance(args[1]->ToObject())) {
      iformat =
        ObjectWrap::Unwrap<AVInputFormatWrapper>(args[1]->ToObject())->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> opts = args[argc]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString()) {
        String::Utf8Value key_str(key);
        String::Utf8Value val_str(val);
        av_dict_set(&options, *key_str, *val_str, 0);
      }
    }
    argc++;
  }

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("invalid arguments");
  }

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  int ret = avformat_open_input(&obj->_this, *filename, iformat, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  // FIXME hack, ffplay maybe should not use url_feof() to test for the end
  if (obj->_this && obj->_this->pb)
      obj->_this->pb->eof_reached = 0;

  if (!obj->_this)
    obj->_this = avformat_alloc_context();

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::CloseInput) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  avformat_close_input(&obj->_this);
  if (!obj->_this)
    obj->_this = avformat_alloc_context();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::FindStreamInfo) {
  NanScope();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  if (!obj->_this->nb_streams)
    NanReturnNull();
  ::AVDictionary **opts =
    (::AVDictionary **)av_mallocz(obj->_this->nb_streams *
                                  sizeof(::AVDictionary *));
  if (!opts)
    return NanThrowError("dictionary of streams memory alloaction error");

  int ret = avformat_find_stream_info(obj->_this, opts);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++)
    av_dict_free(&opts[i]);
  av_freep(&opts);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::FindProgramFromStream) {
  NanScope();

  ::AVProgram *last = nullptr;
  if (args[0]->IsObject()) {
    if (!AVProgramWrapper::HasInstance(args[0]->ToObject()))
      return NanThrowTypeError("program required");
    last = ObjectWrap::Unwrap<AVProgramWrapper>(args[0]->ToObject())->This();
  }
  if (!args[1]->IsNumber())
    return NanThrowTypeError("stream_index required");
  int stream_index = args[1]->Int32Value();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  ::AVProgram *program =
    av_find_program_from_stream(obj->_this, last, stream_index);
  if (!program)
    NanReturnNull();
  Handle<Value> ret = AVProgramWrapper::newInstance(program);
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::FindBestStream) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() ||
      !args[2]->IsNumber() || !args[3]->IsNumber())
    return NanThrowTypeError("media type, wanted_stream_nb, "
                             "related_stream, flags required");

  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[0]->Uint32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  int flags = args[3]->Int32Value();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  ::AVCodec *decoder_ret = nullptr;
  int ret = av_find_best_stream(obj->_this,
                                type, wanted_stream_nb, related_stream,
                                &decoder_ret, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadFrame) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVCodec::AVPacketWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  ::AVPacket *pkt =
    ObjectWrap::Unwrap<AVCodec::AVPacketWrapper>(args[0]->ToObject())->This();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  if (args[1]->IsFunction()) {
    NanCallback *callback = new NanCallback(Local<Function>::Cast(args[1]));
    AVReadFrameWorker *worker =
      new AVReadFrameWorker(&obj->_async_queue, obj->_this, pkt, callback);
    obj->_async_queue.push_back(worker);
    if (obj->_async_queue.size() == 1)
      NanAsyncQueueWorker(obj->_async_queue.front());
    NanReturnUndefined();
  }

  int ret = av_read_frame(obj->_this, pkt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::SeekFrame) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber())
    return NanThrowTypeError("stream_index, timestamp, flags required");

  int stream_index = args[0]->Int32Value();
  int64_t timestamp = args[1]->NumberValue();
  int flags = args[2]->Int32Value();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_seek_frame(obj->_this, stream_index, timestamp, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::SeekFile) {
  NanScope();

  if (!args[0]->IsNumber() || !args[4]->IsNumber())
    return NanThrowTypeError("stream_index, flags required");
  if (!args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    return NanThrowTypeError("min_ts, ts, max_ts required");

  int stream_index = args[0]->Int32Value();
  int64_t min_ts = args[1]->NumberValue();
  int64_t ts = args[2]->NumberValue();
  int64_t max_ts = args[3]->NumberValue();
  int flags = args[4]->Int32Value();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = avformat_seek_file(obj->_this,
                               stream_index, min_ts, ts, max_ts, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadPlay) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_play(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadPause) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_pause(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::WriteHeader) {
  NanScope();

  ::AVDictionary *options = nullptr;

  if (!args[0]->IsUndefined() && args[0]->IsObject()) {
    Local<Object> opts = args[0]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString()) {
        String::Utf8Value key_str(key);
        String::Utf8Value val_str(val);
        av_dict_set(&options, *key_str, *val_str, 0);
      }
    }
  }

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = avformat_write_header(obj->_this, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::WriteFrame) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVCodec::AVPacketWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  ::AVPacket *pkt =
    ObjectWrap::Unwrap<AVCodec::AVPacketWrapper>(args[0]->ToObject())->This();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_write_frame(obj->_this, pkt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::WriteTrailer) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_write_trailer(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::DumpFormat) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsString() || !args[2]->IsNumber())
    return NanThrowTypeError("index, url, is_output required");

  int index = args[0]->Int32Value();
  String::Utf8Value url(args[1]);
  int is_output = args[2]->Int32Value();

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if ((!is_output && obj->_this->iformat) || (is_output && obj->_this->oformat))
    av_dump_format(obj->_this, index, *url, is_output);
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::GuessSampleAspectRatio) {
  NanScope();

  ::AVStream *stream = nullptr;
  ::AVFrame *frame = nullptr;
  int argc = 0;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg0 = args[argc]->ToObject();
    if (AVStreamWrapper::HasInstance(arg0)) {
      stream = ObjectWrap::Unwrap<AVStreamWrapper>(arg0)->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg1 = args[argc]->ToObject();
    if (AVUtil::AVFrameWrapper::HasInstance(arg1)) {
      frame = ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(arg1)->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVRational sample_aspect_ratio =
    av_guess_sample_aspect_ratio(obj->_this, stream, frame);
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(sample_aspect_ratio.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(sample_aspect_ratio.den));
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::GuessFrameRate) {
  NanScope();

  ::AVStream *stream = nullptr;
  ::AVFrame *frame = nullptr;
  int argc = 0;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg0 = args[argc]->ToObject();
    if (AVStreamWrapper::HasInstance(arg0)) {
      stream = ObjectWrap::Unwrap<AVStreamWrapper>(arg0)->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg1 = args[argc]->ToObject();
    if (AVUtil::AVFrameWrapper::HasInstance(arg1)) {
      frame = ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(arg1)->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVRational frame_rate = av_guess_frame_rate(obj->_this, stream, frame);
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(frame_rate.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(frame_rate.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetIFormat) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->iformat)
    NanReturnNull();
  Handle<Value> ret = AVInputFormatWrapper::newInstance(obj->_this->iformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetOFormat) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->oformat)
    NanReturnNull();
  Handle<Value> ret = AVOutputFormatWrapper::newInstance(obj->_this->oformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetStreams) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> streams = NanNew<Array>(obj->_this->nb_streams);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++) {
    if (obj->_this->streams[i]) {
      Handle<Value> v = AVStreamWrapper::newInstance(obj->_this->streams[i]);
      streams->Set(i, v);
    }
  }
  NanReturnValue(streams);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetFilename) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  const char *filename = obj->_this->filename;
  NanReturnValue(NanNew<String>(filename));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetStartTime) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetDuration) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetFlags) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int flags = obj->_this->flags;
  NanReturnValue(NanNew<Integer>(flags));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetPrograms) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> programs = NanNew<Array>(obj->_this->nb_programs);
  for (unsigned int i = 0; i < obj->_this->nb_programs; i++) {
    if (obj->_this->programs[i]) {
      Handle<Value> v = AVProgramWrapper::newInstance(obj->_this->programs[i]);
      programs->Set(i, v);
    }
  }
  NanReturnValue(programs);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetVideoCodecId) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID video_codec_id = obj->_this->video_codec_id;
  NanReturnValue(NanNew<Number>(video_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetAudioCodecId) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID audio_codec_id = obj->_this->audio_codec_id;
  NanReturnValue(NanNew<Number>(audio_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetSubtitleCodecId) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID subtitle_codec_id = obj->_this->subtitle_codec_id;
  NanReturnValue(NanNew<Number>(subtitle_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetChapters) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> chapters = NanNew<Array>(obj->_this->nb_chapters);
  for (unsigned int i = 0; i < obj->_this->nb_chapters; i++) {
    if (obj->_this->chapters[i]) {
      Handle<Value> v = AVChapterWrapper::newInstance(obj->_this->chapters[i]);
      chapters->Set(i, v);
    }
  }
  NanReturnValue(chapters);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetMetadata) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Object> ret = NanNew<Object>();
  AVDictionary *metadata = obj->_this->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetVideoCodec) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_video_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetAudioCodec) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_audio_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetSubtitleCodec) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_subtitle_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetFlags) {
  NanScope();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  obj->_this->flags = value->Int32Value();
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetVideoCodec) {
  NanScope();
  if (!value->IsObject() ||
      !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec =
    ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_video_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetAudioCodec) {
  NanScope();
  if (!value->IsObject() ||
      !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec =
    ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_audio_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetSubtitleCodec) {
  NanScope();
  if (!value->IsObject() ||
      !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec =
    ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj =
    ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_subtitle_codec(obj->_this, codec);
}
