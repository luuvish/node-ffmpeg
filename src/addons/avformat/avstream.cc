#include "avformat/avstream.h"
#include "avcodec/avcodeccontext.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVStream::constructor;

void AVStream::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVStream"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "searchIndexTimestamp", SearchIndexTimestamp);
  NODE_SET_PROTOTYPE_METHOD(tpl, "addIndexEntry", AddIndexEntry);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getEndPts", GetEndPts);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("index"), GetIndex, SetIndex);
  inst->SetAccessor(NanNew("id"), GetId, SetId);
  inst->SetAccessor(NanNew("codec"), GetCodec);
  inst->SetAccessor(NanNew("time_base"), GetTimeBase, SetTimeBase);
  inst->SetAccessor(NanNew("start_time"), GetStartTime);
  inst->SetAccessor(NanNew("duration"), GetDuration);
  inst->SetAccessor(NanNew("nb_frames"), GetNbFrames);
  inst->SetAccessor(NanNew("disposition"), GetDisposition, SetDisposition);
  inst->SetAccessor(NanNew("discard"), GetDiscard, SetDiscard);
  inst->SetAccessor(NanNew("sample_aspect_ratio"),
                    GetSampleAspectRatio, SetSampleAspectRatio);
  inst->SetAccessor(NanNew("metadata"), GetMetadata);
  inst->SetAccessor(NanNew("avg_frame_rate"), GetAvgFrameRate, SetAvgFrameRate);
  inst->SetAccessor(NanNew("event_flags"), GetEventFlags);
  inst->SetAccessor(NanNew("pts_wrap_bits"), GetPtsWrapBits);
  inst->SetAccessor(NanNew("first_dts"), GetFirstDts);
  inst->SetAccessor(NanNew("cur_dts"), GetCurDts);
  inst->SetAccessor(NanNew("last_IP_pts"), GetLastIPPts);
  inst->SetAccessor(NanNew("last_IP_duration"), GetLastIPDuration);
  inst->SetAccessor(NanNew("r_frame_rate"), GetRFrameRate, SetRFrameRate);
  inst->SetAccessor(NanNew("recommended_encoder_configuration"),
                    GetRecommendedEncoderConfiguration,
                    SetRecommendedEncoderConfiguration);
  inst->SetAccessor(NanNew("display_aspect_ratio"),
                    GetDisplayAspectRatio, SetDisplayAspectRatio);

  NanAssignPersistent(constructor, tpl);
}

Local<Object> AVStream::NewInstance(::AVStream* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVStream>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVStream::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVStream* AVStream::This(::AVStream* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVStream::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVStream* obj = new AVStream();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVStream::SearchIndexTimestamp) {
  NanScope();

  if (!args[0]->IsNumber())
    NanThrowTypeError("searchIndexTimestamp: timestamp integer required");
  if (!args[1]->IsNumber())
    NanThrowTypeError("searchIndexTimestamp: flags integer required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t timestamp = args[0]->IntegerValue();
  int flags = args[1]->Int32Value();

  int ret = av_index_search_timestamp(wrap, timestamp, flags);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVStream::AddIndexEntry) {
  NanScope();

  if (!args[0]->IsNumber())
    NanThrowTypeError("addIndexEntry: pos integer required");
  if (!args[1]->IsNumber())
    NanThrowTypeError("addIndexEntry: timestamp integer required");
  if (!args[2]->IsNumber())
    NanThrowTypeError("addIndexEntry: size integer required");
  if (!args[3]->IsNumber())
    NanThrowTypeError("addIndexEntry: distance integer required");
  if (!args[4]->IsNumber())
    NanThrowTypeError("addIndexEntry: flags integer required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pos = args[0]->IntegerValue();
  int64_t timestamp = args[1]->IntegerValue();
  int size = args[2]->Int32Value();
  int distance = args[3]->Int32Value();
  int flags = args[4]->Int32Value();

  int ret = av_add_index_entry(wrap, pos, timestamp, size, distance, flags);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVStream::GetEndPts) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t end_pts = av_stream_get_end_pts(wrap);
  NanReturnValue(NanNew<Number>(end_pts));
}

NAN_GETTER(AVStream::GetIndex) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int index = wrap->index;
  NanReturnValue(NanNew<Int32>(index));
}

NAN_SETTER(AVStream::SetIndex) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("index: integer required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap)
    wrap->index = value->Int32Value();
}

NAN_GETTER(AVStream::GetId) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int id = wrap->id;
  NanReturnValue(NanNew<Int32>(id));
}

NAN_SETTER(AVStream::SetId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("id: integer required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap)
    wrap->id = value->Int32Value();
}

NAN_GETTER(AVStream::GetCodec) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  if (wrap->codec)
    NanReturnValue(avcodec::AVCodecContext::NewInstance(wrap->codec));
  else
    NanReturnNull();
}

NAN_GETTER(AVStream::GetTimeBase) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational time_base = wrap->time_base;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(time_base.num));
  ret->Set(NanNew("den"), NanNew<Int32>(time_base.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVStream::SetTimeBase) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("time_base: AVRational required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    Local<Object> tb = value->ToObject();
    wrap->time_base.num = tb->Get(NanNew("num"))->Int32Value();
    wrap->time_base.den = tb->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVStream::GetStartTime) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t start_time = wrap->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(AVStream::GetDuration) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t duration = wrap->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(AVStream::GetNbFrames) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t nb_frames = wrap->nb_frames;
  NanReturnValue(NanNew<Number>(nb_frames));
}

NAN_GETTER(AVStream::GetDisposition) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int disposition = wrap->disposition;
  NanReturnValue(NanNew<Int32>(disposition));
}

NAN_SETTER(AVStream::SetDisposition) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("disposition: integer required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap)
    wrap->disposition = value->Int32Value();
}

NAN_GETTER(AVStream::GetDiscard) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVDiscard discard = wrap->discard;
  NanReturnValue(NanNew<Int32>(discard));
}

NAN_SETTER(AVStream::SetDiscard) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("discard: enum required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap)
    wrap->discard = static_cast<enum ::AVDiscard>(value->Int32Value());
}

NAN_GETTER(AVStream::GetSampleAspectRatio) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational sar = wrap->sample_aspect_ratio;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(sar.num));
  ret->Set(NanNew("den"), NanNew<Int32>(sar.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVStream::SetSampleAspectRatio) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("sample_aspect_ratio: AVRational required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    Local<Object> sar = value->ToObject();
    wrap->sample_aspect_ratio.num = sar->Get(NanNew("num"))->Int32Value();
    wrap->sample_aspect_ratio.den = sar->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVStream::GetMetadata) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = NanNew<Object>();
  AVDictionary* metadata = wrap->metadata;
  AVDictionaryEntry* t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_GETTER(AVStream::GetAvgFrameRate) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational afr = wrap->avg_frame_rate;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(afr.num));
  ret->Set(NanNew("den"), NanNew<Int32>(afr.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVStream::SetAvgFrameRate) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("avg_frame_rate: AVRational required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    Local<Object> afr = value->ToObject();
    wrap->avg_frame_rate.num = afr->Get(NanNew("num"))->Int32Value();
    wrap->avg_frame_rate.den = afr->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVStream::GetEventFlags) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int event_flags = wrap->event_flags;
  NanReturnValue(NanNew<Int32>(event_flags));
}

NAN_GETTER(AVStream::GetPtsWrapBits) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int pts_wrap_bits = wrap->pts_wrap_bits;
  NanReturnValue(NanNew<Int32>(pts_wrap_bits));
}

NAN_GETTER(AVStream::GetFirstDts) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t first_dts = wrap->first_dts;
  NanReturnValue(NanNew<Number>(first_dts));
}

NAN_GETTER(AVStream::GetCurDts) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t cur_dts = wrap->cur_dts;
  NanReturnValue(NanNew<Number>(cur_dts));
}

NAN_GETTER(AVStream::GetLastIPPts) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t last_IP_pts = wrap->last_IP_pts;
  NanReturnValue(NanNew<Number>(last_IP_pts));
}

NAN_GETTER(AVStream::GetLastIPDuration) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int last_IP_duration = wrap->last_IP_duration;
  NanReturnValue(NanNew<Int32>(last_IP_duration));
}

NAN_GETTER(AVStream::GetRFrameRate) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational r_frame_rate = av_stream_get_r_frame_rate(wrap);
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(r_frame_rate.num));
  ret->Set(NanNew("den"), NanNew<Int32>(r_frame_rate.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVStream::SetRFrameRate) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("r_frame_rate: AVRational required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    Local<Object> dar = value->ToObject();
    ::AVRational r = {
      .num = dar->Get(NanNew("num"))->Int32Value(),
      .den = dar->Get(NanNew("den"))->Int32Value()
    };
    av_stream_set_r_frame_rate(wrap, r);
  }
}

NAN_GETTER(AVStream::GetRecommendedEncoderConfiguration) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* configuration = av_stream_get_recommended_encoder_configuration(wrap);
  if (configuration)
    NanReturnValue(NanNew<String>(configuration));
  else
    NanReturnEmptyString();
}

NAN_SETTER(AVStream::SetRecommendedEncoderConfiguration) {
  NanScope();

  if (!value->IsString())
    NanThrowTypeError("recommended_encoder_configuration: string required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    NanUtf8String recommended_encoder_configuration(value);
    char* configuration = av_strdup(*recommended_encoder_configuration);
    av_freep(&wrap->recommended_encoder_configuration);
    av_stream_set_recommended_encoder_configuration(wrap, configuration);
  }
}

NAN_GETTER(AVStream::GetDisplayAspectRatio) {
  NanScope();

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational display_aspect_ratio = wrap->display_aspect_ratio;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(display_aspect_ratio.num));
  ret->Set(NanNew("den"), NanNew<Int32>(display_aspect_ratio.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVStream::SetDisplayAspectRatio) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("display_aspect_ratio: AVRational required");

  ::AVStream* wrap = Unwrap<AVStream>(args.This())->This();

  if (wrap) {
    Local<Object> dar = value->ToObject();
    wrap->display_aspect_ratio.num = dar->Get(NanNew("num"))->Int32Value();
    wrap->display_aspect_ratio.den = dar->Get(NanNew("den"))->Int32Value();
  }
}

}  // namespace avformat
}  // namespace ffmpeg
