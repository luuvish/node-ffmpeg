#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


void FFmpeg::AVUtil::Initialize(Handle<Object> target) {
  NanScope();

  AVFrameWrapper::Initialize(target);

  // libavutil/channel_layout.h
  NODE_SET_METHOD(target, "getChannelLayoutNbChannels",
                  GetChannelLayoutNbChannels);
  NODE_SET_METHOD(target, "getDefaultChannelLayout",
                  GetDefaultChannelLayout);

  // libavutil/samplefmt.h
  SetContant(target, "AV_SAMPLE_FMT_NONE", ::AV_SAMPLE_FMT_NONE);
  SetContant(target, "AV_SAMPLE_FMT_U8", ::AV_SAMPLE_FMT_U8);
  SetContant(target, "AV_SAMPLE_FMT_S16", ::AV_SAMPLE_FMT_S16);
  SetContant(target, "AV_SAMPLE_FMT_S32", ::AV_SAMPLE_FMT_S32);
  SetContant(target, "AV_SAMPLE_FMT_FLT", ::AV_SAMPLE_FMT_FLT);
  SetContant(target, "AV_SAMPLE_FMT_DBL", ::AV_SAMPLE_FMT_DBL);
  SetContant(target, "AV_SAMPLE_FMT_U8P", ::AV_SAMPLE_FMT_U8P);
  SetContant(target, "AV_SAMPLE_FMT_S16P", ::AV_SAMPLE_FMT_S16P);
  SetContant(target, "AV_SAMPLE_FMT_S32P", ::AV_SAMPLE_FMT_S32P);
  SetContant(target, "AV_SAMPLE_FMT_FLTP", ::AV_SAMPLE_FMT_FLTP);
  SetContant(target, "AV_SAMPLE_FMT_DBLP", ::AV_SAMPLE_FMT_DBLP);

  NODE_SET_METHOD(target, "getPackedSampleFmt", GetPackedSampleFmt);
  NODE_SET_METHOD(target, "getPlanerSampleFmt", GetPlanarSampleFmt);
  NODE_SET_METHOD(target, "getBytesPerSample", GetBytesPerSample);
  NODE_SET_METHOD(target, "getSamplesBufferSize", GetSamplesBufferSize);
}

NAN_METHOD(FFmpeg::AVUtil::GetChannelLayoutNbChannels) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("channel_layout required");
  uint64_t channel_layout = args[0]->NumberValue();
  int ret = av_get_channel_layout_nb_channels(channel_layout);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVUtil::GetDefaultChannelLayout) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("nb_channels required");
  int nb_channels = args[0]->Int32Value();
  int64_t ret = av_get_default_channel_layout(nb_channels);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVUtil::GetPackedSampleFmt) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("sample_fmt required");
  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->NumberValue());
  enum ::AVSampleFormat ret = av_get_packed_sample_fmt(sample_fmt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVUtil::GetPlanarSampleFmt) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("sample_fmt required");
  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->NumberValue());
  enum ::AVSampleFormat ret = av_get_planar_sample_fmt(sample_fmt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVUtil::GetBytesPerSample) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("sample_fmt required");
  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[0]->NumberValue());
  int ret = av_get_bytes_per_sample(sample_fmt);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVUtil::GetSamplesBufferSize) {
  NanScope();

  int *linesize = nullptr;
  if (!args[0]->IsUndefined() && !args[0]->IsNull() && !args[0]->IsArray())
    return NanThrowTypeError("linesize required");
  if (args[0]->IsArray()) {
    Local<Object> arg0 = args[0]->ToObject();
    int length = arg0->Get(NanNew<String>("length"))->ToObject()->Uint32Value();
    linesize = reinterpret_cast<int *>(av_mallocz(length * sizeof(int)));
    for (int i = 0; i < length; i++)
      linesize[i] = arg0->Get(i)->Int32Value();
  }

  if (!args[1]->IsNumber() || !args[2]->IsNumber() ||
      !args[3]->IsNumber() || !args[4]->IsNumber())
    return NanThrowTypeError("nb_channels, nb_samples, "
                             "sample_fmt, align required");
  int nb_channels = args[1]->Int32Value();
  int nb_samples = args[2]->Int32Value();
  enum ::AVSampleFormat sample_fmt =
    static_cast<enum ::AVSampleFormat>(args[3]->NumberValue());
  int align = args[4]->Int32Value();

  int ret = av_samples_get_buffer_size(linesize,
                                       nb_channels,
                                       nb_samples,
                                       sample_fmt,
                                       align);
  av_freep(&linesize);
  NanReturnValue(NanNew<Number>(ret));
}


Persistent<FunctionTemplate> FFmpeg::AVUtil::AVFrameWrapper::constructor;

FFmpeg::AVUtil::AVFrameWrapper::AVFrameWrapper(::AVFrame *frame)
  : _this(frame), _allocated(false) {
  if (!_this) {
    _this = av_frame_alloc();
    _allocated = true;
  }
}

FFmpeg::AVUtil::AVFrameWrapper::~AVFrameWrapper() {
  if (_this && _allocated)
    av_frame_free(&_this);
}

void FFmpeg::AVUtil::AVFrameWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVFrame"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "unref", Unref);
  proto->SetAccessor(NanNew<String>("data"), GetData);
  proto->SetAccessor(NanNew<String>("linesize"), GetLinesize);
  proto->SetAccessor(NanNew<String>("width"), GetWidth);
  proto->SetAccessor(NanNew<String>("height"), GetHeight);
  proto->SetAccessor(NanNew<String>("nb_samples"), GetNbSamples);
  proto->SetAccessor(NanNew<String>("format"), GetFormat);
  proto->SetAccessor(NanNew<String>("sample_aspect_ratio"),
                     GetSampleAspectRatio, SetSampleAspectRatio);
  proto->SetAccessor(NanNew<String>("pts"), GetPts, SetPts);
  proto->SetAccessor(NanNew<String>("pkt_pts"), GetPktPts);
  proto->SetAccessor(NanNew<String>("pkt_dts"), GetPktDts);
  proto->SetAccessor(NanNew<String>("best_effort_timestamp"),
                     GetBestEffortTimestamp, SetBestEffortTimestamp);
  proto->SetAccessor(NanNew<String>("pkt_duration"),
                     GetPktDuration, SetPktDuration);
  proto->SetAccessor(NanNew<String>("pkt_pos"), GetPktPos, SetPktPos);
  proto->SetAccessor(NanNew<String>("channel_layout"),
                     GetChannelLayout, SetChannelLayout);
  proto->SetAccessor(NanNew<String>("channels"), GetChannels, SetChannels);
  proto->SetAccessor(NanNew<String>("sample_rate"),
                     GetSampleRate, SetSampleRate);
  proto->SetAccessor(NanNew<String>("decode_error_flags"),
                     GetDecodeErrorFlags, SetDecodeErrorFlags);
  proto->SetAccessor(NanNew<String>("pkt_size"), GetPktSize, SetPktSize);
  proto->SetAccessor(NanNew<String>("colorspace"),
                     GetColorspace, SetColorspace);
  proto->SetAccessor(NanNew<String>("color_range"),
                     GetColorRange, SetColorRange);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVFrame"), creator);
}

Handle<Value> FFmpeg::AVUtil::AVFrameWrapper::newInstance(::AVFrame *frame) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(frame) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVUtil::AVFrameWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVUtil::AVFrameWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVFrame *frame = nullptr;
    if (args[0]->IsExternal())
      frame = static_cast<::AVFrame *>(External::Unwrap(args[0]));
    AVFrameWrapper *obj = new AVFrameWrapper(frame);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVUtil::AVFrameWrapper::Unref) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  av_frame_unref(obj->_this);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetData) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  Handle<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    Local<Value> v;
    if (obj->_this->buf[i]) {
      char *data = reinterpret_cast<char *>(obj->_this->buf[i]->data);
      int size = obj->_this->buf[i]->size;
      Buffer *slowBuffer = Buffer::New(size);
      memcpy(Buffer::Data(slowBuffer), data, size);
      Local<Object> globalObj = Context::GetCurrent()->Global();
      Local<Function> bufferConstructor =
        Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
      Handle<Value> constructorArgs[3] =
        { slowBuffer->handle_, Integer::New(size), Integer::New(0) };
      v = bufferConstructor->NewInstance(3, constructorArgs);
      // Local<Object> buffer = NanNewBufferHandle(data, size);
      // Local<Object> arrayBuffer = Shell::CreateExternalArrayBuffer(size);
      // Local<Object> buffer =
      //   arrayBuffer->Get(String::New("buffer"))->ToObject();
      // memcpy(buffer->GetIndexedPropertiesExternalArrayData(), data, size);
      // Arguments args = new Arguments(1, &[arrayBuffer]);
      // Local<Object> nativeArray = Shell::Uint8Array(args);
    } else
      v = NanNull();
    ret->Set(i, v);
  }
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetLinesize) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  Handle<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++)
    ret->Set(i, NanNew<Number>(obj->_this->linesize[i]));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetWidth) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int width = obj->_this->width;
  NanReturnValue(NanNew<Number>(width));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetHeight) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int height = obj->_this->height;
  NanReturnValue(NanNew<Number>(height));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetNbSamples) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int nb_samples = obj->_this->nb_samples;
  NanReturnValue(NanNew<Number>(nb_samples));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetFormat) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int format = obj->_this->format;
  NanReturnValue(NanNew<Number>(format));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetSampleAspectRatio) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  ::AVRational sample_aspect_ratio = obj->_this->sample_aspect_ratio;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(sample_aspect_ratio.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(sample_aspect_ratio.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPts) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pts = obj->_this->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPktPts) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_pts = obj->_this->pkt_pts;
  NanReturnValue(NanNew<Number>(pkt_pts));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPktDts) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_dts = obj->_this->pkt_dts;
  NanReturnValue(NanNew<Number>(pkt_dts));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetBestEffortTimestamp) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t best_effort_timestamp =
    av_frame_get_best_effort_timestamp(obj->_this);
  NanReturnValue(NanNew<Number>(best_effort_timestamp));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPktDuration) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_duration = av_frame_get_pkt_duration(obj->_this);
  NanReturnValue(NanNew<Number>(pkt_duration));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPktPos) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_pos = av_frame_get_pkt_pos(obj->_this);
  NanReturnValue(NanNew<Number>(pkt_pos));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetChannelLayout) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t channel_layout = av_frame_get_channel_layout(obj->_this);
  NanReturnValue(NanNew<Number>(channel_layout));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetChannels) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int channels = av_frame_get_channels(obj->_this);
  NanReturnValue(NanNew<Number>(channels));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetSampleRate) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int sample_rate = av_frame_get_sample_rate(obj->_this);
  NanReturnValue(NanNew<Number>(sample_rate));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetDecodeErrorFlags) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int decode_error_flags = av_frame_get_decode_error_flags(obj->_this);
  NanReturnValue(NanNew<Number>(decode_error_flags));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetPktSize) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int pkt_size = av_frame_get_pkt_size(obj->_this);
  NanReturnValue(NanNew<Number>(pkt_size));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetColorspace) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  enum ::AVColorSpace colorspace = av_frame_get_colorspace(obj->_this);
  NanReturnValue(NanNew<Number>(colorspace));
}

NAN_GETTER(FFmpeg::AVUtil::AVFrameWrapper::GetColorRange) {
  NanScope();
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  enum ::AVColorRange color_range = av_frame_get_color_range(obj->_this);
  NanReturnValue(NanNew<Number>(color_range));
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetSampleAspectRatio) {
  NanScope();
  if (!value->IsObject())
    NanThrowTypeError("sample_aspect_ratio required");
  Local<Object> sar = value->ToObject();
  if (!sar->HasOwnProperty(NanNew<String>("num")) ||
      !sar->HasOwnProperty(NanNew<String>("den")))
    NanThrowTypeError("sample_aspect_ratio required");
  ::AVRational sample_aspect_ratio = {
    .num = sar->Get(NanNew<String>("num"))->Int32Value(),
    .den = sar->Get(NanNew<String>("den"))->Int32Value()
  };
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  obj->_this->sample_aspect_ratio = sample_aspect_ratio;
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetPts) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("pts required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pts = value->NumberValue();
  obj->_this->pts = pts;
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetBestEffortTimestamp) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("best_effort_timestamp required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t best_effort_timestamp = value->NumberValue();
  av_frame_set_best_effort_timestamp(obj->_this, best_effort_timestamp);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetPktDuration) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("pkt_duration required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_duration = value->NumberValue();
  av_frame_set_pkt_duration(obj->_this, pkt_duration);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetPktPos) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("pkt_pos required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t pkt_pos = value->NumberValue();
  av_frame_set_pkt_pos(obj->_this, pkt_pos);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetChannelLayout) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("channel_layout required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int64_t channel_layout = value->NumberValue();
  av_frame_set_channel_layout(obj->_this, channel_layout);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetChannels) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("channels required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int channels = value->Int32Value();
  av_frame_set_channels(obj->_this, channels);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetSampleRate) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("sample_rate required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int sample_rate = value->Int32Value();
  av_frame_set_sample_rate(obj->_this, sample_rate);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetDecodeErrorFlags) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("decode_error_flags required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int decode_error_flags = value->Int32Value();
  av_frame_set_decode_error_flags(obj->_this, decode_error_flags);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetPktSize) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("pkt_size required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  int pkt_size = value->Int32Value();
  av_frame_set_pkt_size(obj->_this, pkt_size);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetColorspace) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("colorspace required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  enum ::AVColorSpace colorspace =
    static_cast<enum ::AVColorSpace>(value->Uint32Value());
  av_frame_set_colorspace(obj->_this, colorspace);
}

NAN_SETTER(FFmpeg::AVUtil::AVFrameWrapper::SetColorRange) {
  NanScope();
  if (!value->IsNumber())
    NanThrowTypeError("color_range required");
  AVFrameWrapper *obj = ObjectWrap::Unwrap<AVFrameWrapper>(args.This());
  enum ::AVColorRange color_range =
    static_cast<enum ::AVColorRange>(value->Uint32Value());
  av_frame_set_color_range(obj->_this, color_range);
}
