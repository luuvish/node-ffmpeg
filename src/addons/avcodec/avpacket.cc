#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVPacket::constructor;

void read_pointer_cb(char *data, void *hint) {}

AVPacket::AVPacket(::AVPacket *ref) : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = (::AVPacket *)av_mallocz(sizeof(::AVPacket));
    alloc_ = true;
  }
}

AVPacket::~AVPacket() {
  if (this_ != nullptr && alloc_ == true) {
    av_free_packet(this_);
    av_freep(&this_);
  }
}

void AVPacket::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVPacket"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("pts"), GetPts);
  tpl->InstanceTemplate()->SetAccessor(NanNew("dts"), GetDts);
  tpl->InstanceTemplate()->SetAccessor(NanNew("data"), GetData);
  tpl->InstanceTemplate()->SetAccessor(NanNew("size"), GetSize);
  tpl->InstanceTemplate()->SetAccessor(NanNew("stream_index"), GetStreamIndex);
  tpl->InstanceTemplate()->SetAccessor(NanNew("flgas"), GetFlags);
  tpl->InstanceTemplate()->SetAccessor(NanNew("duration"), GetDuration);
  tpl->InstanceTemplate()->SetAccessor(NanNew("pos"), GetPos);
  tpl->InstanceTemplate()->SetAccessor(NanNew("convergence_duration"),
                                       GetConvergenceDuration);

  NODE_SET_PROTOTYPE_METHOD(tpl, "init", Init);
  NODE_SET_PROTOTYPE_METHOD(tpl, "free", Free);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVPacket"), tpl->GetFunction());
}

Local<Object> AVPacket::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVPacket::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVPacket::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVPacket *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVPacket *>(External::Unwrap(args[0]));
    AVPacket *obj = new AVPacket(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVPacket::Init) {
  NanScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  av_init_packet(ref);

  NanReturnUndefined();
}

NAN_METHOD(AVPacket::Free) {
  NanScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  av_free_packet(ref);

  NanReturnUndefined();
}

NAN_GETTER(AVPacket::GetPts) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int64_t pts = ref->pts;

  NanReturnValue(NanNew<Number>(pts));
}

NAN_GETTER(AVPacket::GetDts) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int64_t dts = ref->dts;

  NanReturnValue(NanNew<Number>(dts));
}

NAN_GETTER(AVPacket::GetData) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  Local<Object> ret = NanNewBufferHandle(reinterpret_cast<char *>(ref->data),
                                         ref->size,
                                         read_pointer_cb, nullptr);

  NanReturnValue(ret);
}

NAN_GETTER(AVPacket::GetSize) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int size = ref->size;

  NanReturnValue(NanNew<Integer>(size));
}

NAN_GETTER(AVPacket::GetStreamIndex) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int stream_index = ref->stream_index;

  NanReturnValue(NanNew<Integer>(stream_index));
}

NAN_GETTER(AVPacket::GetFlags) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int flags = ref->flags;

  NanReturnValue(NanNew<Integer>(flags));
}

NAN_GETTER(AVPacket::GetDuration) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int duration = ref->duration;

  NanReturnValue(NanNew<Integer>(duration));
}

NAN_GETTER(AVPacket::GetPos) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int64_t pos = ref->pos;

  NanReturnValue(NanNew<Number>(pos));
}

NAN_GETTER(AVPacket::GetConvergenceDuration) {
  NanEscapableScope();

  ::AVPacket *ref = Unwrap<AVPacket>(args.This())->This();
  int64_t convergence_duration = ref->convergence_duration;

  NanReturnValue(NanNew<Number>(convergence_duration));
}

}  // namespace avcodec
}  // namespace ffmpeg
