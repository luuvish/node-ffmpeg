#include "avformat/avchapter.h"
#include "avutil/avutil.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

Persistent<FunctionTemplate> AVChapter::constructor;

AVChapter::AVChapter(::AVChapter *ref) : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVChapter *)av_mallocz(sizeof(::AVChapter));
    alloc_ = true;
  }
}

AVChapter::~AVChapter() {
  if (this_ != nullptr && alloc_ == true)
    av_freep(&this_);
}

void AVChapter::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVChapter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("id"), GetId);
  tpl->InstanceTemplate()->SetAccessor(NanNew("time_base"), GetTimeBase);
  tpl->InstanceTemplate()->SetAccessor(NanNew("start"), GetStart);
  tpl->InstanceTemplate()->SetAccessor(NanNew("end"), GetEnd);
  tpl->InstanceTemplate()->SetAccessor(NanNew("metadata"), GetMetadata);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVChapter"), tpl->GetFunction());
}

Local<Object> AVChapter::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVChapter::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVChapter::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVChapter *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVChapter *>(External::Unwrap(args[0]));
    AVChapter *obj = new AVChapter(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_GETTER(AVChapter::GetId) {
  NanEscapableScope();

  ::AVChapter *ref = Unwrap<AVChapter>(args.This())->This();
  int id = ref->id;

  NanReturnValue(NanNew<Integer>(id));
}

NAN_GETTER(AVChapter::GetTimeBase) {
  NanEscapableScope();

  ::AVChapter *ref = Unwrap<AVChapter>(args.This())->This();
  ::AVRational time_base = ref->time_base;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Integer>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Integer>(time_base.den));

  NanReturnValue(ret);
}

NAN_GETTER(AVChapter::GetStart) {
  NanEscapableScope();

  ::AVChapter *ref = Unwrap<AVChapter>(args.This())->This();
  int64_t start = ref->start;

  NanReturnValue(NanNew<Number>(start));
}

NAN_GETTER(AVChapter::GetEnd) {
  NanEscapableScope();

  ::AVChapter *ref = Unwrap<AVChapter>(args.This())->This();
  int64_t end = ref->end;

  NanReturnValue(NanNew<Number>(end));
}

NAN_GETTER(AVChapter::GetMetadata) {
  NanEscapableScope();

  ::AVChapter *ref = Unwrap<AVChapter>(args.This())->This();
  Local<Object> ret = NanNew<Object>();
  ::AVDictionary *metadata = ref->metadata;
  ::AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));

  NanReturnValue(ret);
}

}  // namespace avformat
}  // namespace ffmpeg
