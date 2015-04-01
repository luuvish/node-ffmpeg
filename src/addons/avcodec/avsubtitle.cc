#include "avcodec/avsubtitle.h"
#include "avcodec/avsubtitlerect.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVSubtitle::constructor;

void AVSubtitle::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVSubtitle"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "free", Free);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("format"), GetFormat, SetFormat);
  inst->SetAccessor(NanNew("start_display_time"),
                    GetStartDisplayTime, SetStartDisplayTime);
  inst->SetAccessor(NanNew("end_display_time"),
                    GetEndDisplayTime, SetEndDisplayTime);
  inst->SetAccessor(NanNew("rects"), GetRects);
  inst->SetAccessor(NanNew("pts"), GetPts, SetPts);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVSubtitle"), tpl->GetFunction());
}

Local<Object> AVSubtitle::NewInstance(::AVSubtitle* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVSubtitle>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVSubtitle::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVSubtitle* AVSubtitle::This(::AVSubtitle* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

AVSubtitle::AVSubtitle() : this_(nullptr) {
  this_ = (::AVSubtitle*)av_mallocz(sizeof(::AVSubtitle));
  if (this_ == nullptr)
    NanThrowTypeError("AVSubtitle: cannot allocation");
}

AVSubtitle::~AVSubtitle() {
  if (this_ != nullptr) {
    if (this_->num_rects > 0)
      avsubtitle_free(this_);
    av_freep(&this_);
  }
}

NAN_METHOD(AVSubtitle::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVSubtitle* obj = new AVSubtitle();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVSubtitle::Free) {
  NanScope();

  AVSubtitle* obj = Unwrap<AVSubtitle>(args.This());

  if (obj->this_) {
    if (obj->this_->num_rects > 0)
      avsubtitle_free(obj->this_);
    obj->this_ = nullptr;
  }
  NanReturnUndefined();
}

NAN_GETTER(AVSubtitle::GetFormat) {
  NanScope();

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint16_t format = wrap->format;
  NanReturnValue(NanNew<Uint32>(format));
}

NAN_SETTER(AVSubtitle::SetFormat) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("format: integer required");

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();

  if (wrap)
    wrap->format = static_cast<uint16_t>(value->Uint32Value());
}

NAN_GETTER(AVSubtitle::GetStartDisplayTime) {
  NanScope();

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint32_t start_display_time = wrap->start_display_time;
  NanReturnValue(NanNew<Uint32>(start_display_time));
}

NAN_SETTER(AVSubtitle::SetStartDisplayTime) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("start_display_time: integer required");

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();

  if (wrap)
    wrap->start_display_time = value->Uint32Value();
}

NAN_GETTER(AVSubtitle::GetEndDisplayTime) {
  NanScope();

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint32_t end_display_time = wrap->end_display_time;
  NanReturnValue(NanNew<Uint32>(end_display_time));
}

NAN_SETTER(AVSubtitle::SetEndDisplayTime) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("end_display_time: integer required");

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();

  if (wrap)
    wrap->end_display_time = value->Uint32Value();
}

NAN_GETTER(AVSubtitle::GetRects) {
  NanScope();

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> rets = NanNew<Array>(wrap->num_rects);
  for (uint32_t i = 0; i < wrap->num_rects; i++) {
    if (wrap->rects[i]) {
      Local<Object> v = AVSubtitleRect::NewInstance(wrap->rects[i]);
      rets->Set(i, v);
    }
  }
  NanReturnValue(rets);
}

NAN_GETTER(AVSubtitle::GetPts) {
  NanScope();

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts = wrap->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_SETTER(AVSubtitle::SetPts) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pts: integer required");

  ::AVSubtitle* wrap = Unwrap<AVSubtitle>(args.This())->This();

  if (wrap)
    wrap->pts = value->IntegerValue();
}

}  // namespace avcodec
}  // namespace ffmpeg
