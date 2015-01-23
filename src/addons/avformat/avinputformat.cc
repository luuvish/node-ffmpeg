#include "avformat/avinputformat.h"
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
FFmpeg::AVFormat::AVInputFormatWrapper::constructor;

FFmpeg::AVFormat::AVInputFormatWrapper::
AVInputFormatWrapper(::AVInputFormat *iformat)
  : _this(iformat), _allocated(false) {
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
  proto->SetAccessor(NanNew<String>("flags"), GetFlags, SetFlags);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVInputFormat"), creator);
  NODE_SET_METHOD(creator, "findInputFormat", FindInputFormat);
}

Handle<Value>
FFmpeg::AVFormat::AVInputFormatWrapper::newInstance(::AVInputFormat *iformat) {
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
  AVInputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVFormat::AVInputFormatWrapper::GetLongName) {
  NanScope();
  AVInputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVFormat::AVInputFormatWrapper::GetFlags) {
  NanScope();
  AVInputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  int flags = obj->_this->flags;
  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(FFmpeg::AVFormat::AVInputFormatWrapper::SetFlags) {
  NanScope();
  AVInputFormatWrapper *obj =
    ObjectWrap::Unwrap<AVInputFormatWrapper>(args.This());
  obj->_this->flags = value->Int32Value();
}
