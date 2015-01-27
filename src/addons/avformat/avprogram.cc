#include "avformat/avprogram.h"
#include "avutil/avutil.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVProgram::constructor;

AVProgram::AVProgram(::AVProgram *ref) : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVProgram *)av_mallocz(sizeof(::AVProgram));
    alloc_ = true;
  }
}

AVProgram::~AVProgram() {
  if (this_ != nullptr && alloc_ == true)
    av_freep(&this_);
}

void AVProgram::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVProgram"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("id"), GetId);
  tpl->InstanceTemplate()->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  tpl->InstanceTemplate()->SetAccessor(NanNew("discard"),
                                       GetDiscard, SetDiscard);
  tpl->InstanceTemplate()->SetAccessor(NanNew("stream_indexes"),
                                       GetStreamIndexes);
  tpl->InstanceTemplate()->SetAccessor(NanNew("metadata"), GetMetadata);
  tpl->InstanceTemplate()->SetAccessor(NanNew("program_num"),
                                       GetProgramNum, SetProgramNum);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pmt_pid"), GetPmtPid, SetPmtPid);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pcr_pid"), GetPcrPid, SetPcrPid);
  tpl->InstanceTemplate()->SetAccessor(NanNew("start_time"), GetStartTime);
  tpl->InstanceTemplate()->SetAccessor(NanNew("end_time"), GetEndTime);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pts_wrap_reference"),
                                       GetPtsWrapReference);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pts_wrap_behavior"),
                                       GetPtsWrapBehavior);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVProgram"), tpl->GetFunction());
}

Local<Object> AVProgram::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVProgram::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVProgram::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVProgram *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVProgram *>(External::Unwrap(args[0]));
    AVProgram *obj = new AVProgram(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_GETTER(AVProgram::GetId) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int id = ref->id;

  NanReturnValue(NanNew<Integer>(id));
}

NAN_GETTER(AVProgram::GetFlags) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int flags = ref->flags;

  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(AVProgram::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  ref->flags = value->Int32Value();
}

NAN_GETTER(AVProgram::GetDiscard) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  enum ::AVDiscard discard = ref->discard;

  NanReturnValue(NanNew<Integer>(discard));
}

NAN_SETTER(AVProgram::SetDiscard) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("discard: enum required");

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  ref->discard = static_cast<enum ::AVDiscard>(value->Uint32Value());
}

NAN_GETTER(AVProgram::GetStreamIndexes) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  Local<Array> ret = NanNew<Array>(ref->nb_stream_indexes);
  for (uint32_t i = 0; i < ref->nb_stream_indexes; i++)
    ret->Set(i, NanNew<Integer>(ref->stream_index[i]));

  NanReturnValue(ret);
}

NAN_GETTER(AVProgram::GetMetadata) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  Local<Object> ret = NanNew<Object>();
  AVDictionary *metadata = ref->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));

  NanReturnValue(ret);
}

NAN_GETTER(AVProgram::GetProgramNum) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int program_num = ref->program_num;

  NanReturnValue(NanNew<Integer>(program_num));
}

NAN_SETTER(AVProgram::SetProgramNum) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("program_num: integer required");

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  ref->program_num = value->Int32Value();
}

NAN_GETTER(AVProgram::GetPmtPid) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int pmt_pid = ref->pmt_pid;

  NanReturnValue(NanNew<Integer>(pmt_pid));
}

NAN_SETTER(AVProgram::SetPmtPid) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pmt_pid: integer required");

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  ref->pmt_pid = value->Int32Value();
}

NAN_GETTER(AVProgram::GetPcrPid) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int pcr_pid = ref->pcr_pid;

  NanReturnValue(NanNew<Integer>(pcr_pid));
}

NAN_SETTER(AVProgram::SetPcrPid) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pcr_pid: integer required");

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  ref->pcr_pid = value->Int32Value();
}

NAN_GETTER(AVProgram::GetStartTime) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int64_t start_time = ref->start_time;

  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(AVProgram::GetEndTime) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int64_t end_time = ref->end_time;

  NanReturnValue(NanNew<Number>(end_time));
}

NAN_GETTER(AVProgram::GetPtsWrapReference) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int64_t pts_wrap_reference = ref->pts_wrap_reference;

  NanReturnValue(NanNew<Number>(pts_wrap_reference));
}

NAN_GETTER(AVProgram::GetPtsWrapBehavior) {
  NanEscapableScope();

  ::AVProgram *ref = Unwrap<AVProgram>(args.This())->This();
  int pts_wrap_behavior = ref->pts_wrap_behavior;

  NanReturnValue(NanNew<Integer>(pts_wrap_behavior));
}

}  // namespace avformat
}  // namespace ffmpeg
