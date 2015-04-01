#include "avutil/channel_layout.h"

using namespace v8;

namespace ffmpeg {
namespace avutil {

void ChannelLayout::Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_DEFINE_CONSTANT(obj, AV_CH_FRONT_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_FRONT_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_FRONT_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LOW_FREQUENCY);
  NODE_DEFINE_CONSTANT(obj, AV_CH_BACK_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_BACK_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_FRONT_LEFT_OF_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_FRONT_RIGHT_OF_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_BACK_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_SIDE_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_SIDE_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_FRONT_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_FRONT_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_FRONT_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_BACK_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_BACK_CENTER);
  NODE_DEFINE_CONSTANT(obj, AV_CH_TOP_BACK_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_STEREO_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_STEREO_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_WIDE_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_WIDE_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_SURROUND_DIRECT_LEFT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_SURROUND_DIRECT_RIGHT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LOW_FREQUENCY_2);

  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_NATIVE);

  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_MONO);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_STEREO);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_2POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_2_1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_SURROUND);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_3POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_4POINT0);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_4POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_2_2);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_QUAD);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_5POINT0);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_5POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_5POINT0_BACK);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_5POINT1_BACK);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_6POINT0);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_6POINT0_FRONT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_HEXAGONAL);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_6POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_6POINT1_BACK);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_6POINT1_FRONT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_7POINT0);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_7POINT0_FRONT);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_7POINT1);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_7POINT1_WIDE);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_7POINT1_WIDE_BACK);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_OCTAGONAL);
  NODE_DEFINE_CONSTANT(obj, AV_CH_LAYOUT_STEREO_DOWNMIX);

  NODE_SET_METHOD(obj, "getChannelLayout", GetChannelLayout);
  NODE_SET_METHOD(obj, "getChannelLayoutString", GetChannelLayoutString);
  NODE_SET_METHOD(obj, "getChannelLayoutNbChannels",
                  GetChannelLayoutNbChannels);
  NODE_SET_METHOD(obj, "getDefaultChannelLayout", GetDefaultChannelLayout);
  NODE_SET_METHOD(obj, "getChannelLayoutChannelIndex",
                  GetChannelLayoutChannelIndex);
  NODE_SET_METHOD(obj, "extractChannelLayout", ExtractChannelLayout);
  NODE_SET_METHOD(obj, "getChannelName", GetChannelName);
  NODE_SET_METHOD(obj, "getChannelDescription", GetChannelDescription);
  NODE_SET_METHOD(obj, "getStandardChannelLayout", GetStandardChannelLayout);

  exports->Set(NanNew("channel_layout"), obj);

  AVMatrixEncoding::Init(exports);
}

NAN_METHOD(ChannelLayout::GetChannelLayout) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("get_channel_layout: name string required");

  NanUtf8String name(args[0]);
  uint64_t ret = av_get_channel_layout(*name);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(ChannelLayout::GetChannelLayoutString) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(ChannelLayout::GetChannelLayoutNbChannels) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_channel_layout_nb_channels: channel_layout integer required");

  uint64_t channel_layout = args[0]->IntegerValue();
  int ret = av_get_channel_layout_nb_channels(channel_layout);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(ChannelLayout::GetDefaultChannelLayout) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_default_channel_layout: nb_channels integer required");

  int nb_channels = args[0]->Int32Value();
  int64_t ret = av_get_default_channel_layout(nb_channels);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(ChannelLayout::GetChannelLayoutChannelIndex) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_channel_layout_channel_index: channel_layout integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("get_channel_layout_channel_index: channel integer required");

  uint64_t channel_layout = args[0]->IntegerValue();
  uint64_t channel = args[1]->IntegerValue();
  int ret = av_get_channel_layout_channel_index(channel_layout, channel);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(ChannelLayout::ExtractChannelLayout) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("extract_channel_layout: channel_layout integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("extract_channel_layout: index integer required");

  uint64_t channel_layout = args[0]->IntegerValue();
  int index = args[1]->Int32Value();
  uint64_t ret = av_channel_layout_extract_channel(channel_layout, index);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(ChannelLayout::GetChannelName) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_channel_name: channel integer required");

  uint64_t channel = args[0]->IntegerValue();
  const char* name = av_get_channel_name(channel);
  if (name)
    NanReturnValue(NanNew<String>(name));
  else
    NanReturnEmptyString();
}

NAN_METHOD(ChannelLayout::GetChannelDescription) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_channel_description: channel integer required");

  uint64_t channel = args[0]->IntegerValue();
  const char* description = av_get_channel_description(channel);
  if (description)
    NanReturnValue(NanNew<String>(description));
  else
    NanReturnEmptyString();
}

NAN_METHOD(ChannelLayout::GetStandardChannelLayout) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("get_standard_channel_layout: index integer required");

  unsigned index = args[0]->Uint32Value();
  uint64_t layout;
  const char* name;

  int ret = av_get_standard_channel_layout(index, &layout, &name);
  if (ret == 0) {
    Local<Array> rets = NanNew<Array>(2);
    rets->Set(0, NanNew<Number>(layout));
    rets->Set(1, NanNew<String>(name));
    NanReturnValue(rets);
  } else
    NanReturnValue(NanNew<Int32>(ret));
}

void AVMatrixEncoding::Init(Handle<Object> exports) {
  NanScope();

  Local<Object> obj = NanNew<Object>();

  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_NONE);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DOLBY);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DPLII);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DPLIIX);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DPLIIZ);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DOLBYEX);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_DOLBYHEADPHONE);
  NODE_DEFINE_CONSTANT(obj, AV_MATRIX_ENCODING_NB);

  exports->Set(NanNew("AVMatrixEncoding"), obj);
}

}  // namespace avutil
}  // namespace ffmpeg
