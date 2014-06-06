#include "avformat.h"
#include "avcodec.h"

using namespace node;
using namespace v8;


Persistent<FunctionTemplate> FFmpeg::AVOutputFormatWrapper::constructor;

FFmpeg::AVOutputFormatWrapper::AVOutputFormatWrapper() : _this(nullptr) {}

FFmpeg::AVOutputFormatWrapper::~AVOutputFormatWrapper() {}

void FFmpeg::AVOutputFormatWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVOutputFormat"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("name"), GetName);
  proto->SetAccessor(NanNew<String>("long_name"), GetLongName);
  proto->SetAccessor(NanNew<String>("mime_type"), GetMimeType);
  proto->SetAccessor(NanNew<String>("extensions"), GetExtensions);
  proto->SetAccessor(NanNew<String>("audio_codec"), GetAudioCodec);
  proto->SetAccessor(NanNew<String>("video_codec"), GetVideoCodec);
  proto->SetAccessor(NanNew<String>("subtitle_codec"), GetSubtitleCodec);

  target->Set(NanNew<String>("AVOutputFormat"), ctor->GetFunction());
  NODE_SET_METHOD(ctor->GetFunction(), "guessFormat", GuessFormat);
  NODE_SET_METHOD(ctor->GetFunction(), "guessCodec", GuessCodec);
  NODE_SET_METHOD(ctor->GetFunction(), "queryCodec", QueryCodec);
}

Handle<Value> FFmpeg::AVOutputFormatWrapper::newInstance(AVOutputFormat *oformat)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(ret);
  obj->_this = oformat;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVOutputFormatWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVOutputFormatWrapper *obj = new AVOutputFormatWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVOutputFormatWrapper::GuessFormat) {
  NanScope();

  if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString())
    return NanThrowTypeError("short_name, filename, mime_type required");

  const char *short_name = *String::Utf8Value(args[0]);
  const char *filename = *String::Utf8Value(args[1]);
  const char *mime_type = *String::Utf8Value(args[2]);

  AVOutputFormat *oformat = av_guess_format(short_name, filename, mime_type);

  if (oformat)
    NanReturnValue(newInstance(oformat));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVOutputFormatWrapper::GuessCodec) {
  NanScope();

  if (!args[0]->IsObject() || !NanHasInstance(AVOutputFormatWrapper::constructor, args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsString() || !args[2]->IsString() || !args[3]->IsString())
    return NanThrowTypeError("short_name, filename, mime_type required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("media type required");

  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject());
  AVOutputFormat *fmt = obj->_this;
  const char *short_name = *String::Utf8Value(args[1]);
  const char *filename = *String::Utf8Value(args[2]);
  const char *mime_type = *String::Utf8Value(args[3]);
  enum AVMediaType type = static_cast<enum AVMediaType>(args[4]->Uint32Value());

  enum AVCodecID codec_id = av_guess_codec(fmt, short_name, filename, mime_type, type);

  NanReturnValue(NanNew<Number>(codec_id));
}

NAN_METHOD(FFmpeg::AVOutputFormatWrapper::QueryCodec) {
  NanScope();

  if (!args[0]->IsObject() || !NanHasInstance(AVOutputFormatWrapper::constructor, args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsNumber() || !args[2]->IsNumber())
    return NanThrowTypeError("codec_id, std_compliance required");

  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject());
  AVOutputFormat *ofmt = obj->_this;
  enum AVCodecID codec_id = static_cast<enum AVCodecID>(args[1]->Uint32Value());
  int std_compliance = args[2]->Int32Value();

  int ret = avformat_query_codec(ofmt, codec_id, std_compliance);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetName) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetLongName) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetMimeType) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *mime_type = obj->_this->mime_type;
  NanReturnValue(NanNew<String>(mime_type));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetExtensions) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *extensions = obj->_this->extensions;
  NanReturnValue(NanNew<String>(extensions));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetAudioCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum AVCodecID audio_codec = obj->_this->audio_codec;
  NanReturnValue(NanNew<Number>(audio_codec));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetVideoCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum AVCodecID video_codec = obj->_this->video_codec;
  NanReturnValue(NanNew<Number>(video_codec));
}

NAN_GETTER(FFmpeg::AVOutputFormatWrapper::GetSubtitleCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum AVCodecID subtitle_codec = obj->_this->subtitle_codec;
  NanReturnValue(NanNew<Number>(subtitle_codec));
}


Persistent<FunctionTemplate> FFmpeg::AVInputFormatWrapper::constructor;

FFmpeg::AVInputFormatWrapper::AVInputFormatWrapper() : _this(nullptr) {}

FFmpeg::AVInputFormatWrapper::~AVInputFormatWrapper() {}

void FFmpeg::AVInputFormatWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVInputFormat"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("name"), GetName);
  proto->SetAccessor(NanNew<String>("long_name"), GetLongName);

  target->Set(NanNew<String>("AVInputFormat"), ctor->GetFunction());
  NODE_SET_METHOD(ctor->GetFunction(), "findInputFormat", FindInputFormat);
}

Handle<Value> FFmpeg::AVInputFormatWrapper::newInstance(AVInputFormat *iformat)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(ret);
  obj->_this = iformat;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVInputFormatWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVInputFormatWrapper *obj = new AVInputFormatWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVInputFormatWrapper::FindInputFormat) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("input format name required");

  const char *short_name = *String::Utf8Value(args[0]);
  AVInputFormat *iformat = av_find_input_format(short_name);

  if (iformat)
    NanReturnValue(newInstance(iformat));
  else
    NanReturnNull();
}

NAN_GETTER(FFmpeg::AVInputFormatWrapper::GetName) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVInputFormatWrapper::GetLongName) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}


Persistent<FunctionTemplate> FFmpeg::AVStreamWrapper::constructor;

FFmpeg::AVStreamWrapper::AVStreamWrapper() : _this(nullptr) {}

FFmpeg::AVStreamWrapper::~AVStreamWrapper() {}

void FFmpeg::AVStreamWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FFmpeg::AVStreamWrapper::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVStream"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("index"), GetIndex);
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("codec"), GetCodec);
  proto->SetAccessor(NanNew<String>("pts"), GetPts);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard);
  proto->SetAccessor(NanNew<String>("sample_aspect_ratio"), GetSampleAspectRatio);

  target->Set(NanNew<String>("AVStream"), ctor->GetFunction());
}

Handle<Value> FFmpeg::AVStreamWrapper::newInstance(AVStream *stream)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(ret);
  obj->_this = stream;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVStreamWrapper::New) {
  NanScope();
  AVStreamWrapper *obj = new AVStreamWrapper;
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetIndex) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int index = obj->_this->index;
  NanReturnValue(NanNew<Number>(index));
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetId) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetCodec) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  Handle<Value> ret = AVCodecContextWrapper::newInstance(obj->_this->codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetPts) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  AVFrac pts = obj->_this->pts;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("val"), NanNew<Number>(pts.val));
  ret->Set(NanNew<String>("num"), NanNew<Number>(pts.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(pts.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetTimeBase) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetStartTime) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetDuration) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int64_t duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetDiscard) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  enum AVDiscard discard = obj->_this->discard;
  NanReturnValue(NanNew<Number>(discard));
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetSampleAspectRatio) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  AVRational sample_aspect_ratio = obj->_this->sample_aspect_ratio;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(sample_aspect_ratio.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(sample_aspect_ratio.den));
  NanReturnValue(ret);
}


Persistent<FunctionTemplate> FFmpeg::AVProgramWrapper::constructor;

FFmpeg::AVProgramWrapper::AVProgramWrapper() : _this(nullptr) {}

FFmpeg::AVProgramWrapper::~AVProgramWrapper() {}

void FFmpeg::AVProgramWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FFmpeg::AVProgramWrapper::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVProgram"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("codec"), GetCodec, SetCodec);
}

Handle<Value> FFmpeg::AVProgramWrapper::newInstance(AVProgram *program)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(ret);
  obj->_this = program;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVProgramWrapper::New) {
  NanScope();
  AVProgramWrapper *obj = new FFmpeg::AVProgramWrapper();
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetCodec) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  NanReturnValue(NanNew<String>(""));
}

NAN_SETTER(FFmpeg::AVProgramWrapper::SetCodec) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
}


Persistent<FunctionTemplate> FFmpeg::AVChapterWrapper::constructor;

FFmpeg::AVChapterWrapper::AVChapterWrapper() : _this(nullptr) {}

FFmpeg::AVChapterWrapper::~AVChapterWrapper() {}

void FFmpeg::AVChapterWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FFmpeg::AVChapterWrapper::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVChapter"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("codec"), GetCodec, SetCodec);
}

Handle<Value> FFmpeg::AVChapterWrapper::newInstance(AVChapter *chapter)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(ret);
  obj->_this = chapter;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVChapterWrapper::New) {
  NanScope();
  AVChapterWrapper *obj = new FFmpeg::AVChapterWrapper();
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVChapterWrapper::GetCodec) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  NanReturnValue(NanNew<String>(""));
}

NAN_SETTER(FFmpeg::AVChapterWrapper::SetCodec) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
}


Persistent<FunctionTemplate> FFmpeg::AVFormatContextWrapper::constructor;

FFmpeg::AVFormatContextWrapper::AVFormatContextWrapper() {
  _this = avformat_alloc_context();
}

FFmpeg::AVFormatContextWrapper::~AVFormatContextWrapper() {
  if (_this)
    avformat_free_context(_this);
}

void FFmpeg::AVFormatContextWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVFormatContext"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "openInput", OpenInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "closeInput", CloseInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findStreamInfo", FindStreamInfo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findBestStream", FindBestStream);
  NODE_SET_PROTOTYPE_METHOD(ctor, "seekFile", SeekFile);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readPlay", ReadPlay);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readPause", ReadPause);
  NODE_SET_PROTOTYPE_METHOD(ctor, "readFrame", ReadFrame);
  proto->SetAccessor(NanNew<String>("streams"), GetStreams);
  proto->SetAccessor(NanNew<String>("programs"), GetPrograms);
  proto->SetAccessor(NanNew<String>("chapters"), GetChapters);
  proto->SetAccessor(NanNew<String>("filename"), GetFilename);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);

  target->Set(NanNew<String>("AVFormatContext"), ctor->GetFunction());
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::New) {
  if (!args.IsConstructCall()) {
    NanScope();
    NanReturnValue(constructor->GetFunction()->NewInstance());
  }
  NanScope();
  AVFormatContextWrapper *obj = new AVFormatContextWrapper();
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::OpenInput) {
  NanScope();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  String::Utf8Value filename(args[0]);
  AVInputFormat* iformat = NULL;//args[1]->Int32Value();
  AVDictionary* opts = NULL;
  AVDictionary** options = &opts;//args[2]->Int32value();
  int ret = avformat_open_input(&obj->_this, *filename, iformat, options);

  if (ret < 0)
    NanReturnValue(NanNew<Number>(ret));

  bool genpts = 0;
  int seek_by_bytes = -1;
  const char* window_title = NULL;
  int64_t start_time = AV_NOPTS_VALUE;
  int infinite_buffer = -1;
  bool show_status = 1;

  if (genpts)
    obj->_this->flags |= AVFMT_FLAG_GENPTS;

  if (obj->_this->pb)
    obj->_this->pb->eof_reached = 0;

  if (seek_by_bytes)
    seek_by_bytes = !!(obj->_this->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", obj->_this->iformat->name);

  double max_frame_duration = (obj->_this->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

  AVDictionaryEntry* t;
  if (!window_title && (t = av_dict_get(obj->_this->metadata, "title", NULL, 0)))
    window_title = t->value;

  if (start_time != AV_NOPTS_VALUE) {
    int64_t timestamp = start_time;
    if (obj->_this->start_time != AV_NOPTS_VALUE)
      timestamp += obj->_this->start_time;
    int ret = avformat_seek_file(obj->_this, -1, INT64_MIN, timestamp, INT64_MAX, 0);
  }

  bool realtime = 0;
  if (!strcmp(obj->_this->iformat->name, "rtp") ||
      !strcmp(obj->_this->iformat->name, "rtsp") ||
      !strcmp(obj->_this->iformat->name, "sdp"))
    realtime = 1;
  else if (obj->_this->pb &&
     (!strncmp(obj->_this->filename, "rtp:", 4) ||
      !strncmp(obj->_this->filename, "udp:", 4)))
    realtime = 1;

  if (infinite_buffer < 0 && realtime)
    infinite_buffer = 1;

  if (show_status)
    av_dump_format(obj->_this, 0, *filename, 0);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::CloseInput) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  avformat_close_input(&obj->_this);
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::FindStreamInfo) {
  NanScope();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  if (!obj->_this->nb_streams)
    NanReturnUndefined();
  AVDictionary **opts = (AVDictionary **)av_mallocz(obj->_this->nb_streams * sizeof(AVDictionary *));
  if (!opts)
    NanReturnUndefined();

  int err = avformat_find_stream_info(obj->_this, opts);

  for (unsigned int i = 0; i < obj->_this->nb_streams; i++)
    av_dict_free(&opts[i]);
  av_freep(&opts);

  NanReturnValue(NanNew<Number>(err));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::FindBestStream) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum AVMediaType type = static_cast<enum AVMediaType>(args[0]->Uint32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  AVCodec *decoder_ret = NULL;
  int flags = args[4]->Int32Value();
  int ret = av_find_best_stream(obj->_this, type, wanted_stream_nb, related_stream, &decoder_ret, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::SeekFile) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int stream_index = args[0]->Int32Value();
  int64_t min_ts = args[1]->NumberValue();
  int64_t ts = args[2]->NumberValue();
  int64_t max_ts = args[3]->NumberValue();
  int flags = args[4]->Int32Value();
  int ret = avformat_seek_file(obj->_this, stream_index, min_ts, ts, max_ts, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::ReadPlay) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_play(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::ReadPause) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_pause(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::ReadFrame) {
  NanScope();
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetStreams) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> streams = NanNew<Array>(obj->_this->nb_streams);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++) {
    Handle<Value> v = AVStreamWrapper::newInstance(obj->_this->streams[i]);
    streams->Set(i, v);
  }
  NanReturnValue(streams);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetPrograms) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> streams = NanNew<Array>(obj->_this->nb_streams);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++) {
    Handle<Value> v = AVStreamWrapper::newInstance(obj->_this->streams[i]);
    streams->Set(i, v);
  }
  NanReturnValue(streams);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetChapters) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> streams = NanNew<Array>(obj->_this->nb_streams);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++) {
    Handle<Value> v = AVStreamWrapper::newInstance(obj->_this->streams[i]);
    streams->Set(i, v);
  }
  NanReturnValue(streams);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetFilename) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  const char *filename = obj->_this->filename;
  NanReturnValue(NanNew<String>(filename));
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetStartTime) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetDuration) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}
