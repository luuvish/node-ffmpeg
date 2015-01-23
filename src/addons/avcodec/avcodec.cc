#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avcodec/avpicture.h"
#include "avcodec/avsubtitle.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


void FFmpeg::AVCodec::Initialize(Handle<Object> target) {
  NanScope();

  AVPacketWrapper::Initialize(target);
  AVCodecContextWrapper::Initialize(target);
  AVCodecWrapper::Initialize(target);
  AVPictureWrapper::Initialize(target);
  AVSubtitleRectWrapper::Initialize(target);
  AVSubtitleWrapper::Initialize(target);

  // libavcodec/avcodec.h
  NODE_DEFINE_CONSTANT(target, AVDISCARD_NONE);
  NODE_DEFINE_CONSTANT(target, AVDISCARD_DEFAULT);
  NODE_DEFINE_CONSTANT(target, AVDISCARD_NONREF);
  NODE_DEFINE_CONSTANT(target, AVDISCARD_BIDIR);
  NODE_DEFINE_CONSTANT(target, AVDISCARD_NONKEY);
  NODE_DEFINE_CONSTANT(target, AVDISCARD_ALL);

  //SetContant(target, "AVDISCARD_NONE", ::AVDISCARD_NONE);
  //SetContant(target, "AVDISCARD_DEFAULT", ::AVDISCARD_DEFAULT);
  //SetContant(target, "AVDISCARD_NONREF", ::AVDISCARD_NONREF);
  //SetContant(target, "AVDISCARD_BIDIR", ::AVDISCARD_BIDIR);
  //SetContant(target, "AVDISCARD_NONKEY", ::AVDISCARD_NONKEY);
  //SetContant(target, "AVDISCARD_ALL", ::AVDISCARD_ALL);

  SetContant(target, "CODEC_FLAG_UNALIGNED", CODEC_FLAG_UNALIGNED);
  SetContant(target, "CODEC_FLAG_QSCALE", CODEC_FLAG_QSCALE);
  SetContant(target, "CODEC_FLAG_4MV", CODEC_FLAG_4MV);
  SetContant(target, "CODEC_FLAG_OUTPUT_CORRUPT", CODEC_FLAG_OUTPUT_CORRUPT);
  SetContant(target, "CODEC_FLAG_QPEL", CODEC_FLAG_QPEL);
  SetContant(target, "CODEC_FLAG_GMC", CODEC_FLAG_GMC);
  SetContant(target, "CODEC_FLAG_MV0", CODEC_FLAG_MV0);
  SetContant(target, "CODEC_FLAG_INPUT_PRESERVED", CODEC_FLAG_INPUT_PRESERVED);
  SetContant(target, "CODEC_FLAG_PASS1", CODEC_FLAG_PASS1);
  SetContant(target, "CODEC_FLAG_PASS2", CODEC_FLAG_PASS2);
  SetContant(target, "CODEC_FLAG_GRAY", CODEC_FLAG_GRAY);
  SetContant(target, "CODEC_FLAG_EMU_EDGE", CODEC_FLAG_EMU_EDGE);
  SetContant(target, "CODEC_FLAG_PSNR", CODEC_FLAG_PSNR);
  SetContant(target, "CODEC_FLAG_TRUNCATED", CODEC_FLAG_TRUNCATED);
  SetContant(target, "CODEC_FLAG_NORMALIZE_AQP", CODEC_FLAG_NORMALIZE_AQP);
  SetContant(target, "CODEC_FLAG_INTERLACED_DCT", CODEC_FLAG_INTERLACED_DCT);
  SetContant(target, "CODEC_FLAG_LOW_DELAY", CODEC_FLAG_LOW_DELAY);
  SetContant(target, "CODEC_FLAG_GLOBAL_HEADER", CODEC_FLAG_GLOBAL_HEADER);
  SetContant(target, "CODEC_FLAG_BITEXACT", CODEC_FLAG_BITEXACT);
  SetContant(target, "CODEC_FLAG_AC_PRED", CODEC_FLAG_AC_PRED);
  SetContant(target, "CODEC_FLAG_LOOP_FILTER", CODEC_FLAG_LOOP_FILTER);
  SetContant(target, "CODEC_FLAG_INTERLACED_ME", CODEC_FLAG_INTERLACED_ME);
  SetContant(target, "CODEC_FLAG_CLOSED_GOP", CODEC_FLAG_CLOSED_GOP);

  SetContant(target, "CODEC_FLAG2_FAST", CODEC_FLAG2_FAST);
  SetContant(target, "CODEC_FLAG2_NO_OUTPUT", CODEC_FLAG2_NO_OUTPUT);
  SetContant(target, "CODEC_FLAG2_LOCAL_HEADER", CODEC_FLAG2_LOCAL_HEADER);
  SetContant(target, "CODEC_FLAG2_DROP_FRAME_TIMECODE",
                     CODEC_FLAG2_DROP_FRAME_TIMECODE);
  SetContant(target, "CODEC_FLAG2_IGNORE_CROP", CODEC_FLAG2_IGNORE_CROP);
  SetContant(target, "CODEC_FLAG2_CHUNKS", CODEC_FLAG2_CHUNKS);
  SetContant(target, "CODEC_FLAG2_SHOW_ALL", CODEC_FLAG2_SHOW_ALL);

  SetContant(target, "CODEC_CAP_DRAW_HORIZ_BAND", CODEC_CAP_DRAW_HORIZ_BAND);
  SetContant(target, "CODEC_CAP_DR1", CODEC_CAP_DR1);
  SetContant(target, "CODEC_CAP_TRUNCATED", CODEC_CAP_TRUNCATED);
  SetContant(target, "CODEC_CAP_HWACCEL", CODEC_CAP_HWACCEL);
  SetContant(target, "CODEC_CAP_DELAY", CODEC_CAP_DELAY);
  SetContant(target, "CODEC_CAP_SMALL_LAST_FRAME", CODEC_CAP_SMALL_LAST_FRAME);
  SetContant(target, "CODEC_CAP_HWACCEL_VDPAU", CODEC_CAP_HWACCEL_VDPAU);
  SetContant(target, "CODEC_CAP_SUBFRAMES", CODEC_CAP_SUBFRAMES);
  SetContant(target, "CODEC_CAP_EXPERIMENTAL", CODEC_CAP_EXPERIMENTAL);
  SetContant(target, "CODEC_CAP_CHANNEL_CONF", CODEC_CAP_CHANNEL_CONF);
  SetContant(target, "CODEC_CAP_NEG_LINESIZES", CODEC_CAP_NEG_LINESIZES);
  SetContant(target, "CODEC_CAP_FRAME_THREADS", CODEC_CAP_FRAME_THREADS);
  SetContant(target, "CODEC_CAP_SLICE_THREADS", CODEC_CAP_SLICE_THREADS);
  SetContant(target, "CODEC_CAP_PARAM_CHANGE", CODEC_CAP_PARAM_CHANGE);
  SetContant(target, "CODEC_CAP_AUTO_THREADS", CODEC_CAP_AUTO_THREADS);
  SetContant(target, "CODEC_CAP_VARIABLE_FRAME_SIZE",
                     CODEC_CAP_VARIABLE_FRAME_SIZE);
  SetContant(target, "CODEC_CAP_INTRA_ONLY", CODEC_CAP_INTRA_ONLY);
  SetContant(target, "CODEC_CAP_LOSSLESS", CODEC_CAP_LOSSLESS);

  SetContant(target, "SUBTITLE_NONE", ::SUBTITLE_NONE);
  SetContant(target, "SUBTITLE_BITMAP", ::SUBTITLE_BITMAP);
  SetContant(target, "SUBTITLE_TEXT", ::SUBTITLE_TEXT);
  SetContant(target, "SUBTITLE_ASS", ::SUBTITLE_ASS);
}


Persistent<FunctionTemplate> FFmpeg::AVCodec::AVCodecWrapper::constructor;

FFmpeg::AVCodec::AVCodecWrapper::AVCodecWrapper(::AVCodec *codec)
  : _this(codec), _allocated(false) {
  if (!_this) {
    _this = (::AVCodec *)av_mallocz(sizeof(::AVCodec));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVCodecWrapper::~AVCodecWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVCodec::AVCodecWrapper::Initialize(Handle<Object> target) {
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
  proto->SetAccessor(NanNew<String>("capabilities"), GetCapabilities);
  proto->SetAccessor(NanNew<String>("max_lowres"), GetMaxLowres);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVCodec"), creator);
  NODE_SET_METHOD(creator, "findDecoder", FindDecoder);
  NODE_SET_METHOD(creator, "findEncoder", FindEncoder);
  NODE_SET_METHOD(creator, "getType", GetMediaType);
  NODE_SET_METHOD(creator, "getName", GetCodecName);
}

Handle<Value> FFmpeg::AVCodec::AVCodecWrapper::newInstance(::AVCodec *codec) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(codec) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVCodecWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVCodec *codec = nullptr;
    if (args[0]->IsExternal())
      codec = static_cast<::AVCodec *>(External::Unwrap(args[0]));
    AVCodecWrapper *obj = new AVCodecWrapper(codec);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::FindDecoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  ::AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum ::AVCodecID codec_id =
      static_cast<enum ::AVCodecID>(args[0]->NumberValue());
    codec = avcodec_find_decoder(codec_id);
  }
  if (args[0]->IsString()) {
    String::Utf8Value codec_name(args[0]);
    codec = avcodec_find_decoder_by_name(*codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::FindEncoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  ::AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum ::AVCodecID codec_id =
      static_cast<enum ::AVCodecID>(args[0]->Uint32Value());
    codec = avcodec_find_encoder(codec_id);
  }
  if (args[0]->IsString()) {
    String::Utf8Value codec_name(args[0]);
    codec = avcodec_find_encoder_by_name(*codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::GetMediaType) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("codec id required");
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[0]->Uint32Value());
  enum ::AVMediaType type = avcodec_get_type(codec_id);
  NanReturnValue(NanNew<Number>(type));
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::GetCodecName) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("codec id required");
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[0]->NumberValue());
  const char *name = avcodec_get_name(codec_id);
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetIsEncoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_encoder = av_codec_is_encoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_encoder));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetIsDecoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_decoder = av_codec_is_decoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_decoder));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetLongName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetType) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum ::AVMediaType type = obj->_this->type;
  NanReturnValue(NanNew<Number>(type));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetId) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum ::AVCodecID id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetCapabilities) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int capabilities = obj->_this->capabilities;
  NanReturnValue(NanNew<Integer>(capabilities));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetMaxLowres) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  uint8_t max_lowres = obj->_this->max_lowres;
  NanReturnValue(NanNew<Number>(max_lowres));
}
