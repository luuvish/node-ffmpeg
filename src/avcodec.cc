#include "avcodec.h"

using namespace node;
using namespace v8;


Persistent<FunctionTemplate> FFmpeg::AVPacketWrapper::constructor;

FFmpeg::AVPacketWrapper::AVPacketWrapper() : _this(nullptr) {}

FFmpeg::AVPacketWrapper::~AVPacketWrapper() {}

void FFmpeg::AVPacketWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVPacket"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("pts"), GetPts);
  proto->SetAccessor(NanNew<String>("dts"), GetDts);
  proto->SetAccessor(NanNew<String>("size"), GetSize);
  proto->SetAccessor(NanNew<String>("stream_index"), GetStreamIndex);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("pos"), GetPos);

  target->Set(NanNew<String>("AVPacket"), ctor->GetFunction());
}

Handle<Value> FFmpeg::AVPacketWrapper::newInstance(AVPacket *packet)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(ret);
  obj->_this = packet;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVPacketWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVPacketWrapper *obj = new AVPacketWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVPacketWrapper::GetPts) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t pts = obj->_this->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_GETTER(FFmpeg::AVPacketWrapper::GetDts) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t dts = obj->_this->dts;
  NanReturnValue(NanNew<Number>(dts));
}

NAN_GETTER(FFmpeg::AVPacketWrapper::GetSize) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int size = obj->_this->size;
  NanReturnValue(NanNew<Number>(size));
}

NAN_GETTER(FFmpeg::AVPacketWrapper::GetDuration) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVPacketWrapper::GetPos) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t pos = obj->_this->pos;
  NanReturnValue(NanNew<Number>(pos));
}


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
  NODE_SET_PROTOTYPE_METHOD(ctor, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(ctor, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeAudio", DecodeAudio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeVideo", DecodeVideo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeSubtitle", DecodeSubtitle);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeAudio", EncodeAudio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeVideo", EncodeVideo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeSubtitle", EncodeSubtitle);
  NODE_SET_PROTOTYPE_METHOD(ctor, "flushBuffers", FlushBuffers);
  proto->SetAccessor(NanNew<String>("is_open"), GetIsOpen);
  proto->SetAccessor(NanNew<String>("codec_type"), GetCodecType);
  proto->SetAccessor(NanNew<String>("codec"), GetCodec);
  proto->SetAccessor(NanNew<String>("codec_name"), GetCodecName);
  proto->SetAccessor(NanNew<String>("codec_id"), GetCodecId);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("width"), GetWidth);
  proto->SetAccessor(NanNew<String>("height"), GetHeight);
  proto->SetAccessor(NanNew<String>("pix_fmt"), GetPixFmt);
  proto->SetAccessor(NanNew<String>("sample_rate"), GetSampleRate);
  proto->SetAccessor(NanNew<String>("channels"), GetChannels);
  proto->SetAccessor(NanNew<String>("sample_fmt"), GetSampleFmt);
  proto->SetAccessor(NanNew<String>("channel_layout"), GetChannelLayout);
  proto->SetAccessor(NanNew<String>("workaround_bugs"), GetWorkaroundBugs, SetWorkaroundBugs);
  proto->SetAccessor(NanNew<String>("error_concealment"), GetErrorConcealment, SetErrorConcealment);
  proto->SetAccessor(NanNew<String>("lowres"), GetLowres, SetLowres);

  target->Set(NanNew<String>("AVCodecContext"), ctor->GetFunction());
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
  if (args.IsConstructCall()) {
    NanScope();
    AVCodecContextWrapper *obj = new AVCodecContextWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::Open) {
  NanScope();

  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());

  AVCodec *codec = nullptr;
  AVDictionary *options = nullptr;
  int argc = 0;

  if (!args[0]->IsUndefined()) {
    if (args[0]->IsObject()) {
      Local<Object> arg0 = args[0]->ToObject();
      if (NanHasInstance(AVCodecWrapper::constructor, arg0)) {
        codec = ObjectWrap::Unwrap<AVCodecWrapper>(arg0)->_this;
        argc++;
      }
    } else if (args[0]->IsNumber()) {
      codec = avcodec_find_decoder(static_cast<enum AVCodecID>(args[0]->Uint32Value()));
      argc++;
    } else if (args[0]->IsString()) {
      codec = avcodec_find_decoder_by_name(*String::Utf8Value(args[0]));
      argc++;
    }
  }
  if (argc == 0)
    codec = avcodec_find_decoder(obj->_this->codec_id);

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> opts = args[argc]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString())
        av_dict_set(&options, *String::Utf8Value(key), *String::Utf8Value(val), 0);
    }
    argc++;
  }

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("invalid arguments");
  }

  obj->_this->codec_id = codec->id;
  if (codec->capabilities & CODEC_CAP_DR1)
    obj->_this->flags |= CODEC_FLAG_EMU_EDGE;

  int ret = avcodec_open2(obj->_this, codec, &options);

  AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::Close) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int ret = avcodec_close(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::DecodeAudio) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::DecodeVideo) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::DecodeSubtitle) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::EncodeAudio) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::EncodeVideo) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::EncodeSubtitle) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecContextWrapper::FlushBuffers) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  avcodec_flush_buffers(obj->_this);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetIsOpen) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int is_open = avcodec_is_open(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_open));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodecType) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum AVMediaType codec_type = obj->_this->codec_type;
  NanReturnValue(NanNew<Number>(codec_type));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodec) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  const AVCodec *codec = obj->_this->codec;
  if (codec) {
    Handle<Value> ret = AVCodecWrapper::newInstance(const_cast<AVCodec *>(codec));
    NanReturnValue(ret);
  }
  NanReturnNull();
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodecName) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  const char *codec_name = obj->_this->codec_name;
  NanReturnValue(NanNew<String>(codec_name));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetCodecId) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum AVCodecID codec_id = obj->_this->codec_id;
  NanReturnValue(NanNew<Number>(codec_id));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetTimeBase) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
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

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetPixFmt) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum AVPixelFormat pix_fmt = obj->_this->pix_fmt;
  NanReturnValue(NanNew<Number>(pix_fmt));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetSampleRate) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int sample_rate = obj->_this->sample_rate;
  NanReturnValue(NanNew<Number>(sample_rate));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetChannels) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int channels = obj->_this->channels;
  NanReturnValue(NanNew<Number>(channels));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetSampleFmt) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum AVSampleFormat sample_fmt = obj->_this->sample_fmt;
  NanReturnValue(NanNew<Number>(sample_fmt));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetChannelLayout) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  uint64_t channel_layout = obj->_this->channel_layout;
  NanReturnValue(NanNew<Number>(channel_layout));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetWorkaroundBugs) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int workaround_bugs = obj->_this->workaround_bugs;
  NanReturnValue(NanNew<Number>(workaround_bugs));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetErrorConcealment) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int error_concealment = obj->_this->error_concealment;
  NanReturnValue(NanNew<Number>(error_concealment));
}

NAN_GETTER(FFmpeg::AVCodecContextWrapper::GetLowres) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int lowres = obj->_this->lowres;
  NanReturnValue(NanNew<Number>(lowres));
}

NAN_SETTER(FFmpeg::AVCodecContextWrapper::SetWorkaroundBugs) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->workaround_bugs = value->NumberValue();
}

NAN_SETTER(FFmpeg::AVCodecContextWrapper::SetErrorConcealment) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->error_concealment = value->NumberValue();
}

NAN_SETTER(FFmpeg::AVCodecContextWrapper::SetLowres) {
  NanScope();
  AVCodecContextWrapper *obj = ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->lowres = value->NumberValue();
}


Persistent<FunctionTemplate> FFmpeg::AVCodecWrapper::constructor;

FFmpeg::AVCodecWrapper::AVCodecWrapper() : _this(nullptr) {}

FFmpeg::AVCodecWrapper::~AVCodecWrapper() {}

void FFmpeg::AVCodecWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVCodec"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("is_encoder"), GetIsEncoder);
  proto->SetAccessor(NanNew<String>("is_decoder"), GetIsDecoder);
  proto->SetAccessor(NanNew<String>("name"), GetName);
  proto->SetAccessor(NanNew<String>("long_name"), GetLongName);
  proto->SetAccessor(NanNew<String>("type"), GetType);
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("max_lowres"), GetMaxLowres);

  target->Set(NanNew<String>("AVCodec"), ctor->GetFunction());
  NODE_SET_METHOD(ctor->GetFunction(), "findDecoder", FindDecoder);
  NODE_SET_METHOD(ctor->GetFunction(), "findEncoder", FindEncoder);
  NODE_SET_METHOD(ctor->GetFunction(), "getType", GetMediaType);
  NODE_SET_METHOD(ctor->GetFunction(), "getName", GetCodecName);
}

Handle<Value> FFmpeg::AVCodecWrapper::newInstance(AVCodec *codec)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(ret);
  obj->_this = codec;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVCodecWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVCodecWrapper *obj = new AVCodecWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodecWrapper::FindDecoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum AVCodecID codec_id = static_cast<enum AVCodecID>(args[0]->NumberValue());
    codec = avcodec_find_decoder(codec_id);
  }
  if (args[0]->IsString()) {
    const char *codec_name = *String::Utf8Value(args[0]);
    codec = avcodec_find_decoder_by_name(codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodecWrapper::FindEncoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum AVCodecID codec_id = static_cast<enum AVCodecID>(args[0]->Uint32Value());
    codec = avcodec_find_encoder(codec_id);
  }
  if (args[0]->IsString()) {
    const char *codec_name = *String::Utf8Value(args[0]);
    codec = avcodec_find_encoder_by_name(codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodecWrapper::GetMediaType) {
  NanScope();
  if (args[0]->IsNumber()) {
    enum AVCodecID codec_id = static_cast<enum AVCodecID>(args[0]->Uint32Value());
    enum AVMediaType type = avcodec_get_type(codec_id);
    NanReturnValue(NanNew<Number>(type));
  }
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodecWrapper::GetCodecName) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("codec id required");
  enum AVCodecID codec_id = static_cast<enum AVCodecID>(args[0]->NumberValue());
  const char *name = avcodec_get_name(codec_id);
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetIsEncoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_encoder = av_codec_is_encoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_encoder));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetIsDecoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_decoder = av_codec_is_decoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_decoder));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetLongName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetType) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum AVMediaType type = obj->_this->type;
  NanReturnValue(NanNew<Number>(type));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetId) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum AVCodecID id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVCodecWrapper::GetMaxLowres) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  uint8_t max_lowres = obj->_this->max_lowres;
  NanReturnValue(NanNew<Number>(max_lowres));
}


Persistent<FunctionTemplate> FFmpeg::AVPictureWrapper::constructor;

FFmpeg::AVPictureWrapper::AVPictureWrapper() : _this(nullptr) {}

FFmpeg::AVPictureWrapper::~AVPictureWrapper() {}

void FFmpeg::AVPictureWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVPicture"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("data"), GetData);
  proto->SetAccessor(NanNew<String>("line_size"), GetLineSize);

  target->Set(NanNew<String>("AVPicture"), ctor->GetFunction());
}

Handle<Value> FFmpeg::AVPictureWrapper::newInstance(AVPicture *picture)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVPictureWrapper *obj = ObjectWrap::Unwrap<AVPictureWrapper>(ret);
  obj->_this = picture;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVPictureWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVPictureWrapper *obj = new AVPictureWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVPictureWrapper::GetData) {
  NanScope();
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVPictureWrapper::GetLineSize) {
  NanScope();
  NanReturnUndefined();
}


Persistent<FunctionTemplate> FFmpeg::AVSubtitleRectWrapper::constructor;

FFmpeg::AVSubtitleRectWrapper::AVSubtitleRectWrapper() : _this(nullptr) {}

FFmpeg::AVSubtitleRectWrapper::~AVSubtitleRectWrapper() {}

void FFmpeg::AVSubtitleRectWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVSubtitleRect"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("x"), GetX);
  proto->SetAccessor(NanNew<String>("y"), GetY);
  proto->SetAccessor(NanNew<String>("w"), GetW);
  proto->SetAccessor(NanNew<String>("h"), GetH);
  proto->SetAccessor(NanNew<String>("nb_colors"), GetNbColors);
  proto->SetAccessor(NanNew<String>("pict"), GetPict);
  proto->SetAccessor(NanNew<String>("type"), GetType);
  proto->SetAccessor(NanNew<String>("text"), GetText);
  proto->SetAccessor(NanNew<String>("ass"), GetAss);

  target->Set(NanNew<String>("AVSubtitleRect"), ctor->GetFunction());
}

Handle<Value> FFmpeg::AVSubtitleRectWrapper::newInstance(AVSubtitleRect *rect)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(ret);
  obj->_this = rect;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVSubtitleRectWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVSubtitleRectWrapper *obj = new AVSubtitleRectWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetX) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int x = obj->_this->x;
  NanReturnValue(NanNew<Number>(x));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetY) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int y = obj->_this->y;
  NanReturnValue(NanNew<Number>(y));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetW) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int w = obj->_this->w;
  NanReturnValue(NanNew<Number>(w));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetH) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int h = obj->_this->h;
  NanReturnValue(NanNew<Number>(h));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetNbColors) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int nb_colors = obj->_this->nb_colors;
  NanReturnValue(NanNew<Number>(nb_colors));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetPict) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  Handle<Value> pict = AVPictureWrapper::newInstance(&obj->_this->pict);
  NanReturnValue(pict);
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetType) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  enum AVSubtitleType type = obj->_this->type;
  NanReturnValue(NanNew<Number>(type));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetText) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  char *text = obj->_this->text;
  NanReturnValue(NanNew<String>(text));
}

NAN_GETTER(FFmpeg::AVSubtitleRectWrapper::GetAss) {
  NanScope();
  AVSubtitleRectWrapper *obj = ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  char *ass = obj->_this->ass;
  NanReturnValue(NanNew<String>(ass));
}


Persistent<FunctionTemplate> FFmpeg::AVSubtitleWrapper::constructor;

FFmpeg::AVSubtitleWrapper::AVSubtitleWrapper() : _this(nullptr) {}

FFmpeg::AVSubtitleWrapper::~AVSubtitleWrapper() {}

void FFmpeg::AVSubtitleWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVSubtitle"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("format"), GetFormat);
  proto->SetAccessor(NanNew<String>("start_display_time"), GetStartDisplayTime);
  proto->SetAccessor(NanNew<String>("end_display_time"), GetEndDisplayTime);
  proto->SetAccessor(NanNew<String>("rects"), GetRects);
  proto->SetAccessor(NanNew<String>("pts"), GetPts);

  target->Set(NanNew<String>("AVSubtitle"), ctor->GetFunction());
}

Handle<Value> FFmpeg::AVSubtitleWrapper::newInstance(AVSubtitle *subtitle)
{
  NanScope();
  Local<Function> ctor = constructor->GetFunction();
  Handle<Object> ret = ctor->NewInstance();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(ret);
  obj->_this = subtitle;
  NanReturnValue(ret);
}

NAN_METHOD(FFmpeg::AVSubtitleWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    AVSubtitleWrapper *obj = new AVSubtitleWrapper;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVSubtitleWrapper::GetFormat) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint16_t format = obj->_this->format;
  NanReturnValue(NanNew<Number>(format));
}

NAN_GETTER(FFmpeg::AVSubtitleWrapper::GetStartDisplayTime) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint32_t start_display_time = obj->_this->start_display_time;
  NanReturnValue(NanNew<Number>(start_display_time));
}

NAN_GETTER(FFmpeg::AVSubtitleWrapper::GetEndDisplayTime) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint32_t end_display_time = obj->_this->end_display_time;
  NanReturnValue(NanNew<Number>(end_display_time));
}

NAN_GETTER(FFmpeg::AVSubtitleWrapper::GetRects) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  Handle<Array> rects = NanNew<Array>(obj->_this->num_rects);
  for (unsigned int i = 0; i < obj->_this->num_rects; i++) {
    Handle<Value> v = AVSubtitleRectWrapper::newInstance(obj->_this->rects[i]);
    rects->Set(i, v);
  }
  NanReturnValue(rects);
}

NAN_GETTER(FFmpeg::AVSubtitleWrapper::GetPts) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  int64_t pts = obj->_this->pts;
  NanReturnValue(NanNew<Number>(pts));
}