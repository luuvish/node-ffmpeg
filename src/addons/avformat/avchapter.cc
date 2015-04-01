#include "avformat/avchapter.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVChapter::constructor;

void AVChapter::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVChapter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("id"), GetId);
  inst->SetAccessor(NanNew("time_base"), GetTimeBase);
  inst->SetAccessor(NanNew("start"), GetStart);
  inst->SetAccessor(NanNew("end"), GetEnd);
  inst->SetAccessor(NanNew("metadata"), GetMetadata);

  NanAssignPersistent(constructor, tpl);
}

Local<Object> AVChapter::NewInstance(::AVChapter* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVChapter>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVChapter::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVChapter* AVChapter::This(::AVChapter* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVChapter::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVChapter* obj = new AVChapter();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_GETTER(AVChapter::GetId) {
  NanScope();

  ::AVChapter* wrap = Unwrap<AVChapter>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int id = wrap->id;
  NanReturnValue(NanNew<Int32>(id));
}

NAN_GETTER(AVChapter::GetTimeBase) {
  NanScope();

  ::AVChapter* wrap = Unwrap<AVChapter>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational time_base = wrap->time_base;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(time_base.num));
  ret->Set(NanNew("den"), NanNew<Int32>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(AVChapter::GetStart) {
  NanScope();

  ::AVChapter* wrap = Unwrap<AVChapter>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t start = wrap->start;
  NanReturnValue(NanNew<Number>(start));
}

NAN_GETTER(AVChapter::GetEnd) {
  NanScope();

  ::AVChapter* wrap = Unwrap<AVChapter>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t end = wrap->end;
  NanReturnValue(NanNew<Number>(end));
}

NAN_GETTER(AVChapter::GetMetadata) {
  NanScope();

  ::AVChapter* wrap = Unwrap<AVChapter>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = NanNew<Object>();
  ::AVDictionary* metadata = wrap->metadata;
  ::AVDictionaryEntry* t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

}  // namespace avformat
}  // namespace ffmpeg
