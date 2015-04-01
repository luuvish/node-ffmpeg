#include "avformat/avprogram.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVProgram::constructor;

void AVProgram::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVProgram"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("id"), GetId);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("discard"), GetDiscard, SetDiscard);
  inst->SetAccessor(NanNew("stream_indexes"), GetStreamIndexes);
  inst->SetAccessor(NanNew("metadata"), GetMetadata);
  inst->SetAccessor(NanNew("program_num"), GetProgramNum, SetProgramNum);
  inst->SetAccessor(NanNew("pmt_pid"), GetPmtPid, SetPmtPid);
  inst->SetAccessor(NanNew("pcr_pid"), GetPcrPid, SetPcrPid);
  inst->SetAccessor(NanNew("start_time"), GetStartTime);
  inst->SetAccessor(NanNew("end_time"), GetEndTime);
  inst->SetAccessor(NanNew("pts_wrap_reference"), GetPtsWrapReference);
  inst->SetAccessor(NanNew("pts_wrap_behavior"), GetPtsWrapBehavior);

  NanAssignPersistent(constructor, tpl);
}

Local<Object> AVProgram::NewInstance(::AVProgram* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVProgram>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVProgram::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVProgram* AVProgram::This(::AVProgram* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVProgram::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVProgram* obj = new AVProgram();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(AVProgram::GetId) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int id = wrap->id;
  NanReturnValue(NanNew<Int32>(id));
}

NAN_GETTER(AVProgram::GetFlags) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVProgram::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVProgram::GetDiscard) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVDiscard discard = wrap->discard;
  NanReturnValue(NanNew<Int32>(discard));
}

NAN_SETTER(AVProgram::SetDiscard) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("discard: enum required");

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();

  if (wrap)
    wrap->discard = static_cast<enum ::AVDiscard>(value->Int32Value());
}

NAN_GETTER(AVProgram::GetStreamIndexes) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> ret = NanNew<Array>(wrap->nb_stream_indexes);
  for (uint32_t i = 0; i < wrap->nb_stream_indexes; i++)
    ret->Set(i, NanNew<Int32>(wrap->stream_index[i]));
  NanReturnValue(ret);
}

NAN_GETTER(AVProgram::GetMetadata) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = NanNew<Object>();
  AVDictionary* metadata = wrap->metadata;
  AVDictionaryEntry* t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_GETTER(AVProgram::GetProgramNum) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int program_num = wrap->program_num;
  NanReturnValue(NanNew<Int32>(program_num));
}

NAN_SETTER(AVProgram::SetProgramNum) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("program_num: integer required");

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();

  if (wrap)
    wrap->program_num = value->Int32Value();
}

NAN_GETTER(AVProgram::GetPmtPid) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int pmt_pid = wrap->pmt_pid;
  NanReturnValue(NanNew<Int32>(pmt_pid));
}

NAN_SETTER(AVProgram::SetPmtPid) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pmt_pid: integer required");

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();

  if (wrap)
    wrap->pmt_pid = value->Int32Value();
}

NAN_GETTER(AVProgram::GetPcrPid) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int pcr_pid = wrap->pcr_pid;
  NanReturnValue(NanNew<Int32>(pcr_pid));
}

NAN_SETTER(AVProgram::SetPcrPid) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pcr_pid: integer required");

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();

  if (wrap)
    wrap->pcr_pid = value->Int32Value();
}

NAN_GETTER(AVProgram::GetStartTime) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t start_time = wrap->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(AVProgram::GetEndTime) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t end_time = wrap->end_time;
  NanReturnValue(NanNew<Number>(end_time));
}

NAN_GETTER(AVProgram::GetPtsWrapReference) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts_wrap_reference = wrap->pts_wrap_reference;
  NanReturnValue(NanNew<Number>(pts_wrap_reference));
}

NAN_GETTER(AVProgram::GetPtsWrapBehavior) {
  NanScope();

  ::AVProgram* wrap = Unwrap<AVProgram>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int pts_wrap_behavior = wrap->pts_wrap_behavior;
  NanReturnValue(NanNew<Int32>(pts_wrap_behavior));
}

}  // namespace avformat
}  // namespace ffmpeg
