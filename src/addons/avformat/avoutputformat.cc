#include "avformat/avoutputformat.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVOutputFormat::constructor;

void AVOutputFormat::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVOutputFormat"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("name"), GetName);
  inst->SetAccessor(NanNew("long_name"), GetLongName);
  inst->SetAccessor(NanNew("mime_type"), GetMimeType);
  inst->SetAccessor(NanNew("extensions"), GetExtensions);
  inst->SetAccessor(NanNew("audio_codec"), GetAudioCodec);
  inst->SetAccessor(NanNew("video_codec"), GetVideoCodec);
  inst->SetAccessor(NanNew("subtitle_codec"), GetSubtitleCodec);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);

  NODE_SET_METHOD(tpl->GetFunction(), "guessFormat", GuessFormat);
  NODE_SET_METHOD(tpl->GetFunction(), "guessCodec", GuessCodec);
  NODE_SET_METHOD(tpl->GetFunction(), "queryCodec", QueryCodec);

  NanAssignPersistent(constructor, tpl);

  NODE_SET_METHOD(exports, "oformats", GetOFormats);
  NODE_SET_METHOD(exports, "guessFormat", GuessFormat);
  NODE_SET_METHOD(exports, "guessCodec", GuessCodec);
  NODE_SET_METHOD(exports, "queryCodec", QueryCodec);
}

Local<Object> AVOutputFormat::NewInstance(::AVOutputFormat* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVOutputFormat>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVOutputFormat::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVOutputFormat* AVOutputFormat::This(::AVOutputFormat* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVOutputFormat::GetOFormats) {
  NanScope();

  ::AVOutputFormat* fmt = nullptr;
  int size = 0;

  while ((fmt = av_oformat_next(fmt))) size++;

  Local<Array> ret = NanNew<Array>(size);
  fmt = nullptr;
  for (int i = 0; i < size; i++) {
    fmt = av_oformat_next(fmt);
    ret->Set(i, NewInstance(fmt));
  }

  if (size > 0)
    NanReturnValue(ret);
  else
    NanReturnNull();
}

NAN_METHOD(AVOutputFormat::GuessFormat) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("guessFormat: short_name string required");
  if (!args[1]->IsString())
    return NanThrowTypeError("guessFormat: filename string required");
  if (!args[2]->IsString())
    return NanThrowTypeError("guessFormat: mime_type string required");

  NanUtf8String short_name(args[0]);
  NanUtf8String filename(args[1]);
  NanUtf8String mime_type(args[2]);

  ::AVOutputFormat* wrap = av_guess_format(*short_name, *filename, *mime_type);

  if (wrap)
    NanReturnValue(NewInstance(wrap));
  else
    NanReturnNull();
}

NAN_METHOD(AVOutputFormat::GuessCodec) {
  NanScope();

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

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args[0]->ToObject())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  NanUtf8String short_name(args[1]);
  NanUtf8String filename(args[2]);
  NanUtf8String mime_type(args[3]);
  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[4]->Int32Value());

  enum ::AVCodecID codec_id =
    av_guess_codec(wrap, *short_name, *filename, *mime_type, type);

  NanReturnValue(NanNew<Uint32>(codec_id));
}

NAN_METHOD(AVOutputFormat::QueryCodec) {
  NanScope();

  if (!HasInstance(args[0]))
    return NanThrowTypeError("queryCodec: AVOutputFormat instance expected");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("queryCodec: codec_id enum required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("queryCodec: std_compliance integer required");

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args[0]->ToObject())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[1]->Uint32Value());
  int std_compliance = args[2]->Int32Value();

  int ret = avformat_query_codec(wrap, codec_id, std_compliance);

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVOutputFormat::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVOutputFormat* obj = new AVOutputFormat();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(AVOutputFormat::GetName) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* name = wrap->name;
  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetLongName) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* long_name = wrap->long_name;
  if (long_name)
    NanReturnValue(NanNew<String>(long_name));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetMimeType) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* mime_type = wrap->mime_type;
  if (mime_type)
    NanReturnValue(NanNew<String>(mime_type));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetExtensions) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* extensions = wrap->extensions;
  if (extensions)
    NanReturnValue(NanNew<String>(extensions));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVOutputFormat::GetAudioCodec) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID audio_codec = wrap->audio_codec;
  NanReturnValue(NanNew<Uint32>(audio_codec));
}

NAN_GETTER(AVOutputFormat::GetVideoCodec) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID video_codec = wrap->video_codec;
  NanReturnValue(NanNew<Uint32>(video_codec));
}

NAN_GETTER(AVOutputFormat::GetSubtitleCodec) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID subtitle_codec = wrap->subtitle_codec;
  NanReturnValue(NanNew<Uint32>(subtitle_codec));
}

NAN_GETTER(AVOutputFormat::GetFlags) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVOutputFormat::SetFlags) {
  NanScope();

  ::AVOutputFormat* wrap = Unwrap<AVOutputFormat>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

}  // namespace avformat
}  // namespace ffmpeg
