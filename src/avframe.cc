#include "avframe.h"

using namespace node;
using namespace v8;


Persistent<FunctionTemplate> FFmpeg::AVFrameWrapper::constructor;

FFmpeg::AVFrameWrapper::AVFrameWrapper(AVFrame *frame) : _this(frame) {
  if (!_this)
    _this = av_frame_alloc();
}

FFmpeg::AVFrameWrapper::~AVFrameWrapper() {
  if (_this)
    av_frame_free(&_this);
}

void FFmpeg::AVFrameWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVFrame"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("width"), GetWidth);
  proto->SetAccessor(NanNew<String>("height"), GetHeight);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVFrame"), creator);
}

Handle<Value> FFmpeg::AVFrameWrapper::newInstance(AVFrame *frame)
{
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(frame) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFrameWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFrameWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVFrame *frame = nullptr;
    if (args[0]->IsExternal())
      frame = static_cast<AVFrame *>(External::Unwrap(args[0]));
    AVFrameWrapper *obj = new AVFrameWrapper(frame);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFrameWrapper::GetWidth) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int width = obj->_this->width;
  NanReturnValue(NanNew<Number>(width));
}

NAN_GETTER(FFmpeg::AVFrameWrapper::GetHeight) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int height = obj->_this->height;
  NanReturnValue(NanNew<Number>(height));
}
