#include "avutil/avutil.h"
#include "avutil/channel_layout.h"
#include "avutil/pixfmt.h"
#include "avutil/samplefmt.h"
#include "avutil/avframe.h"

using namespace v8;

namespace ffmpeg {
namespace avutil {

NAN_METHOD(Version) {
  NanScope();

  unsigned ret = avutil_version();

  NanReturnValue(NanNew<Uint32>(ret));
}

NAN_METHOD(Configuration) {
  NanScope();

  const char* ret = avutil_configuration();

  if (ret)
    NanReturnValue(NanNew<String>(ret));
  else
    NanReturnEmptyString();
}

NAN_METHOD(License) {
  NanScope();

  const char* ret = avutil_license();

  if (ret)
    NanReturnValue(NanNew<String>(ret));
  else
    NanReturnEmptyString();
}

NAN_METHOD(GetTimeBaseQ) {
  NanScope();

  AVRational time_base_q = av_get_time_base_q();

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(time_base_q.num));
  ret->Set(NanNew("den"), NanNew<Int32>(time_base_q.den));

  NanReturnValue(ret);
}

void Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_SET_METHOD(obj, "version", Version);
  NODE_SET_METHOD(obj, "configuration", Configuration);
  NODE_SET_METHOD(obj, "license", License);

  AVMediaType::Init(obj);

  NODE_DEFINE_CONSTANT(obj, FF_LAMBDA_SHIFT);
  NODE_DEFINE_CONSTANT(obj, FF_LAMBDA_SCALE);
  NODE_DEFINE_CONSTANT(obj, FF_QP2LAMBDA);
  NODE_DEFINE_CONSTANT(obj, FF_LAMBDA_MAX);
  NODE_DEFINE_CONSTANT(obj, FF_QUALITY_SCALE);

  NODE_DEFINE_CONSTANT(obj, AV_NOPTS_VALUE);
  NODE_DEFINE_CONSTANT(obj, AV_TIME_BASE);

  Local<Object> time_base_q = NanNew<Object>();
  time_base_q->Set(NanNew("num"), NanNew<Int32>(1));
  time_base_q->Set(NanNew("den"), NanNew<Int32>(AV_TIME_BASE));
  obj->Set(NanNew("AV_TIME_BASE_Q"), time_base_q);

  AVPictureType::Init(obj);

  NODE_SET_METHOD(obj, "getTimeBaseQ", GetTimeBaseQ);

  ChannelLayout::Init(obj);

  AVPixelFormat::Init(obj);
  AVColorPrimaries::Init(obj);
  AVColorTransferCharacteristic::Init(obj);
  AVColorSpace::Init(obj);
  AVColorRange::Init(obj);
  AVChromaLocation::Init(obj);

  AVSampleFormat::Init(obj);
  AVFrame::Init(obj);

  exports->Set(NanNew("avutil"), obj);
}

void AVMediaType::Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_UNKNOWN);
  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_VIDEO);
  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_AUDIO);
  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_DATA);
  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_SUBTITLE);
  NODE_DEFINE_CONSTANT(obj, AVMEDIA_TYPE_ATTACHMENT);

  NODE_SET_METHOD(obj, "getMediaTypeString", GetMediaTypeString);

  exports->Set(NanNew("AVMediaType"), obj);
}

NAN_METHOD(AVMediaType::GetMediaTypeString) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getMediaTypeString: media_type enum required");

  enum ::AVMediaType media_type =
    static_cast<enum ::AVMediaType>(args[0]->Int32Value());

  const char* ret = av_get_media_type_string(media_type);

  if (ret)
    NanReturnValue(NanNew<String>(ret));
  else
    NanReturnEmptyString();
}

void AVPictureType::Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_NONE);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_I);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_P);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_B);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_S);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_SI);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_SP);
  NODE_DEFINE_CONSTANT(obj, AV_PICTURE_TYPE_BI);

  NODE_SET_METHOD(obj, "getPictureTypeChar", GetPictureTypeChar);

  exports->Set(NanNew("AVPictureType"), obj);
}

NAN_METHOD(AVPictureType::GetPictureTypeChar) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getPictureTypeChar: pict_type enum required");

  enum ::AVPictureType pict_type =
    static_cast<enum ::AVPictureType>(args[0]->Uint32Value());

  char ret[2] = {av_get_picture_type_char(pict_type), 0};

  NanReturnValue(NanNew<String>(ret));
}

}  // namespace avutil
}  // namespace ffmpeg
