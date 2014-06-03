#include "avformat.h"
#include "avcodec.h"

using namespace node;
using namespace v8;


Persistent<FunctionTemplate> FFmpeg::AVInputFormatWrapper::constructor;

void FFmpeg::AVInputFormatWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FFmpeg::AVInputFormatWrapper::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVInputFormat"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("source"), GetSource, SetSource);

  target->Set(NanNew<String>("AVInputFormat"), ctor->GetFunction());
}

NAN_METHOD(FFmpeg::AVInputFormatWrapper::New) {
  NanScope();
  AVInputFormatWrapper *obj = new FFmpeg::AVInputFormatWrapper;
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVInputFormatWrapper::GetSource) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  NanReturnValue(NanNew<String>(""));
}

NAN_SETTER(FFmpeg::AVInputFormatWrapper::SetSource) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
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
  proto->SetAccessor(NanNew<String>("codec"), GetCodec);
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
  AVStreamWrapper *obj = new FFmpeg::AVStreamWrapper();
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVStreamWrapper::GetCodec) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  Handle<Value> ret = AVCodecContextWrapper::newInstance(obj->_this->codec);
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

  av_register_all();
  avformat_network_init();
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
