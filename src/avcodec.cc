#include "avcodec.h"

using namespace node;
using namespace v8;


Persistent<FunctionTemplate> FFmpeg::AVCodecContextWrapper::constructor;

FFmpeg::AVCodecContextWrapper::AVCodecContextWrapper() : _this(nullptr) {}

FFmpeg::AVCodecContextWrapper::~AVCodecContextWrapper() {}

void FFmpeg::AVCodecContextWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVCodecContext"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("width"), GetWidth);
  proto->SetAccessor(NanNew<String>("height"), GetHeight);
  proto->SetAccessor(NanNew<String>("coded_width"), GetCodedWidth);
  proto->SetAccessor(NanNew<String>("coded_height"), GetCodedHeight);
  proto->SetAccessor(NanNew<String>("pix_fmt"), GetPixFmt);

  target->Set(NanNew<String>("AVCodecContext"), ctor->GetFunction());

  avcodec_register_all();
}

Handle<Value> FFmpeg::AVCodecContextWrapper::newInstance(AVCodecContext *ctx)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(ret);
  obj->_this = ctx;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::New) {
  if (!args.IsConstructCall()) {
    NanScope();
    NanReturnValue(constructor->GetFunction()->NewInstance());
  }
  NanScope();
  AVCodecContextWrapper *obj = new AVCodecContextWrapper();
  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetTimeBase) {
  NanScope();
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetWidth) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int width = obj->_this->width;
  NanReturnValue(NanNew<Number>(width));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetHeight) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int height = obj->_this->height;
  NanReturnValue(NanNew<Number>(height));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodedWidth) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int coded_width = obj->_this->coded_width;
  NanReturnValue(NanNew<Number>(coded_width));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodedHeight) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int coded_height = obj->_this->coded_height;
  NanReturnValue(NanNew<Number>(coded_height));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetPixFmt) {
  NanScope();
  NanReturnUndefined();
}
