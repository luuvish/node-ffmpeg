#include "avutil/samplefmt.h"

using namespace v8;

namespace ffmpeg {
namespace avutil {

void AVSampleFormat::Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_NONE);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_U8);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_S16);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_S32);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_FLT);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_DBL);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_U8P);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_S16P);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_S32P);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_FLTP);
  NODE_DEFINE_CONSTANT(obj, AV_SAMPLE_FMT_DBLP);

  NODE_SET_METHOD(obj, "getSampleFmtName", GetSampleFmtName);
  NODE_SET_METHOD(obj, "getSampleFmt", GetSampleFmt);
  NODE_SET_METHOD(obj, "getAltSampleFmt", GetAltSampleFmt);
  NODE_SET_METHOD(obj, "getPackedSampleFmt", GetPackedSampleFmt);
  NODE_SET_METHOD(obj, "getPlanarSampleFmt", GetPlanarSampleFmt);
  NODE_SET_METHOD(obj, "getSampleFmtString", GetSampleFmtString);
  NODE_SET_METHOD(obj, "getBytesPerSample", GetBytesPerSample);
  NODE_SET_METHOD(obj, "isSampleFmtPlanar", IsSampleFmtPlanar);
  NODE_SET_METHOD(obj, "getSamplesBufferSize", GetSamplesBufferSize);

  exports->Set(NanNew("AVSampleFormat"), obj);
}

NAN_METHOD(AVSampleFormat::GetSampleFmtName) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getSampleFmtName: AVSampleFormat enum required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  const char* name = av_get_sample_fmt_name(sample_fmt);
  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_METHOD(AVSampleFormat::GetSampleFmt) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("getSampleFmt: name string required");

  NanUtf8String name(args[0]);
  enum ::AVSampleFormat ret = av_get_sample_fmt(*name);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVSampleFormat::GetAltSampleFmt) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getAltSampleFmt: AVSampleFormat enum required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("getAltSampleFmt: planar integer required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  int planar = args[1]->Int32Value();
  enum ::AVSampleFormat ret = av_get_alt_sample_fmt(sample_fmt, planar);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVSampleFormat::GetPackedSampleFmt) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getPackedSampleFmt: AVSampleFormat enum required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  enum ::AVSampleFormat ret = av_get_packed_sample_fmt(sample_fmt);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVSampleFormat::GetPlanarSampleFmt) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getPlanarSampleFmt: AVSampleFormat enum required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  enum ::AVSampleFormat ret = av_get_planar_sample_fmt(sample_fmt);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVSampleFormat::GetSampleFmtString) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(AVSampleFormat::GetBytesPerSample) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getBytesPerSample: AVSampleFormat enum required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  int ret = av_get_bytes_per_sample(sample_fmt);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVSampleFormat::IsSampleFmtPlanar) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("isSampleFmtPlanar: AVSampleFormat enum required");

  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->Int32Value());
  int ret = av_sample_fmt_is_planar(sample_fmt);
  NanReturnValue(NanNew<Boolean>(ret));
}

NAN_METHOD(AVSampleFormat::GetSamplesBufferSize) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getSamplesBufferSize: nb_channels integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("getSamplesBufferSize: nb_samples integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("getSamplesBufferSize: AVSampleFormat enum required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("getSamplesBufferSize: align integer required");

  int linesize;
  int nb_channels = args[0]->Int32Value();
  int nb_samples = args[1]->Int32Value();
  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[2]->Int32Value());
  int align = args[3]->Int32Value();

  int ret = av_samples_get_buffer_size(
    &linesize, nb_channels, nb_samples, sample_fmt, align);

  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Int32>(linesize));
  NanReturnValue(rets);
}

}  // namespace avutil
}  // namespace ffmpeg
