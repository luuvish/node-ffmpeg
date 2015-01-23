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


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVStreamWrapper::constructor;

FFmpeg::AVFormat::AVStreamWrapper::AVStreamWrapper(::AVStream *stream)
  : _this(stream), _allocated(false) {
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
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard, SetDiscard);
  proto->SetAccessor(NanNew<String>("sample_aspect_ratio"),
                     GetSampleAspectRatio);
  proto->SetAccessor(NanNew<String>("metadata"), GetMetadata);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVStream"), creator);
}

Handle<Value>
FFmpeg::AVFormat::AVStreamWrapper::newInstance(::AVStream *stream) {
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
  Handle<Value> ret =
    AVCodec::AVCodecContextWrapper::newInstance(obj->_this->codec);
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

NAN_GETTER(FFmpeg::AVFormat::AVStreamWrapper::GetMetadata) {
  NanScope();
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  Handle<Object> ret = NanNew<Object>();
  AVDictionary *metadata = obj->_this->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_SETTER(FFmpeg::AVFormat::AVStreamWrapper::SetDiscard) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("discard required");
  AVStreamWrapper *obj = ObjectWrap::Unwrap<AVStreamWrapper>(args.This());
  obj->_this->discard = static_cast<enum AVDiscard>(value->NumberValue());
}
