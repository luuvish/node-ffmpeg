#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


Persistent<FunctionTemplate> FFmpeg::AVCodec::AVPacketWrapper::constructor;

FFmpeg::AVCodec::AVPacketWrapper::AVPacketWrapper(::AVPacket *packet)
  : _this(packet), _allocated(false) {
  if (!_this) {
    _this = (::AVPacket *)av_mallocz(sizeof(::AVPacket));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVPacketWrapper::~AVPacketWrapper() {
  if (_this && _allocated) {
    av_free_packet(_this);
    av_freep(&_this);
  }
}

void FFmpeg::AVCodec::AVPacketWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVPacket"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "init", Init);
  NODE_SET_PROTOTYPE_METHOD(ctor, "free", Free);
  proto->SetAccessor(NanNew<String>("pts"), GetPts);
  proto->SetAccessor(NanNew<String>("dts"), GetDts);
  proto->SetAccessor(NanNew<String>("size"), GetSize);
  proto->SetAccessor(NanNew<String>("stream_index"), GetStreamIndex);
  proto->SetAccessor(NanNew<String>("duration"), GetDuration);
  proto->SetAccessor(NanNew<String>("pos"), GetPos);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVPacket"), creator);
}

Handle<Value>
FFmpeg::AVCodec::AVPacketWrapper::newInstance(::AVPacket *packet) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(packet) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVPacketWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVPacketWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVPacket *packet = nullptr;
    if (args[0]->IsExternal())
      packet = static_cast<::AVPacket *>(External::Unwrap(args[0]));
    AVPacketWrapper *obj = new AVPacketWrapper(packet);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodec::AVPacketWrapper::Init) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  av_init_packet(obj->_this);
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodec::AVPacketWrapper::Free) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  av_free_packet(obj->_this);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetPts) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t pts = obj->_this->pts;
  NanReturnValue(NanNew<Number>(pts));
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetDts) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t dts = obj->_this->dts;
  NanReturnValue(NanNew<Number>(dts));
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetSize) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int size = obj->_this->size;
  NanReturnValue(NanNew<Number>(size));
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetStreamIndex) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int stream_index = obj->_this->stream_index;
  NanReturnValue(NanNew<Number>(stream_index));
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetDuration) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int duration = obj->_this->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(FFmpeg::AVCodec::AVPacketWrapper::GetPos) {
  NanScope();
  AVPacketWrapper *obj = ObjectWrap::Unwrap<AVPacketWrapper>(args.This());
  int64_t pos = obj->_this->pos;
  NanReturnValue(NanNew<Number>(pos));
}
