#include "avformat/avinputformat.h"
#include "avutil/avutil.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVInputFormat::constructor;

AVInputFormat::AVInputFormat(::AVInputFormat *ref) : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVInputFormat *)av_mallocz(sizeof(::AVInputFormat));
    alloc_ = true;
  }
}

AVInputFormat::~AVInputFormat() {
  if (this_ != nullptr && alloc_ == true) {
    av_freep(&this_);
  }
}

void AVInputFormat::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVInputFormat"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("name"), GetName);
  tpl->InstanceTemplate()->SetAccessor(NanNew("long_name"), GetLongName);
  tpl->InstanceTemplate()->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  tpl->InstanceTemplate()->SetAccessor(NanNew("extensions"), GetExtensions);
  tpl->InstanceTemplate()->SetAccessor(NanNew("mime_type"), GetMimeType);
  tpl->InstanceTemplate()->SetAccessor(NanNew("raw_codec_id"), GetRawCodecId);

  NODE_SET_METHOD(tpl->GetFunction(), "findInputFormat", FindInputFormat);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVInputFormat"), tpl->GetFunction());
}

Local<Value> AVInputFormat::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVInputFormat::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVInputFormat::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVInputFormat *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVInputFormat *>(External::Unwrap(args[0]));
    AVInputFormat *obj = new AVInputFormat(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVInputFormat::FindInputFormat) {
  NanEscapableScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("findInputFormat: short_name string required");

  String::Utf8Value short_name(args[0]);
  ::AVInputFormat *ref = av_find_input_format(*short_name);

  if (ref)
    NanReturnValue(NewInstance(NanNew<External>(ref)));
  else
    NanReturnNull();
}

NAN_GETTER(AVInputFormat::GetName) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  const char *name = ref->name;

  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetLongName) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  const char *long_name = ref->long_name;

  if (long_name)
    NanReturnValue(NanNew<String>(long_name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetFlags) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  int flags = ref->flags;

  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(AVInputFormat::SetFlags) {
  NanScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  ref->flags = value->Int32Value();
}

NAN_GETTER(AVInputFormat::GetExtensions) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  const char *extensions = ref->extensions;

  if (extensions)
    NanReturnValue(NanNew<String>(extensions));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetMimeType) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  const char *mime_type = ref->mime_type;

  if (mime_type)
    NanReturnValue(NanNew<String>(mime_type));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVInputFormat::GetRawCodecId) {
  NanEscapableScope();

  ::AVInputFormat *ref = Unwrap<AVInputFormat>(args.This())->This();
  int raw_codec_id = ref->raw_codec_id;

  NanReturnValue(NanNew<Integer>(raw_codec_id));
}

}  // namespace avformat
}  // namespace ffmpeg
