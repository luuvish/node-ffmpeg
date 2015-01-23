#include "avutil/avutil.h"
#include "avcodec/avsubtitle.h"
#include "avcodec/avsubtitlerect.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVSubtitle::constructor;

AVSubtitle::AVSubtitle(::AVSubtitle *ref) : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVSubtitle *)av_mallocz(sizeof(::AVSubtitle));
    alloc_ = true;
  }
}

AVSubtitle::~AVSubtitle() {
  if (this_ != nullptr && alloc_ == true) {
    avsubtitle_free(this_);
    av_freep(&this_);
  }
}

void AVSubtitle::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVSubtitle"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("format"), GetFormat);
  tpl->InstanceTemplate()->SetAccessor(NanNew("start_display_time"),
                                       GetStartDisplayTime);
  tpl->InstanceTemplate()->SetAccessor(NanNew("end_display_time"),
                                       GetEndDisplayTime);
  tpl->InstanceTemplate()->SetAccessor(NanNew("rects"), GetRects);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pts"), GetPts);

  NODE_SET_PROTOTYPE_METHOD(tpl, "free", Free);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVSubtitle"), tpl->GetFunction());
}

NAN_METHOD(AVSubtitle::New) {
  NanScope();

  if (args.IsConstructCall()) {
    ::AVSubtitle *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVSubtitle *>(External::Unwrap(args[0]));
    AVSubtitle *obj = new AVSubtitle(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVSubtitle::Free) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();
  avsubtitle_free(ref);
  obj->this_ = nullptr;
  obj->alloc_ = false;

  NanReturnUndefined();
}

Local<Object> AVSubtitle::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVSubtitle::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_PROPERTY_GETTER(AVSubtitle::GetFormat) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();
  uint16_t format = ref->format;

  NanReturnValue(NanNew<Number>(format));
}

NAN_PROPERTY_GETTER(AVSubtitle::GetStartDisplayTime) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();
  uint32_t start_display_time = ref->start_display_time;

  NanReturnValue(NanNew<Number>(start_display_time));
}

NAN_PROPERTY_GETTER(AVSubtitle::GetEndDisplayTime) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();
  uint32_t end_display_time = ref->end_display_time;

  NanReturnValue(NanNew<Number>(end_display_time));
}

NAN_PROPERTY_GETTER(AVSubtitle::GetRects) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();

  Local<Array> ret = NanNew<Array>(ref->num_rects);
  for (unsigned int i = 0; i < ref->num_rects; i++) {
    if (ref->rects[i]) {
      Local<External> e = NanNew<External>(ref->rects[i]);
      Local<Object> v = AVSubtitleRect::NewInstance(e);
      ret->Set(i, v);
    }
  }

  NanReturnValue(ret);
}

NAN_PROPERTY_GETTER(AVSubtitle::GetPts) {
  NanScope();

  AVSubtitle *obj = ObjectWrap::Unwrap<AVSubtitle>(args.This());
  ::AVSubtitle *ref = obj->This();
  int64_t pts = ref->pts;

  NanReturnValue(NanNew<Number>(pts));
}

}  // namespace avcodec
}  // namespace ffmpeg
