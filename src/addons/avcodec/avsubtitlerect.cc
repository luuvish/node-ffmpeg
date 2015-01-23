#include "avutil/avutil.h"
#include "avcodec/avpicture.h"
#include "avcodec/avsubtitlerect.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVSubtitleRect::constructor;

AVSubtitleRect::AVSubtitleRect(::AVSubtitleRect *ref)
  : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVSubtitleRect *)av_mallocz(sizeof(::AVSubtitleRect));
    alloc_ = true;
  }
}

AVSubtitleRect::~AVSubtitleRect() {
  if (this_ != nullptr && alloc_ == true) {
    av_freep(&this_);
  }
}

void AVSubtitleRect::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVSubtitleRect"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("x"), GetX);
  tpl->InstanceTemplate()->SetAccessor(NanNew("y"), GetY);
  tpl->InstanceTemplate()->SetAccessor(NanNew("w"), GetW);
  tpl->InstanceTemplate()->SetAccessor(NanNew("h"), GetH);
  tpl->InstanceTemplate()->SetAccessor(NanNew("nb_colors"), GetNbColors);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pict"), GetPict);
  tpl->InstanceTemplate()->SetAccessor(NanNew("type"), GetType);
  tpl->InstanceTemplate()->SetAccessor(NanNew("text"), GetText);
  tpl->InstanceTemplate()->SetAccessor(NanNew("ass"), GetAss);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVSubtitleRect"), tpl->GetFunction());
}

NAN_METHOD(AVSubtitleRect::New) {
  NanScope();

  if (args.IsConstructCall()) {
    ::AVSubtitleRect *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVSubtitleRect *>(External::Unwrap(args[0]));
    AVSubtitleRect *obj = new AVSubtitleRect(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

Local<Object> AVSubtitleRect::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVSubtitleRect::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetX) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<Number>(ref->x));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetY) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<Number>(ref->y));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetW) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<Number>(ref->w));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetH) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<Number>(ref->h));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetNbColors) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<Number>(ref->nb_colors));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetPict) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();
  Local<Value> arg[3];
  arg[0] = NanNew<External>(&ref->pict);
  arg[1] = NanNew<Number>(ref->w);
  arg[2] = NanNew<Number>(ref->h);
  Local<Object> ret = AVPicture::NewInstance(arg);

  NanReturnValue(ret);
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetType) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();
  enum ::AVSubtitleType type = ref->type;

  NanReturnValue(NanNew<Number>(type));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetText) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<String>(ref->text ? ref->text : ""));
}

NAN_PROPERTY_GETTER(AVSubtitleRect::GetAss) {
  NanScope();

  AVSubtitleRect *obj = ObjectWrap::Unwrap<AVSubtitleRect>(args.This());
  ::AVSubtitleRect *ref = obj->This();

  NanReturnValue(NanNew<String>(ref->ass ? ref->ass : ""));
}

}  // namespace avcodec
}  // namespace ffmpeg
