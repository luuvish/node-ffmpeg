#include "avformat.h"
#include "avcodec.h"
#include "avutil.h"

using namespace node;
using namespace v8;


void FFmpeg::AVFormat::Initialize(Handle<Object> target) {
  NanScope();

  AVOutputFormatWrapper::Initialize(target);
  AVInputFormatWrapper::Initialize(target);
  AVStreamWrapper::Initialize(target);
  AVProgramWrapper::Initialize(target);
  AVChapterWrapper::Initialize(target);
  AVFormatContextWrapper::Initialize(target);
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVOutputFormatWrapper::constructor;

FFmpeg::AVFormat::AVOutputFormatWrapper::AVOutputFormatWrapper(::AVOutputFormat *oformat) : _this(oformat), _allocated(false) {
  if (!_this) {
    _this = (::AVOutputFormat *)av_mallocz(sizeof(::AVOutputFormat));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVOutputFormatWrapper::~AVOutputFormatWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVOutputFormatWrapper::Initialize(Handle<Object> target) {
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

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVOutputFormat"), creator);
  NODE_SET_METHOD(creator, "guessFormat", GuessFormat);
  NODE_SET_METHOD(creator, "guessCodec", GuessCodec);
  NODE_SET_METHOD(creator, "queryCodec", QueryCodec);
}

Handle<Value> FFmpeg::AVFormat::AVOutputFormatWrapper::newInstance(::AVOutputFormat *oformat) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(oformat) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVOutputFormatWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVOutputFormat *oformat = nullptr;
    if (args[0]->IsExternal())
      oformat = static_cast<::AVOutputFormat *>(External::Unwrap(args[0]));
    AVOutputFormatWrapper *obj = new AVOutputFormatWrapper(oformat);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::GuessFormat) {
  NanScope();

  if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString())
    return NanThrowTypeError("short_name, filename, mime_type required");

  String::Utf8Value short_name(args[0]);
  String::Utf8Value filename(args[1]);
  String::Utf8Value mime_type(args[2]);

  ::AVOutputFormat *oformat = av_guess_format(*short_name, *filename, *mime_type);

  if (oformat)
    NanReturnValue(newInstance(oformat));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::GuessCodec) {
  NanScope();

  if (!args[0]->IsObject() || !AVOutputFormatWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsString() || !args[2]->IsString() || !args[3]->IsString())
    return NanThrowTypeError("short_name, filename, mime_type required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("media type required");

  ::AVOutputFormat *fmt = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject())->This();
  String::Utf8Value short_name(args[1]);
  String::Utf8Value filename(args[2]);
  String::Utf8Value mime_type(args[3]);
  enum ::AVMediaType type = static_cast<enum ::AVMediaType>(args[4]->Uint32Value());

  enum ::AVCodecID codec_id = av_guess_codec(fmt, *short_name, *filename, *mime_type, type);

  NanReturnValue(NanNew<Number>(codec_id));
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::QueryCodec) {
  NanScope();

  if (!args[0]->IsObject() || !AVOutputFormatWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsNumber() || !args[2]->IsNumber())
    return NanThrowTypeError("codec_id, std_compliance required");

  ::AVOutputFormat *ofmt = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject())->This();
  enum ::AVCodecID codec_id = static_cast<enum ::AVCodecID>(args[1]->Uint32Value());
  int std_compliance = args[2]->Int32Value();

  int ret = avformat_query_codec(ofmt, codec_id, std_compliance);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetName) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetLongName) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetMimeType) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *mime_type = obj->_this->mime_type;
  NanReturnValue(NanNew<String>(mime_type ? mime_type : ""));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetExtensions) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *extensions = obj->_this->extensions;
  NanReturnValue(NanNew<String>(extensions));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetAudioCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID audio_codec = obj->_this->audio_codec;
  NanReturnValue(NanNew<Number>(audio_codec));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetVideoCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID video_codec = obj->_this->video_codec;
  NanReturnValue(NanNew<Number>(video_codec));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetSubtitleCodec) {
  NanScope();
  AVOutputFormatWrapper *obj = ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID subtitle_codec = obj->_this->subtitle_codec;
  NanReturnValue(NanNew<Number>(subtitle_codec));
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVInputFormatWrapper::constructor;

FFmpeg::AVFormat::AVInputFormatWrapper::AVInputFormatWrapper(::AVInputFormat *iformat) : _this(iformat), _allocated(false) {
  if (!_this) {
    _this = (::AVInputFormat *)av_mallocz(sizeof(::AVInputFormat));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVInputFormatWrapper::~AVInputFormatWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVInputFormatWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVInputFormat"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("name"), GetName);
  proto->SetAccessor(NanNew<String>("long_name"), GetLongName);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVInputFormat"), creator);
  NODE_SET_METHOD(creator, "findInputFormat", FindInputFormat);
}

Handle<Value> FFmpeg::AVFormat::AVInputFormatWrapper::newInstance(::AVInputFormat *iformat) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(iformat) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVInputFormatWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVInputFormatWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVInputFormat *iformat = nullptr;
    if (args[0]->IsExternal())
      iformat = static_cast<::AVInputFormat *>(External::Unwrap(args[0]));
    AVInputFormatWrapper *obj = new AVInputFormatWrapper(iformat);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVFormat::AVInputFormatWrapper::FindInputFormat) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("input format name required");

  String::Utf8Value short_name(args[0]);
  ::AVInputFormat *iformat = av_find_input_format(*short_name);

  if (iformat)
    NanReturnValue(newInstance(iformat));
  else
    NanReturnNull();
}

NAN_GETTER(FFmpeg::AVFormat::AVInputFormatWrapper::GetName) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVFormat::AVInputFormatWrapper::GetLongName) {
  NanScope();
  AVInputFormatWrapper *obj = ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVStreamWrapper::constructor;

FFmpeg::AVFormat::AVStreamWrapper::AVStreamWrapper(::AVStream *stream) : _this(stream), _allocated(false) {
  if (!_this) {
    _this = (::AVStream *)av_mallocz(sizeof(::AVStream));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVStreamWrapper::~AVStreamWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVStreamWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
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
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard, SetDiscard);
  proto->SetAccessor(NanNew<String>("sample_aspect_ratio"), GetSampleAspectRatio);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVStream"), creator);
}

Handle<Value> FFmpeg::AVFormat::AVStreamWrapper::newInstance(::AVStream *stream) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(stream) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVStreamWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVStreamWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVStream *stream = nullptr;
    if (args[0]->IsExternal())
      stream = static_cast<::AVStream *>(External::Unwrap(args[0]));
    AVStreamWrapper *obj = new AVStreamWrapper(stream);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetIndex) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int index = obj->_this->index;
  NanReturnValue(NanNew<Number>(index));
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetId) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetCodec) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  Handle<Value> ret = AVCodec::AVCodecContextWrapper::newInstance(obj->_this->codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetPts) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  ::AVFrac pts = obj->_this->pts;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("val"), NanNew<Number>(pts.val));
  ret->Set(NanNew<String>("num"), NanNew<Number>(pts.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(pts.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetTimeBase) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  ::AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetStartTime) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetDuration) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  int64_t duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetDiscard) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  enum ::AVDiscard discard = obj->_this->discard;
  NanReturnValue(NanNew<Number>(discard));
}

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetSampleAspectRatio) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  ::AVRational sample_aspect_ratio = obj->_this->sample_aspect_ratio;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(sample_aspect_ratio.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(sample_aspect_ratio.den));
  NanReturnValue(ret);
}

NAN_SETTER(FFmpeg::AVFormat::AVStreamWrapper::SetDiscard) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("discard required");
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  obj->_this->discard = static_cast<enum AVDiscard>(value->NumberValue());
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVProgramWrapper::constructor;

FFmpeg::AVFormat::AVProgramWrapper::AVProgramWrapper(::AVProgram *program) : _this(program), _allocated(false) {
  if (!_this) {
    _this = (::AVProgram *)av_mallocz(sizeof(::AVProgram));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVProgramWrapper::~AVProgramWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVProgramWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVProgram"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard);
  proto->SetAccessor(NanNew<String>("stream_indexes"), GetStreamIndexes);
  proto->SetAccessor(NanNew<String>("program_num"), GetProgramNum);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("end_time"), GetEndTime);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVProgram"), creator);
}

Handle<Value> FFmpeg::AVFormat::AVProgramWrapper::newInstance(::AVProgram *program) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(program) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVProgramWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVProgramWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVProgram *program = nullptr;
    if (args[0]->IsExternal())
      program = static_cast<::AVProgram *>(External::Unwrap(args[0]));
    AVProgramWrapper *obj = new AVProgramWrapper(program);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetId) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetDiscard) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  enum ::AVDiscard discard = obj->_this->discard;
  NanReturnValue(NanNew<Number>(discard));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetStreamIndexes) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  Handle<Array> stream_indexes = NanNew<Array>(obj->_this->nb_stream_indexes);
  for (unsigned int i = 0; i < obj->_this->nb_stream_indexes; i++)
    stream_indexes->Set(i, NanNew<Number>(obj->_this->stream_index[i]));
  NanReturnValue(stream_indexes);
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetProgramNum) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int program_num = obj->_this->program_num;
  NanReturnValue(NanNew<Number>(program_num));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetStartTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetEndTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t end_time = obj->_this->end_time;
  NanReturnValue(NanNew<Number>(end_time));
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVChapterWrapper::constructor;

FFmpeg::AVFormat::AVChapterWrapper::AVChapterWrapper(::AVChapter *chapter) : _this(chapter), _allocated(false) {
  if (!_this) {
    _this = (::AVChapter *)av_mallocz(sizeof(::AVChapter));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVChapterWrapper::~AVChapterWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVChapterWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVChapter"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("start"), GetStart);
  proto->SetAccessor(NanNew<String>("end"), GetEnd);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVChapter"), creator);
}

Handle<Value> FFmpeg::AVFormat::AVChapterWrapper::newInstance(::AVChapter *chapter) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(chapter) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVChapterWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVChapterWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVChapter *chapter = nullptr;
    if (args[0]->IsExternal())
      chapter = static_cast<::AVChapter *>(External::Unwrap(args[0]));
    AVChapterWrapper *obj = new AVChapterWrapper(chapter);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetId) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetTimeBase) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  ::AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetStart) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t start = obj->_this->start;
  NanReturnValue(NanNew<Number>(start));
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetEnd) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t end = obj->_this->end;
  NanReturnValue(NanNew<Number>(end));
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVFormatContextWrapper::constructor;

FFmpeg::AVFormat::AVFormatContextWrapper::AVFormatContextWrapper(::AVFormatContext *ctx) : _this(ctx), _allocated(false) {
  if (!_this) {
    _this = avformat_alloc_context();
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVFormatContextWrapper::~AVFormatContextWrapper() {
  if (_this && _allocated)
    avformat_free_context(_this);
}

void FFmpeg::AVFormat::AVFormatContextWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVFormatContext"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "openInput", OpenInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "closeInput", CloseInput);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findStreamInfo", FindStreamInfo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "findProgramFromStream", FindProgramFromStream);
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
  NODE_SET_PROTOTYPE_METHOD(ctor, "guessSampleAspectRatio", GuessSampleAspectRatio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "guessFrameRate", GuessFrameRate);
  proto->SetAccessor(NanNew<String>("iformat"), GetIFormat);
  proto->SetAccessor(NanNew<String>("oformat"), GetOFormat);
  proto->SetAccessor(NanNew<String>("streams"), GetStreams);
  proto->SetAccessor(NanNew<String>("filename"), GetFilename);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("programs"), GetPrograms);
  proto->SetAccessor(NanNew<String>("video_codec_id"), GetVideoCodecId);
  proto->SetAccessor(NanNew<String>("audio_codec_id"), GetAudioCodecId);
  proto->SetAccessor(NanNew<String>("subtitle_codec_id"), GetSubtitleCodecId);
  proto->SetAccessor(NanNew<String>("chapters"), GetChapters);
  proto->SetAccessor(NanNew<String>("video_codec"), GetVideoCodec, SetVideoCodec);
  proto->SetAccessor(NanNew<String>("audio_codec"), GetAudioCodec, SetAudioCodec);
  proto->SetAccessor(NanNew<String>("subtitle_codec"), GetSubtitleCodec, SetSubtitleCodec);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVFormatContext"), creator);
}

Handle<Value> FFmpeg::AVFormat::AVFormatContextWrapper::newInstance(::AVFormatContext *ctx) {
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
      iformat = ObjectWrap::Unwrap<AVInputFormatWrapper>(args[1]->ToObject())->This();
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  int ret = avformat_open_input(&obj->_this, *filename, iformat, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  if (obj->_this && obj->_this->pb)
      obj->_this->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use url_feof() to test for the end

  if (!obj->_this)
    obj->_this = avformat_alloc_context();

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::CloseInput) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  avformat_close_input(&obj->_this);
  if (!obj->_this)
    obj->_this = avformat_alloc_context();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::FindStreamInfo) {
  NanScope();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  if (!obj->_this->nb_streams)
    NanReturnNull();
  ::AVDictionary **opts = (::AVDictionary **)av_mallocz(obj->_this->nb_streams * sizeof(::AVDictionary *));
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  ::AVProgram *program = av_find_program_from_stream(obj->_this, last, stream_index);
  if (!program)
    NanReturnNull();
  Handle<Value> ret = AVProgramWrapper::newInstance(program);
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::FindBestStream) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    return NanThrowTypeError("media type, wanted_stream_nb, related_stream, flags required");

  enum ::AVMediaType type = static_cast<enum ::AVMediaType>(args[0]->Uint32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  int flags = args[3]->Int32Value();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  ::AVCodec *decoder_ret = nullptr;
  int ret = av_find_best_stream(obj->_this, type, wanted_stream_nb, related_stream, &decoder_ret, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadFrame) {
  NanScope();

  if (!args[0]->IsObject() || !AVCodec::AVPacketWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  ::AVPacket *pkt = ObjectWrap::Unwrap<AVCodec::AVPacketWrapper>(args[0]->ToObject())->This();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = avformat_seek_file(obj->_this, stream_index, min_ts, ts, max_ts, flags);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadPlay) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_play(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::ReadPause) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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

  if (!args[0]->IsObject() || !AVCodec::AVPacketWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  ::AVPacket *pkt = ObjectWrap::Unwrap<AVCodec::AVPacketWrapper>(args[0]->ToObject())->This();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_write_frame(obj->_this, pkt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormat::AVFormatContextWrapper::WriteTrailer) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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
    if (AVStreamWrapper::HasInstance(args[argc]->ToObject())) {
      stream = ObjectWrap::Unwrap<AVStreamWrapper>(args[argc]->ToObject())->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    if (AVUtil::AVFrameWrapper::HasInstance(args[argc]->ToObject())) {
      frame = ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(args[argc]->ToObject())->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVRational sample_aspect_ratio = av_guess_sample_aspect_ratio(obj->_this, stream, frame);
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
    if (AVStreamWrapper::HasInstance(args[argc]->ToObject())) {
      stream = ObjectWrap::Unwrap<AVStreamWrapper>(args[argc]->ToObject())->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    if (AVUtil::AVFrameWrapper::HasInstance(args[argc]->ToObject())) {
      frame = ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(args[argc]->ToObject())->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVRational frame_rate = av_guess_frame_rate(obj->_this, stream, frame);
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(frame_rate.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(frame_rate.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetIFormat) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->iformat)
    NanReturnNull();
  Handle<Value> ret = AVInputFormatWrapper::newInstance(obj->_this->iformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetOFormat) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->oformat)
    NanReturnNull();
  Handle<Value> ret = AVOutputFormatWrapper::newInstance(obj->_this->oformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetStreams) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  const char *filename = obj->_this->filename;
  NanReturnValue(NanNew<String>(filename));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetStartTime) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetDuration) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int64_t duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetPrograms) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
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
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID video_codec_id = obj->_this->video_codec_id;
  NanReturnValue(NanNew<Number>(video_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetAudioCodecId) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID audio_codec_id = obj->_this->audio_codec_id;
  NanReturnValue(NanNew<Number>(audio_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetSubtitleCodecId) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum ::AVCodecID subtitle_codec_id = obj->_this->subtitle_codec_id;
  NanReturnValue(NanNew<Number>(subtitle_codec_id));
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetChapters) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  Handle<Array> chapters = NanNew<Array>(obj->_this->nb_chapters);
  for (unsigned int i = 0; i < obj->_this->nb_chapters; i++) {
    if (obj->_this->chapters[i]) {
      Handle<Value> v = AVChapterWrapper::newInstance(obj->_this->chapters[i]);
      chapters->Set(i, v);
    }
  }
  NanReturnValue(chapters);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetVideoCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_video_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetAudioCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_audio_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVFormatContextWrapper::GetSubtitleCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  ::AVCodec *codec = av_format_get_subtitle_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodec::AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetVideoCodec) {
  NanScope();
  if (!value->IsObject() || !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec = ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_video_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetAudioCodec) {
  NanScope();
  if (!value->IsObject() || !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec = ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_audio_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormat::AVFormatContextWrapper::SetSubtitleCodec) {
  NanScope();
  if (!value->IsObject() || !AVCodec::AVCodecWrapper::HasInstance(value->ToObject()))
    NanThrowTypeError("codec required");
  ::AVCodec *codec = ObjectWrap::Unwrap<AVCodec::AVCodecWrapper>(value->ToObject())->This();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_subtitle_codec(obj->_this, codec);
}
