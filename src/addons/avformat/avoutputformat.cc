#include "avformat/avoutputformat.h"
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


Persistent<FunctionTemplate>
FFmpeg::AVFormat::AVOutputFormatWrapper::constructor;

FFmpeg::AVFormat::AVOutputFormatWrapper::
AVOutputFormatWrapper(::AVOutputFormat *oformat)
  : _this(oformat), _allocated(false) {
  if (!_this) {
    _this = (::AVOutputFormat *)av_mallocz(sizeof(::AVOutputFormat));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVOutputFormatWrapper::~AVOutputFormatWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void
FFmpeg::AVFormat::AVOutputFormatWrapper::Initialize(Handle<Object> target) {
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
  proto->SetAccessor(NanNew<String>("flags"), GetFlags, SetFlags);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVOutputFormat"), creator);
  NODE_SET_METHOD(creator, "guessFormat", GuessFormat);
  NODE_SET_METHOD(creator, "guessCodec", GuessCodec);
  NODE_SET_METHOD(creator, "queryCodec", QueryCodec);
}

Handle<Value>
FFmpeg::AVFormat::AVOutputFormatWrapper::
newInstance(::AVOutputFormat *oformat) {
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

  ::AVOutputFormat *oformat =
    av_guess_format(*short_name, *filename, *mime_type);

  if (oformat)
    NanReturnValue(newInstance(oformat));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::GuessCodec) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVOutputFormatWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsString() || !args[2]->IsString() || !args[3]->IsString())
    return NanThrowTypeError("short_name, filename, mime_type required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("media type required");

  ::AVOutputFormat *fmt =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject())->This();
  String::Utf8Value short_name(args[1]);
  String::Utf8Value filename(args[2]);
  String::Utf8Value mime_type(args[3]);
  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[4]->Uint32Value());

  enum ::AVCodecID codec_id =
    av_guess_codec(fmt, *short_name, *filename, *mime_type, type);

  NanReturnValue(NanNew<Number>(codec_id));
}

NAN_METHOD(FFmpeg::AVFormat::AVOutputFormatWrapper::QueryCodec) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVOutputFormatWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("output format required");
  if (!args[1]->IsNumber() || !args[2]->IsNumber())
    return NanThrowTypeError("codec_id, std_compliance required");

  ::AVOutputFormat *ofmt =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args[0]->ToObject())->This();
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[1]->Uint32Value());
  int std_compliance = args[2]->Int32Value();

  int ret = avformat_query_codec(ofmt, codec_id, std_compliance);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetName) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetLongName) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetMimeType) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *mime_type = obj->_this->mime_type;
  NanReturnValue(NanNew<String>(mime_type ? mime_type : ""));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetExtensions) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  const char *extensions = obj->_this->extensions;
  NanReturnValue(NanNew<String>(extensions));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetAudioCodec) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID audio_codec = obj->_this->audio_codec;
  NanReturnValue(NanNew<Number>(audio_codec));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetVideoCodec) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID video_codec = obj->_this->video_codec;
  NanReturnValue(NanNew<Number>(video_codec));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetSubtitleCodec) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  enum ::AVCodecID subtitle_codec = obj->_this->subtitle_codec;
  NanReturnValue(NanNew<Number>(subtitle_codec));
}

NAN_GETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::GetFlags) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  int flags = obj->_this->flags;
  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(FFmpeg::AVFormat::AVOutputFormatWrapper::SetFlags) {
  NanScope();
  AVOutputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVOutputFormatWrapper>(args.This());
  obj->_this->flags = value->Int32Value();
}
