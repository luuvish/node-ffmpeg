#include "avformat/avprogram.h"
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


Persistent<FunctionTemplate> FFmpeg::AVFormat::AVProgramWrapper::constructor;

FFmpeg::AVFormat::AVProgramWrapper::AVProgramWrapper(::AVProgram *program)
  : _this(program), _allocated(false) {
  if (!_this) {
    _this = (::AVProgram *)av_mallocz(sizeof(::AVProgram));
    _allocated = true;
  }
}

FFmpeg::AVFormat::AVProgramWrapper::~AVProgramWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVFormat::AVProgramWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVProgram"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("discard"), GetDiscard);
  proto->SetAccessor(NanNew<String>("stream_indexes"), GetStreamIndexes);
  proto->SetAccessor(NanNew<String>("metadata"), GetMetadata);
  proto->SetAccessor(NanNew<String>("program_num"), GetProgramNum);
  proto->SetAccessor(NanNew<String>("start_time"), GetStartTime);
  proto->SetAccessor(NanNew<String>("end_time"), GetEndTime);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVProgram"), creator);
}

Handle<Value>
FFmpeg::AVFormat::AVProgramWrapper::newInstance(::AVProgram *program) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(program) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVFormat::AVProgramWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVFormat::AVProgramWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVProgram *program = nullptr;
    if (args[0]->IsExternal())
      program = static_cast<::AVProgram *>(External::Unwrap(args[0]));
    AVProgramWrapper *obj = new AVProgramWrapper(program);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetId) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetDiscard) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  enum ::AVDiscard discard = obj->_this->discard;
  NanReturnValue(NanNew<Number>(discard));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetStreamIndexes) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  Handle<Array> stream_indexes = NanNew<Array>(obj->_this->nb_stream_indexes);
  for (unsigned int i = 0; i < obj->_this->nb_stream_indexes; i++)
    stream_indexes->Set(i, NanNew<Number>(obj->_this->stream_index[i]));
  NanReturnValue(stream_indexes);
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetMetadata) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  Handle<Object> ret = NanNew<Object>();
  AVDictionary *metadata = obj->_this->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetProgramNum) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int program_num = obj->_this->program_num;
  NanReturnValue(NanNew<Number>(program_num));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetStartTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t start_time = obj->_this->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(FFmpeg::AVFormat::AVProgramWrapper::GetEndTime) {
  NanScope();
  AVProgramWrapper *obj = ObjectWrap::Unwrap<AVProgramWrapper>(args.This());
  int64_t end_time = obj->_this->end_time;
  NanReturnValue(NanNew<Number>(end_time));
}
