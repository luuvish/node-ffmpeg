#include "avcodec/avsubtitlerect.h"
#include "avcodec/avpicture.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVSubtitleRect::constructor;

void AVSubtitleRect::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVSubtitleRect"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("x"), GetX);
  inst->SetAccessor(NanNew("y"), GetY);
  inst->SetAccessor(NanNew("w"), GetW);
  inst->SetAccessor(NanNew("h"), GetH);
  inst->SetAccessor(NanNew("nb_colors"), GetNbColors);
  inst->SetAccessor(NanNew("pict"), GetPict);
  inst->SetAccessor(NanNew("type"), GetType);
  inst->SetAccessor(NanNew("text"), GetText);
  inst->SetAccessor(NanNew("ass"), GetAss);
  inst->SetAccessor(NanNew("flags"), GetFlags);

  NanAssignPersistent(constructor, tpl);
}

Local<Object> AVSubtitleRect::NewInstance(::AVSubtitleRect* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVSubtitleRect>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVSubtitleRect::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVSubtitleRect* AVSubtitleRect::This(::AVSubtitleRect* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVSubtitleRect::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVSubtitleRect* obj = new AVSubtitleRect();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(AVSubtitleRect::GetX) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int x = wrap->x;
  NanReturnValue(NanNew<Int32>(x));
}

NAN_GETTER(AVSubtitleRect::GetY) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int y = wrap->y;
  NanReturnValue(NanNew<Int32>(y));
}

NAN_GETTER(AVSubtitleRect::GetW) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int w = wrap->w;
  NanReturnValue(NanNew<Int32>(w));
}

NAN_GETTER(AVSubtitleRect::GetH) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int h = wrap->h;
  NanReturnValue(NanNew<Int32>(h));
}

NAN_GETTER(AVSubtitleRect::GetNbColors) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int nb_colors = wrap->nb_colors;
  NanReturnValue(NanNew<Int32>(nb_colors));
}

NAN_GETTER(AVSubtitleRect::GetPict) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = AVPicture::NewInstance(&wrap->pict);
  NanReturnValue(ret);
}

NAN_GETTER(AVSubtitleRect::GetType) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVSubtitleType type = wrap->type;
  NanReturnValue(NanNew<Uint32>(type));
}

NAN_GETTER(AVSubtitleRect::GetText) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* text = wrap->text;
  if (text)
    NanReturnValue(NanNew<String>(text));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVSubtitleRect::GetAss) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* ass = wrap->ass;
  if (ass)
    NanReturnValue(NanNew<String>(ass));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVSubtitleRect::GetFlags) {
  NanScope();

  ::AVSubtitleRect* wrap = Unwrap<AVSubtitleRect>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

}  // namespace avcodec
}  // namespace ffmpeg
