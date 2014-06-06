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

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVOutputFormat"), creator);
  NODE_SET_METHOD(creator, "guessFormat", GuessFormat);
  NODE_SET_METHOD(creator, "guessCodec", GuessCodec);
  NODE_SET_METHOD(creator, "queryCodec", QueryCodec);
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

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVInputFormat"), creator);
  NODE_SET_METHOD(creator, "findInputFormat", FindInputFormat);
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

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVStream"), creator);
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
  if (args.IsConstructCall()) {
    NanScope();
    AVStreamWrapper *obj = new AVStreamWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
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
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard);
  proto->SetAccessor(NanNew<String>("stream_index"), GetStreamIndex);
  proto->SetAccessor(NanNew<String>("program_num"), GetProgramNum);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("end_time"), GetEndTime);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVProgram"), creator);
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
  if (args.IsConstructCall()) {
    NanScope();
    AVProgramWrapper *obj = new AVProgramWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetId) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetDiscard) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  enum AVDiscard discard = obj->_this->discard;
  NanReturnValue(NanNew<Number>(discard));
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetStreamIndex) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  Handle<Array> stream_indexes = NanNew<Array>(obj->_this->nb_stream_indexes);
  for (unsigned int i = 0; i < obj->_this->nb_stream_indexes; i++)
    stream_indexes->Set(i, NanNew<Number>(obj->_this->stream_index[i]));
  NanReturnValue(stream_indexes);
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetProgramNum) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int program_num = obj->_this->program_num;
  NanReturnValue(NanNew<Number>(program_num));
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetStartTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVProgramWrapper::GetEndTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t end_time = obj->_this->end_time;
  NanReturnValue(NanNew<Number>(end_time));
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
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("start"), GetStart);
  proto->SetAccessor(NanNew<String>("end"), GetEnd);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVChapter"), creator);
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
  if (args.IsConstructCall()) {
    NanScope();
    AVChapterWrapper *obj = new AVChapterWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVChapterWrapper::GetId) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVChapterWrapper::GetTimeBase) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVChapterWrapper::GetStart) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t start = obj->_this->start;
  NanReturnValue(NanNew<Number>(start));
}

NAN_GETTER(FFmpeg::AVChapterWrapper::GetEnd) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t end = obj->_this->end;
  NanReturnValue(NanNew<Number>(end));
}


Persistent<FunctionTemplate> FFmpeg::AVFormatContextWrapper::constructor;

FFmpeg::AVFormatContextWrapper::AVFormatContextWrapper() : _this(nullptr) {
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

NAN_METHOD(FFmpeg::AVFormatContextWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVFormatContextWrapper *obj = new AVFormatContextWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::OpenInput) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("filename required");

  const char *filename = *String::Utf8Value(args[0]);
  AVInputFormat* iformat = nullptr;
  AVDictionary* options = nullptr;
  int argc = 1;

  if (!args[1]->IsUndefined() && args[1]->IsObject()) {
    Local<Object> arg1 = args[1]->ToObject();
    if (NanHasInstance(AVInputFormatWrapper::constructor, arg1)) {
      iformat = ObjectWrap::Unwrap<AVInputFormatWrapper>(arg1)->_this;
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> opts = args[argc]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString())
        av_dict_set(&options, *String::Utf8Value(key), *String::Utf8Value(val), 0);
    }
    argc++;
  }

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("invalid arguments");
  }

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  int ret = avformat_open_input(&obj->_this, filename, iformat, &options);

  AVDictionaryEntry *t = nullptr;
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

NAN_METHOD(FFmpeg::AVFormatContextWrapper::CloseInput) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  avformat_close_input(&obj->_this);
  if (!obj->_this)
    obj->_this = avformat_alloc_context();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::FindStreamInfo) {
  NanScope();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  if (!obj->_this->nb_streams)
    NanReturnNull();
  AVDictionary **opts = (AVDictionary **)av_mallocz(obj->_this->nb_streams * sizeof(AVDictionary *));
  if (!opts)
    return NanThrowError("dictionary of streams memory alloaction error");

  int ret = avformat_find_stream_info(obj->_this, opts);
  for (unsigned int i = 0; i < obj->_this->nb_streams; i++)
    av_dict_free(&opts[i]);
  av_freep(&opts);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::FindProgramFromStream) {
  NanScope();

  AVProgram *last = nullptr;
  if (args[0]->IsObject()) {
    if (!NanHasInstance(AVProgramWrapper::constructor, args[0]->ToObject()))
      return NanThrowTypeError("media type, wanted_stream_nb, related_stream, flags required");
    AVProgramWrapper *arg0 = ObjectWrap::Unwrap<AVProgramWrapper>(args[0]->ToObject());
    last = arg0->_this;
  }
  if (!args[1]->IsNumber())
    return NanThrowTypeError("stream_index required");
  int stream_index = args[1]->Int32Value();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  AVProgram *program = av_find_program_from_stream(obj->_this, last, stream_index);
  if (!program)
    NanReturnNull();
  Handle<Value> ret = AVProgramWrapper::newInstance(program);
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::FindBestStream) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
    return NanThrowTypeError("media type, wanted_stream_nb, related_stream, flags required");

  enum AVMediaType type = static_cast<enum AVMediaType>(args[0]->Uint32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  int flags = args[3]->Int32Value();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());

  AVCodec *decoder_ret = nullptr;
  int ret = av_find_best_stream(obj->_this, type, wanted_stream_nb, related_stream, &decoder_ret, flags);
  if (!decoder_ret)
    NanReturnValue(NanNew<Number>(ret));

  Handle<Value> codec = AVCodecWrapper::newInstance(decoder_ret);
  NanReturnValue(codec);
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::ReadFrame) {
  NanScope();

  if (!args[0]->IsObject() || !NanHasInstance(AVPacketWrapper::constructor, args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  AVPacketWrapper *arg0 = ObjectWrap::Unwrap<AVPacketWrapper>(args[0]->ToObject());
  AVPacket *pkt = arg0->_this;

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_read_frame(obj->_this, pkt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::SeekFrame) {
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

NAN_METHOD(FFmpeg::AVFormatContextWrapper::SeekFile) {
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

NAN_METHOD(FFmpeg::AVFormatContextWrapper::WriteHeader) {
  NanScope();

  AVDictionary *options = nullptr;

  if (!args[0]->IsUndefined() && args[0]->IsObject()) {
    Local<Object> opts = args[0]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString())
        av_dict_set(&options, *String::Utf8Value(key), *String::Utf8Value(val), 0);
    }
  }

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = avformat_write_header(obj->_this, &options);

  AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::WriteFrame) {
  NanScope();

  if (!args[0]->IsObject() || !NanHasInstance(AVPacketWrapper::constructor, args[0]->ToObject()))
    return NanThrowTypeError("packet required");

  AVPacketWrapper *arg0 = ObjectWrap::Unwrap<AVPacketWrapper>(args[0]->ToObject());
  AVPacket *pkt = arg0->_this;

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_write_frame(obj->_this, pkt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::WriteTrailer) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  int ret = av_write_trailer(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVFormatContextWrapper::DumpFormat) {
  NanScope();

  if (!args[0]->IsNumber() || !args[1]->IsString() || !args[2]->IsNumber())
    return NanThrowTypeError("index, url, is_output required");

  int index = args[0]->Int32Value();
  const char *url = *String::Utf8Value(args[1]);
  int is_output = args[2]->Int32Value();

  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if ((!is_output && obj->_this->iformat) || (is_output && obj->_this->oformat))
    av_dump_format(obj->_this, index, url, is_output);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetIFormat) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->iformat)
    NanReturnNull();
  Handle<Value> ret = AVInputFormatWrapper::newInstance(obj->_this->iformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetOFormat) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  if (!obj->_this->oformat)
    NanReturnNull();
  Handle<Value> ret = AVOutputFormatWrapper::newInstance(obj->_this->oformat);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetStreams) {
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

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetPrograms) {
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

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetVideoCodecId) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum AVCodecID video_codec_id = obj->_this->video_codec_id;
  NanReturnValue(NanNew<Number>(video_codec_id));
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetAudioCodecId) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum AVCodecID audio_codec_id = obj->_this->audio_codec_id;
  NanReturnValue(NanNew<Number>(audio_codec_id));
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetSubtitleCodecId) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  enum AVCodecID subtitle_codec_id = obj->_this->subtitle_codec_id;
  NanReturnValue(NanNew<Number>(subtitle_codec_id));
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetChapters) {
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

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetVideoCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  AVCodec *codec = av_format_get_video_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetAudioCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  AVCodec *codec = av_format_get_audio_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormatContextWrapper::GetSubtitleCodec) {
  NanScope();
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  AVCodec *codec = av_format_get_subtitle_codec(obj->_this);
  if (!codec)
    NanReturnNull();
  Handle<Value> ret = AVCodecWrapper::newInstance(codec);
  NanReturnValue(ret);
}

NAN_SETTER(FFmpeg::AVFormatContextWrapper::SetVideoCodec) {
  NanScope();
  if (!value->IsObject() || !NanHasInstance(AVCodecWrapper::constructor, value->ToObject()))
    NanThrowTypeError("codec required");
  AVCodecWrapper *arg0 = ObjectWrap::Unwrap<AVCodecWrapper>(value->ToObject());
  AVCodec *codec = arg0->_this;
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_video_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormatContextWrapper::SetAudioCodec) {
  NanScope();
  if (!value->IsObject() || !NanHasInstance(AVCodecWrapper::constructor, value->ToObject()))
    NanThrowTypeError("codec required");
  AVCodecWrapper *arg0 = ObjectWrap::Unwrap<AVCodecWrapper>(value->ToObject());
  AVCodec *codec = arg0->_this;
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_audio_codec(obj->_this, codec);
}

NAN_SETTER(FFmpeg::AVFormatContextWrapper::SetSubtitleCodec) {
  NanScope();
  if (!value->IsObject() || !NanHasInstance(AVCodecWrapper::constructor, value->ToObject()))
    NanThrowTypeError("codec required");
  AVCodecWrapper *arg0 = ObjectWrap::Unwrap<AVCodecWrapper>(value->ToObject());
  AVCodec *codec = arg0->_this;
  AVFormatContextWrapper *obj = ObjectWrap::Unwrap<AVFormatContextWrapper>(args.This());
  av_format_set_subtitle_codec(obj->_this, codec);
}
