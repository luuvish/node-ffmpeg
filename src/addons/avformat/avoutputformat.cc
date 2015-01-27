#include "avformat/avoutputformat.h"
#include "avutil/avutil.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVOutputFormat::constructor;

AVOutputFormat::AVOutputFormat(::AVOutputFormat *ref)
  : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVOutputFormat *)av_mallocz(sizeof(::AVOutputFormat));
    alloc_ = true;
  }
}

AVOutputFormat::~AVOutputFormat() {
  if (this_ != nullptr && alloc_ == true) {
    av_freep(&this_);
  }
}

void AVOutputFormat::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVOutputFormat"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("name"), GetName);
  tpl->InstanceTemplate()->SetAccessor(NanNew("long_name"), GetLongName);
  tpl->InstanceTemplate()->SetAccessor(NanNew("mime_type"), GetMimeType);
  tpl->InstanceTemplate()->SetAccessor(NanNew("extensions"), GetExtensions);
  tpl->InstanceTemplate()->SetAccessor(NanNew("audio_codec"), GetAudioCodec);
  tpl->InstanceTemplate()->SetAccessor(NanNew("video_codec"), GetVideoCodec);
  tpl->InstanceTemplate()->SetAccessor(NanNew("subtitle_codec"),
                                       GetSubtitleCodec);
  tpl->InstanceTemplate()->SetAccessor(NanNew("flags"), GetFlags, SetFlags);

  NODE_SET_METHOD(tpl->GetFunction(), "guessFormat", GuessFormat);
  NODE_SET_METHOD(tpl->GetFunction(), "guessCodec", GuessCodec);
  NODE_SET_METHOD(tpl->GetFunction(), "queryCodec", QueryCodec);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVOutputFormat"), tpl->GetFunction());
}

Local<Object> AVOutputFormat::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVOutputFormat::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVOutputFormat::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVOutputFormat *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVOutputFormat *>(External::Unwrap(args[0]));
    AVOutputFormat *obj = new AVOutputFormat(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVOutputFormat::GuessFormat) {
  NanEscapableScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("guessFormat: short_name string required");
  if (!args[1]->IsString())
    return NanThrowTypeError("guessFormat: filename string required");
  if (!args[2]->IsString())
    return NanThrowTypeError("guessFormat: mime_type string required");

  String::Utf8Value short_name(args[0]);
  String::Utf8Value filename(args[1]);
  String::Utf8Value mime_type(args[2]);

  ::AVOutputFormat *ref = av_guess_format(*short_name, *filename, *mime_type);

  if (ref)
    NanReturnValue(NewInstance(NanNew<External>(ref)));
  else
    NanReturnNull();
}

NAN_METHOD(AVOutputFormat::GuessCodec) {
  NanEscapableScope();

  if (!HasInstance(args[0]))
    return NanThrowTypeError("guessCodec: AVOutputFormat instance expected");
  if (!args[1]->IsString())
    return NanThrowTypeError("guessCodec: short_name string required");
  if (!args[2]->IsString())
    return NanThrowTypeError("guessCodec: filename string required");
  if (!args[3]->IsString())
    return NanThrowTypeError("guessCodec: mime_type string required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("guessCodec: media type enum required");

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args[0]->ToObject())->This();
  String::Utf8Value short_name(args[1]);
  String::Utf8Value filename(args[2]);
  String::Utf8Value mime_type(args[3]);
  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[4]->Uint32Value());

  enum ::AVCodecID codec_id =
    av_guess_codec(ref, *short_name, *filename, *mime_type, type);

  NanReturnValue(NanNew<Integer>(codec_id));
}

NAN_METHOD(AVOutputFormat::QueryCodec) {
  NanEscapableScope();

  if (!HasInstance(args[0]))
    return NanThrowTypeError("queryCodec: AVOutputFormat instance expected");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("queryCodec: codec_id enum required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("queryCodec: std_compliance integer required");

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args[0]->ToObject())->This();
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[1]->Uint32Value());
  int std_compliance = args[2]->Int32Value();

  int ret = avformat_query_codec(ref, codec_id, std_compliance);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_GETTER(AVOutputFormat::GetName) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  const char *name = ref->name;

  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetLongName) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  const char *long_name = ref->long_name;

  if (long_name)
    NanReturnValue(NanNew<String>(long_name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetMimeType) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  const char *mime_type = ref->mime_type;

  if (mime_type)
    NanReturnValue(NanNew<String>(mime_type));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetExtensions) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  const char *extensions = ref->extensions;

  if (extensions)
    NanReturnValue(NanNew<String>(extensions));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetAudioCodec) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  enum ::AVCodecID audio_codec = ref->audio_codec;

  NanReturnValue(NanNew<Integer>(audio_codec));
}

NAN_GETTER(AVOutputFormat::GetVideoCodec) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  enum ::AVCodecID video_codec = ref->video_codec;

  NanReturnValue(NanNew<Integer>(video_codec));
}

NAN_GETTER(AVOutputFormat::GetSubtitleCodec) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  enum ::AVCodecID subtitle_codec = ref->subtitle_codec;

  NanReturnValue(NanNew<Integer>(subtitle_codec));
}

NAN_GETTER(AVOutputFormat::GetFlags) {
  NanEscapableScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  int flags = ref->flags;

  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(AVOutputFormat::SetFlags) {
  NanScope();

  ::AVOutputFormat *ref = Unwrap<AVOutputFormat>(args.This())->This();
  ref->flags = value->Int32Value();
}

}  // namespace avformat
}  // namespace ffmpeg
