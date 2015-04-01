#include "avcodec/avpacket.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVPacket::constructor;

void AVPacket::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVPacket"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "dup", Dup);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", Copy);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copySideData", CopySideData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "newSideData", NewSideData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getSideData", GetSideData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "mergeSideData", MergeSideData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "splitSideData", SplitSideData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "rescaleTs", RescaleTs);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("pts"), GetPts, SetPts);
  inst->SetAccessor(NanNew("dts"), GetDts, SetDts);
  inst->SetAccessor(NanNew("data"), GetData);
  inst->SetAccessor(NanNew("size"), GetSize);
  inst->SetAccessor(NanNew("stream_index"), GetStreamIndex, SetStreamIndex);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("duration"), GetDuration, SetDuration);
  inst->SetAccessor(NanNew("pos"), GetPos, SetPos);
  inst->SetAccessor(NanNew("convergence_duration"),
                    GetConvergenceDuration, SetConvergenceDuration);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVPacket"), tpl->GetFunction());
}

Local<Object> AVPacket::NewInstance(::AVPacket* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVPacket>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVPacket::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVPacket* AVPacket::This(::AVPacket* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

AVPacket::AVPacket() : this_(nullptr) {
  this_ = (::AVPacket*)av_mallocz(sizeof(::AVPacket));
  if (this_ == nullptr)
    NanThrowTypeError("AVPacket: cannot allocation");
  av_init_packet(this_);
}

AVPacket::~AVPacket() {
  if (this_ != nullptr) {
    av_free_packet(this_);
    av_freep(&this_);
  }
}

NAN_METHOD(AVPacket::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVPacket* obj = new AVPacket();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVPacket::Dup) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_dup_packet(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPacket::Copy) {
  NanScope();

  if (!AVPacket::HasInstance(args[0]))
    NanThrowTypeError("copy: AVPacket instance required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* src = Unwrap<AVPacket>(args[0]->ToObject())->This();
  int ret = av_copy_packet(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPacket::CopySideData) {
  NanScope();

  if (!AVPacket::HasInstance(args[0]))
    NanThrowTypeError("copySideData: AVPacket instance required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* src = Unwrap<AVPacket>(args[0]->ToObject())->This();
  int ret = av_copy_packet_side_data(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPacket::NewSideData) {
  NanScope();

  if (!args[0]->IsNumber())
    NanThrowTypeError("getSideData: AVPacketSideDataType enum required");
  if (!args[1]->IsNumber())
    NanThrowTypeError("getSideData: size integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVPacketSideDataType type =
    static_cast<enum ::AVPacketSideDataType>(args[0]->Uint32Value());
  int size = args[0]->Int32Value();
  uint8_t* data = av_packet_new_side_data(wrap, type, size);

  if (data != nullptr && size > 0) {
    Local<Object> ret = NanNew<Object>();
    ExternalArrayType type = kExternalUint8Array;
    ret->SetIndexedPropertiesToExternalArrayData(data, type, size);
    NanReturnValue(ret);
  } else
    NanReturnNull();
}

NAN_METHOD(AVPacket::GetSideData) {
  NanScope();

  if (!args[0]->IsNumber())
    NanThrowTypeError("getSideData: AVPacketSideDataType enum required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVPacketSideDataType type =
    static_cast<enum ::AVPacketSideDataType>(args[0]->Uint32Value());
  int size;
  uint8_t* data = av_packet_get_side_data(wrap, type, &size);

  if (data != nullptr && size > 0) {
    Local<Object> ret = NanNew<Object>();
    ExternalArrayType type = kExternalUint8Array;
    ret->SetIndexedPropertiesToExternalArrayData(data, type, size);
    NanReturnValue(ret);
  } else
    NanReturnNull();
}

NAN_METHOD(AVPacket::MergeSideData) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_packet_merge_side_data(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPacket::SplitSideData) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_packet_split_side_data(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPacket::RescaleTs) {
  NanScope();

  if (!args[0]->IsObject() ||
      !args[0]->ToObject()->HasOwnProperty(NanNew("num")) ||
      !args[0]->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("rescaleTs: tb_src AVRational instance required");
  if (!args[1]->IsObject() ||
      !args[1]->ToObject()->HasOwnProperty(NanNew("num")) ||
      !args[1]->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("rescaleTs: tb_dst AVRational instance required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational tb_src = {
    .num = args[0]->ToObject()->Get(NanNew("num"))->Int32Value(),
    .den = args[0]->ToObject()->Get(NanNew("den"))->Int32Value()
  };
  ::AVRational tb_dst = {
    .num = args[1]->ToObject()->Get(NanNew("num"))->Int32Value(),
    .den = args[1]->ToObject()->Get(NanNew("den"))->Int32Value()
  };
  av_packet_rescale_ts(wrap, tb_src, tb_dst);
  NanReturnUndefined();
}

NAN_GETTER(AVPacket::GetPts) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts = wrap->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_SETTER(AVPacket::SetPts) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pts: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->pts = value->IntegerValue();
}

NAN_GETTER(AVPacket::GetDts) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t dts = wrap->dts;
  NanReturnValue(NanNew<Number>(dts));
}

NAN_SETTER(AVPacket::SetDts) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("dts: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->dts = value->IntegerValue();
}

NAN_GETTER(AVPacket::GetData) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ExternalArrayType type = kExternalUint8Array;
  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(wrap->data, type, wrap->size);
  NanReturnValue(ret);
}

NAN_GETTER(AVPacket::GetSize) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int size = wrap->size;
  NanReturnValue(NanNew<Int32>(size));
}

NAN_GETTER(AVPacket::GetStreamIndex) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = wrap->stream_index;
  NanReturnValue(NanNew<Int32>(stream_index));
}

NAN_SETTER(AVPacket::SetStreamIndex) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("stream_index: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->stream_index = value->Int32Value();
}

NAN_GETTER(AVPacket::GetFlags) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVPacket::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVPacket::GetDuration) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int duration = wrap->duration;
  NanReturnValue(NanNew<Int32>(duration));
}

NAN_SETTER(AVPacket::SetDuration) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("duration: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->duration = value->Int32Value();
}

NAN_GETTER(AVPacket::GetPos) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pos = wrap->pos;
  NanReturnValue(NanNew<Number>(pos));
}

NAN_SETTER(AVPacket::SetPos) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pos: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->pos = value->IntegerValue();
}

NAN_GETTER(AVPacket::GetConvergenceDuration) {
  NanScope();

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t convergence_duration = wrap->convergence_duration;
  NanReturnValue(NanNew<Number>(convergence_duration));
}

NAN_SETTER(AVPacket::SetConvergenceDuration) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("convergence_duration: integer required");

  ::AVPacket* wrap = Unwrap<AVPacket>(args.This())->This();

  if (wrap)
    wrap->convergence_duration = value->IntegerValue();
}

}  // namespace avcodec
}  // namespace ffmpeg
