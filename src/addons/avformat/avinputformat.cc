#include "avformat/avinputformat.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVInputFormat::constructor;

void AVInputFormat::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVInputFormat"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("name"), GetName);
  inst->SetAccessor(NanNew("long_name"), GetLongName);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("extensions"), GetExtensions);
  inst->SetAccessor(NanNew("mime_type"), GetMimeType);
  inst->SetAccessor(NanNew("raw_codec_id"), GetRawCodecId);

  NODE_SET_METHOD(tpl->GetFunction(), "findInputFormat", FindInputFormat);

  NanAssignPersistent(constructor, tpl);

  NODE_SET_METHOD(exports, "iformats", GetIFormats);
  NODE_SET_METHOD(exports, "findInputFormat", FindInputFormat);
}

Local<Object> AVInputFormat::NewInstance(::AVInputFormat* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVInputFormat>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVInputFormat::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVInputFormat* AVInputFormat::This(::AVInputFormat* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVInputFormat::GetIFormats) {
  NanScope();

  ::AVInputFormat* fmt = nullptr;
  int size = 0;

  while ((fmt = av_iformat_next(fmt))) size++;

  Local<Array> ret = NanNew<Array>(size);
  fmt = nullptr;
  for (int i = 0; i < size; i++) {
    fmt = av_iformat_next(fmt);
    ret->Set(i, NewInstance(fmt));
  }

  if (size > 0)
    NanReturnValue(ret);
  else
    NanReturnNull();
}

NAN_METHOD(AVInputFormat::FindInputFormat) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("findInputFormat: short_name string required");

  NanUtf8String short_name(args[0]);
  ::AVInputFormat* wrap = av_find_input_format(*short_name);

  if (wrap)
    NanReturnValue(NewInstance(wrap));
  else
    NanReturnNull();
}

NAN_METHOD(AVInputFormat::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVInputFormat* obj = new AVInputFormat();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(AVInputFormat::GetName) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* name = wrap->name;
  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetLongName) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* long_name = wrap->long_name;
  if (long_name)
    NanReturnValue(NanNew<String>(long_name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetFlags) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVInputFormat::SetFlags) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVInputFormat::GetExtensions) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* extensions = wrap->extensions;
  if (extensions)
    NanReturnValue(NanNew<String>(extensions));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetMimeType) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* mime_type = wrap->mime_type;
  if (mime_type)
    NanReturnValue(NanNew<String>(mime_type));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetRawCodecId) {
  NanScope();

  ::AVInputFormat* wrap = Unwrap<AVInputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int raw_codec_id = wrap->raw_codec_id;
  NanReturnValue(NanNew<Int32>(raw_codec_id));
}

}  // namespace avformat
}  // namespace ffmpeg
