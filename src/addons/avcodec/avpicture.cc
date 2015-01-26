#include "avutil/avutil.h"
#include "avcodec/avpicture.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVPicture::constructor;

AVPicture::AVPicture(::AVPicture *ref, int w, int h)
  : this_(ref), alloc_(false), w_(w), h_(h) {
  if (this_ == nullptr) {
    this_ = (::AVPicture *)av_mallocz(sizeof(::AVPicture));
    alloc_ = true;
  }
}

AVPicture::~AVPicture() {
  if (this_ != nullptr && alloc_ == true) {
    av_freep(&this_);
  }
}

void AVPicture::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVPicture"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(NanNew("data"), GetData);
  tpl->InstanceTemplate()->SetAccessor(NanNew("linesize"), GetLinesize);

  NODE_SET_PROTOTYPE_METHOD(tpl, "alloc", Alloc);
  NODE_SET_PROTOTYPE_METHOD(tpl, "free", Free);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fill", Fill);
  NODE_SET_PROTOTYPE_METHOD(tpl, "layout", Layout);

  NODE_SET_METHOD(tpl->GetFunction(), "getSize", GetSize);
  NODE_SET_METHOD(tpl->GetFunction(), "copy", Copy);
  NODE_SET_METHOD(tpl->GetFunction(), "crop", Crop);
  NODE_SET_METHOD(tpl->GetFunction(), "pad", Pad);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVPicture"), tpl->GetFunction());
}

Local<Object> AVPicture::NewInstance(Local<Value> arg[3]) {
  NanEscapableScope();

  const int argc = 3;
  Local<Value> argv[argc] = { arg[0], arg[1], arg[2] };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVPicture::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVPicture::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVPicture *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVPicture *>(External::Unwrap(args[0]));
    int w = args[1]->IsNumber() ? args[1]->Int32Value() : 0;
    int h = args[2]->IsNumber() ? args[2]->Int32Value() : 0;
    AVPicture *obj = new AVPicture(ref, w, h);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 3;
    Local<Value> argv[argc] = { args[0], args[1], args[2] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVPicture::Alloc) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("height required");

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Uint32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();

  int ret = avpicture_alloc(ref, pix_fmt, width, height);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVPicture::Free) {
  NanScope();

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();
  avpicture_free(ref);

  NanReturnUndefined();
}

NAN_METHOD(AVPicture::Fill) {
  NanScope();

  int argc = args[0]->IsArray() ? 1 : 0;

  if (!args[0]->IsNumber() && !args[0]->IsArray())
    return NanThrowTypeError("ptr required");
  if (!args[argc + 0]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[argc + 1]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[argc + 2]->IsNumber())
    return NanThrowTypeError("height required");

  const uint8_t *ptr = nullptr;
  if (argc == 1) {
    char *data = node::Buffer::Data(args[0]->ToObject());
    ptr = reinterpret_cast<const uint8_t *>(data);
  }

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[argc + 0]->Uint32Value());
  int width = args[argc + 1]->Int32Value();
  int height = args[argc + 2]->Int32Value();

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();

  int ret = avpicture_fill(ref, ptr, pix_fmt, width, height);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVPicture::Layout) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("height required");
  if (!args[3]->IsArray())
    return NanThrowTypeError("dest required");

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Uint32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  Local<Object> buf = args[3]->ToObject();
  uint8_t *dest = reinterpret_cast<uint8_t *>(node::Buffer::Data(buf));
  int dest_size = static_cast<int>(node::Buffer::Length(buf));

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();

  int ret = avpicture_layout(ref, pix_fmt, width, height, dest, dest_size);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVPicture::GetSize) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("height required");

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Uint32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();

  int ret = avpicture_get_size(pix_fmt, width, height);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVPicture::Copy) {
  NanScope();

  if (!args[0]->IsObject() || !HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("dst required");
  if (!args[1]->IsObject() || !HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("src required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("height required");

  ::AVPicture *dst = ObjectWrap::Unwrap<AVPicture>(args[0]->ToObject())->This();
  ::AVPicture *src = ObjectWrap::Unwrap<AVPicture>(args[1]->ToObject())->This();
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[2]->Uint32Value());
  int width = args[3]->Int32Value();
  int height = args[4]->Int32Value();

  av_picture_copy(dst, src, pix_fmt, width, height);

  NanReturnUndefined();
}

NAN_METHOD(AVPicture::Crop) {
  NanScope();

  if (!args[0]->IsObject() || !HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("dst required");
  if (!args[1]->IsObject() || !HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("src required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("height required");

  ::AVPicture *dst = ObjectWrap::Unwrap<AVPicture>(args[0]->ToObject())->This();
  ::AVPicture *src = ObjectWrap::Unwrap<AVPicture>(args[1]->ToObject())->This();
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[2]->Uint32Value());
  int top_band = args[3]->Int32Value();
  int left_band = args[4]->Int32Value();

  int ret = av_picture_crop(dst, src, pix_fmt, top_band, left_band);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVPicture::Pad) {
  NanScope();

  if (!args[0]->IsObject() || !HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("dst required");
  if (!args[1]->IsObject() || !HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("src required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("height required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("width required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("pix_fmt required");
  if (!args[5]->IsNumber())
    return NanThrowTypeError("padtop required");
  if (!args[6]->IsNumber())
    return NanThrowTypeError("padbottom required");
  if (!args[7]->IsNumber())
    return NanThrowTypeError("padleft required");
  if (!args[8]->IsNumber())
    return NanThrowTypeError("padright required");
  if (!args[9]->IsArray())
    return NanThrowTypeError("color required");

  ::AVPicture *dst = ObjectWrap::Unwrap<AVPicture>(args[0]->ToObject())->This();
  ::AVPicture *src = ObjectWrap::Unwrap<AVPicture>(args[1]->ToObject())->This();
  int height = args[2]->Int32Value();
  int width = args[3]->Int32Value();
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[4]->Uint32Value());
  int padtop = args[5]->Int32Value();
  int padbottom = args[6]->Int32Value();
  int padleft = args[7]->Int32Value();
  int padright = args[8]->Int32Value();
  int color[4] = {0, };
  Local<Array> arr = args[9].As<Array>();
  for (uint32_t i = 0; i < 4 && i < arr->Length(); i++) {
    color[i] = arr->Get(i)->Uint32Value();
  }

  int ret = av_picture_pad(dst, src, height, width, pix_fmt,
                           padtop, padbottom, padleft, padright, color);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_PROPERTY_GETTER(AVPicture::GetData) {
  NanScope();

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();

  uint32_t size = obj->w_ * obj->h_;
  Local<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    uint8_t *data = ref->data[i];
    if (data)
      //ret->Set(i, NanBufferUse(reinterpret_cast<char *>(data), size));
      ret->Set(i, NanNewBufferHandle(reinterpret_cast<char *>(data), size));
    else
      ret->Set(i, NanNull());
  }

  NanReturnValue(ret);
}

NAN_PROPERTY_GETTER(AVPicture::GetLinesize) {
  NanScope();

  AVPicture *obj = ObjectWrap::Unwrap<AVPicture>(args.This());
  ::AVPicture *ref = obj->This();

  Local<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    int linesize = ref->linesize[i];
    ret->Set(i, NanNew<Integer>(linesize));
  }

  NanReturnValue(ret);
}

}  // namespace avcodec
}  // namespace ffmpeg
