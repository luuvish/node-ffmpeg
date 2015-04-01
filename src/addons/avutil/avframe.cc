#include "avutil/avframe.h"

using namespace v8;

namespace ffmpeg {
namespace avutil {

Persistent<FunctionTemplate> AVFrame::constructor;

void AVFrame::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVFrame"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "ref", Ref);
  NODE_SET_PROTOTYPE_METHOD(tpl, "unref", Unref);
  NODE_SET_PROTOTYPE_METHOD(tpl, "moveRef", MoveRef);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getBuffer", GetBuffer);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", Copy);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copyProps", CopyProps);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("is_writable"), GetIsWritable, SetIsWritable);
  inst->SetAccessor(NanNew("data"), GetData);
  inst->SetAccessor(NanNew("linesize"), GetLinesize);
  inst->SetAccessor(NanNew("width"), GetWidth, SetWidth);
  inst->SetAccessor(NanNew("height"), GetHeight, SetHeight);
  inst->SetAccessor(NanNew("nb_samples"), GetNbSamples, SetNbSamples);
  inst->SetAccessor(NanNew("format"), GetFormat, SetFormat);
  inst->SetAccessor(NanNew("key_frame"), GetKeyFrame);
  inst->SetAccessor(NanNew("pict_type"), GetPictType, SetPictType);
  inst->SetAccessor(NanNew("sample_aspect_ratio"),
                    GetSampleAspectRatio, SetSampleAspectRatio);
  inst->SetAccessor(NanNew("pts"), GetPts, SetPts);
  inst->SetAccessor(NanNew("pkt_pts"), GetPktPts);
  inst->SetAccessor(NanNew("pkt_dts"), GetPktDts);
  inst->SetAccessor(NanNew("coded_picture_number"), GetCodedPictureNumber);
  inst->SetAccessor(NanNew("display_picture_number"), GetDisplayPictureNumber);
  inst->SetAccessor(NanNew("quality"), GetQuality, SetQuality);
  inst->SetAccessor(NanNew("repeat_pict"), GetRepeatPict, SetRepeatPict);
  inst->SetAccessor(NanNew("interlaced_frame"),
                    GetInterlacedFrame, SetInterlacedFrame);
  inst->SetAccessor(NanNew("top_field_first"),
                    GetTopFieldFirst, SetTopFieldFirst);
  inst->SetAccessor(NanNew("palette_has_changed"), GetPaletteHasChanged);
  inst->SetAccessor(NanNew("reordered_opaque"), GetReorderedOpaque);
  inst->SetAccessor(NanNew("sample_rate"), GetSampleRate, SetSampleRate);
  inst->SetAccessor(NanNew("channel_layout"),
                    GetChannelLayout, SetChannelLayout);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("color_range"), GetColorRange, SetColorRange);
  inst->SetAccessor(NanNew("color_primaries"), GetColorPrimaries);
  inst->SetAccessor(NanNew("color_trc"), GetColorTrc);
  inst->SetAccessor(NanNew("colorspace"), GetColorspace, SetColorspace);
  inst->SetAccessor(NanNew("chroma_location"), GetChromaLocation);
  inst->SetAccessor(NanNew("best_effort_timestamp"),
                    GetBestEffortTimestamp, SetBestEffortTimestamp);
  inst->SetAccessor(NanNew("pkt_pos"), GetPktPos, SetPktPos);
  inst->SetAccessor(NanNew("pkt_duration"), GetPktDuration, SetPktDuration);
  inst->SetAccessor(NanNew("metadata"), GetMetadata, SetMetadata);
  inst->SetAccessor(NanNew("decode_error_flags"),
                    GetDecodeErrorFlags, SetDecodeErrorFlags);
  inst->SetAccessor(NanNew("channels"), GetChannels, SetChannels);
  inst->SetAccessor(NanNew("pkt_size"), GetPktSize, SetPktSize);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVFrame"), tpl->GetFunction());
}

Local<Object> AVFrame::NewInstance(::AVFrame* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVFrame>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVFrame::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVFrame* AVFrame::This(::AVFrame* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

AVFrame::AVFrame() : this_(nullptr) {
  this_ = av_frame_alloc();
  if (this_ == nullptr)
    NanThrowTypeError("AVFrame: cannot allocation");
}

AVFrame::~AVFrame() {
  if (this_ != nullptr)
    av_frame_free(&this_);
}

NAN_METHOD(AVFrame::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVFrame* obj = new AVFrame();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVFrame::Ref) {
  NanScope();

  if (!AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("ref: AVFrame instance required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* src = Unwrap<AVFrame>(args[0]->ToObject())->This();
  int ret = av_frame_ref(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFrame::Unref) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  av_frame_unref(wrap);
  NanReturnUndefined();
}

NAN_METHOD(AVFrame::MoveRef) {
  NanScope();

  if (!AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("ref: AVFrame instance required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* src = Unwrap<AVFrame>(args[0]->ToObject())->This();
  av_frame_move_ref(wrap, src);
  NanReturnUndefined();
}

NAN_METHOD(AVFrame::GetBuffer) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getBuffer: align integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int align = args[0]->Int32Value();
  int ret = av_frame_get_buffer(wrap, align);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFrame::Copy) {
  NanScope();

  if (!AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("copy: AVFrame instance required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* src = Unwrap<AVFrame>(args[0]->ToObject())->This();
  int ret = av_frame_copy(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFrame::CopyProps) {
  NanScope();

  if (!AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("ref: AVFrame instance required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* src = Unwrap<AVFrame>(args[0]->ToObject())->This();
  int ret = av_frame_copy_props(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_GETTER(AVFrame::GetIsWritable) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int writable = av_frame_is_writable(wrap);
  NanReturnValue(NanNew<Boolean>(writable));
}

NAN_SETTER(AVFrame::SetIsWritable) {
  NanScope();

  if (!value->IsBoolean())
    NanThrowTypeError("writable: boolean required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    bool writable = value->BooleanValue();
    if (writable)
      av_frame_make_writable(wrap);
  }
}

NAN_GETTER(AVFrame::GetData) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> rets = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    Local<Object> ret = NanNew<Object>();
    if (wrap->buf[i]) {
      void* data = wrap->buf[i]->data;
      ExternalArrayType type = kExternalUint8Array;
      int size = wrap->buf[i]->size;
      ret->SetIndexedPropertiesToExternalArrayData(data, type, size);
    }
    rets->Set(i, ret);
  }
  NanReturnValue(rets);
}

NAN_GETTER(AVFrame::GetLinesize) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> rets = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++)
    rets->Set(i, NanNew<Int32>(wrap->linesize[i]));
  NanReturnValue(rets);
}

NAN_GETTER(AVFrame::GetWidth) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int width = wrap->width;
  NanReturnValue(NanNew<Int32>(width));
}

NAN_SETTER(AVFrame::SetWidth) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("width: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->width = value->Int32Value();
}

NAN_GETTER(AVFrame::GetHeight) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int height = wrap->height;
  NanReturnValue(NanNew<Int32>(height));
}

NAN_SETTER(AVFrame::SetHeight) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("height: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->height = value->Int32Value();
}

NAN_GETTER(AVFrame::GetNbSamples) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int nb_samples = wrap->nb_samples;
  NanReturnValue(NanNew<Int32>(nb_samples));
}

NAN_SETTER(AVFrame::SetNbSamples) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("nb_samples: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->nb_samples = value->Int32Value();
}

NAN_GETTER(AVFrame::GetFormat) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int format = wrap->format;
  NanReturnValue(NanNew<Int32>(format));
}

NAN_SETTER(AVFrame::SetFormat) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("format: enum required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->format = value->Int32Value();
}

NAN_GETTER(AVFrame::GetKeyFrame) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int key_frame = wrap->key_frame;
  NanReturnValue(NanNew<Int32>(key_frame));
}

NAN_GETTER(AVFrame::GetPictType) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVPictureType pict_type = wrap->pict_type;
  NanReturnValue(NanNew<Uint32>(pict_type));
}

NAN_SETTER(AVFrame::SetPictType) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pict_type: AVPictureType enum required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->pict_type = static_cast<enum ::AVPictureType>(value->Uint32Value());
}

NAN_GETTER(AVFrame::GetSampleAspectRatio) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational sar = wrap->sample_aspect_ratio;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(sar.num));
  ret->Set(NanNew("den"), NanNew<Int32>(sar.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVFrame::SetSampleAspectRatio) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("sample_aspect_ratio: AVRational required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    Local<Object> sar = value->ToObject();
    wrap->sample_aspect_ratio.num = sar->Get(NanNew("num"))->Int32Value();
    wrap->sample_aspect_ratio.den = sar->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVFrame::GetPts) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts = wrap->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_SETTER(AVFrame::SetPts) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pts: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->pts = value->IntegerValue();
}

NAN_GETTER(AVFrame::GetPktPts) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pkt_pts = wrap->pkt_pts;
  NanReturnValue(NanNew<Number>(pkt_pts));
}

NAN_GETTER(AVFrame::GetPktDts) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pkt_dts = wrap->pkt_dts;
  NanReturnValue(NanNew<Number>(pkt_dts));
}

NAN_GETTER(AVFrame::GetCodedPictureNumber) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int coded_picture_number = wrap->coded_picture_number;
  NanReturnValue(NanNew<Int32>(coded_picture_number));
}

NAN_GETTER(AVFrame::GetDisplayPictureNumber) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int display_picture_number = wrap->display_picture_number;
  NanReturnValue(NanNew<Int32>(display_picture_number));
}

NAN_GETTER(AVFrame::GetQuality) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int quality = wrap->quality;
  NanReturnValue(NanNew<Int32>(quality));
}

NAN_SETTER(AVFrame::SetQuality) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("quality: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->quality = value->Int32Value();
}

NAN_GETTER(AVFrame::GetRepeatPict) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int repeat_pict = wrap->repeat_pict;
  NanReturnValue(NanNew<Int32>(repeat_pict));
}

NAN_SETTER(AVFrame::SetRepeatPict) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("repeat_pict: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->repeat_pict = value->Int32Value();
}

NAN_GETTER(AVFrame::GetInterlacedFrame) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int interlaced_frame = wrap->interlaced_frame;
  NanReturnValue(NanNew<Int32>(interlaced_frame));
}

NAN_SETTER(AVFrame::SetInterlacedFrame) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("interlaced_frame: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->interlaced_frame = value->Int32Value();
}

NAN_GETTER(AVFrame::GetTopFieldFirst) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int top_field_first = wrap->top_field_first;
  NanReturnValue(NanNew<Int32>(top_field_first));
}

NAN_SETTER(AVFrame::SetTopFieldFirst) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("top_field_first: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->top_field_first = value->Int32Value();
}

NAN_GETTER(AVFrame::GetPaletteHasChanged) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int palette_has_changed = wrap->palette_has_changed;
  NanReturnValue(NanNew<Int32>(palette_has_changed));
}

NAN_GETTER(AVFrame::GetReorderedOpaque) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t reordered_opaque = wrap->reordered_opaque;
  NanReturnValue(NanNew<Number>(reordered_opaque));
}

NAN_GETTER(AVFrame::GetSampleRate) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int sample_rate = av_frame_get_sample_rate(wrap);
  NanReturnValue(NanNew<Int32>(sample_rate));
}

NAN_SETTER(AVFrame::SetSampleRate) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("sample_rate: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int sample_rate = value->Int32Value();
    av_frame_set_sample_rate(wrap, sample_rate);
  }
}

NAN_GETTER(AVFrame::GetChannelLayout) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint64_t channel_layout = av_frame_get_channel_layout(wrap);
  NanReturnValue(NanNew<Number>(channel_layout));
}

NAN_SETTER(AVFrame::SetChannelLayout) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("channel_layout: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    uint64_t channel_layout = value->IntegerValue();
    av_frame_set_channel_layout(wrap, channel_layout);
  }
}

NAN_GETTER(AVFrame::GetFlags) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVFrame::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVFrame::GetColorRange) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorRange color_range = av_frame_get_color_range(wrap);
  NanReturnValue(NanNew<Uint32>(color_range));
}

NAN_SETTER(AVFrame::SetColorRange) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("color_range: enum required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    enum ::AVColorRange color_range =
      static_cast<enum ::AVColorRange>(value->Uint32Value());
    av_frame_set_color_range(wrap, color_range);
  }
}

NAN_GETTER(AVFrame::GetColorPrimaries) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorPrimaries color_primaries = wrap->color_primaries;
  NanReturnValue(NanNew<Uint32>(color_primaries));
}

NAN_GETTER(AVFrame::GetColorTrc) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorTransferCharacteristic color_trc = wrap->color_trc;
  NanReturnValue(NanNew<Uint32>(color_trc));
}

NAN_GETTER(AVFrame::GetColorspace) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorSpace colorspace = av_frame_get_colorspace(wrap);
  NanReturnValue(NanNew<Uint32>(colorspace));
}

NAN_SETTER(AVFrame::SetColorspace) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("colorspace: enum required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    enum ::AVColorSpace colorspace =
      static_cast<enum ::AVColorSpace>(value->Uint32Value());
    av_frame_set_colorspace(wrap, colorspace);
  }
}

NAN_GETTER(AVFrame::GetChromaLocation) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVChromaLocation chroma_location = wrap->chroma_location;
  NanReturnValue(NanNew<Uint32>(chroma_location));
}

NAN_GETTER(AVFrame::GetBestEffortTimestamp) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t best_effort_timestamp = av_frame_get_best_effort_timestamp(wrap);
  NanReturnValue(NanNew<Number>(best_effort_timestamp));
}

NAN_SETTER(AVFrame::SetBestEffortTimestamp) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("best_effort_timestamp: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int64_t best_effort_timestamp = value->IntegerValue();
    av_frame_set_best_effort_timestamp(wrap, best_effort_timestamp);
  }
}

NAN_GETTER(AVFrame::GetPktPos) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pkt_pos = av_frame_get_pkt_pos(wrap);
  NanReturnValue(NanNew<Number>(pkt_pos));
}

NAN_SETTER(AVFrame::SetPktPos) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pkt_pos: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int64_t pkt_pos = value->IntegerValue();
    av_frame_set_pkt_pos(wrap, pkt_pos);
  }
}

NAN_GETTER(AVFrame::GetPktDuration) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pkt_duration = av_frame_get_pkt_duration(wrap);
  NanReturnValue(NanNew<Number>(pkt_duration));
}

NAN_SETTER(AVFrame::SetPktDuration) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pkt_duration: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int64_t pkt_duration = value->IntegerValue();
    av_frame_set_pkt_duration(wrap, pkt_duration);
  }
}

NAN_GETTER(AVFrame::GetMetadata) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = NanNew<Object>();
  AVDictionary* metadata = av_frame_get_metadata(wrap);
  AVDictionaryEntry* t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_SETTER(AVFrame::SetMetadata) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("metadata: object required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    AVDictionary* metadata = nullptr;

    Local<Object> opts = value->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString()) {
        NanUtf8String key_str(key);
        NanUtf8String val_str(val);
        av_dict_set(&metadata, *key_str, *val_str, 0);
      }
    }

    av_dict_free(&wrap->metadata);
    av_frame_set_metadata(wrap, metadata);
  }
}

NAN_GETTER(AVFrame::GetDecodeErrorFlags) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int decode_error_flags = av_frame_get_decode_error_flags(wrap);
  NanReturnValue(NanNew<Int32>(decode_error_flags));
}

NAN_SETTER(AVFrame::SetDecodeErrorFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("decode_error_flags: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int decode_error_flags = value->Int32Value();
    av_frame_set_decode_error_flags(wrap, decode_error_flags);
  }
}

NAN_GETTER(AVFrame::GetChannels) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int channels = av_frame_get_channels(wrap);
  NanReturnValue(NanNew<Int32>(channels));
}

NAN_SETTER(AVFrame::SetChannels) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("channels: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int channels = value->Int32Value();
    av_frame_set_channels(wrap, channels);
  }
}

NAN_GETTER(AVFrame::GetPktSize) {
  NanScope();

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int pkt_size = av_frame_get_pkt_size(wrap);
  NanReturnValue(NanNew<Int32>(pkt_size));
}

NAN_SETTER(AVFrame::SetPktSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pkt_size: integer required");

  ::AVFrame* wrap = Unwrap<AVFrame>(args.This())->This();

  if (wrap) {
    int pkt_size = value->Int32Value();
    av_frame_set_pkt_size(wrap, pkt_size);
  }
}

}  // namespace avutil
}  // namespace ffmpeg
