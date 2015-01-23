#include "avformat/avchapter.h"
#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVChapterWrapper::constructor;

FFmpeg::AVFormat::AVChapterWrapper::AVChapterWrapper(::AVChapter *chapter)
  : _this(chapter), _allocated(false) {
  if (!_this) {
    _this = (::AVChapter *)av_mallocz(sizeof(::AVChapter));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVChapterWrapper::~AVChapterWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVChapterWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVChapter"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("start"), GetStart);
  proto->SetAccessor(NanNew<String>("end"), GetEnd);
  proto->SetAccessor(NanNew<String>("metadata"), GetMetadata);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVChapter"), creator);
}

Handle<Value>
FFmpeg::AVFormat::AVChapterWrapper::newInstance(::AVChapter *chapter) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(chapter) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVChapterWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVChapterWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVChapter *chapter = nullptr;
    if (args[0]->IsExternal())
      chapter = static_cast<::AVChapter *>(External::Unwrap(args[0]));
    AVChapterWrapper *obj = new AVChapterWrapper(chapter);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetId) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetTimeBase) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  ::AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetStart) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t start = obj->_this->start;
  NanReturnValue(NanNew<Number>(start));
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetEnd) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  int64_t end = obj->_this->end;
  NanReturnValue(NanNew<Number>(end));
}

NAN_GETTER(FFmpeg::AVFormat::AVChapterWrapper::GetMetadata) {
  NanScope();
  AVChapterWrapper *obj = ObjectWrap::Unwrap<AVChapterWrapper>(args.This());
  Handle<Object> ret = NanNew<Object>();
  AVDictionary *metadata = obj->_this->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}
