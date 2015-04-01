#include "avcodec/avpicture.h"
#include "avutil/avframe.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {

Persistent<FunctionTemplate> AVPicture::constructor;

void AVPicture::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVPicture"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "alloc", Alloc);
  NODE_SET_PROTOTYPE_METHOD(tpl, "free", Free);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fill", Fill);
  NODE_SET_PROTOTYPE_METHOD(tpl, "layout", Layout);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", Copy);
  NODE_SET_PROTOTYPE_METHOD(tpl, "crop", Crop);
  NODE_SET_PROTOTYPE_METHOD(tpl, "pad", Pad);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("data"), GetData);
  inst->SetAccessor(NanNew("linesize"), GetLinesize);
  inst->SetAccessor(NanNew("pix_fmt"), GetPixFmt, SetPixFmt);
  inst->SetAccessor(NanNew("width"), GetWidth, SetWidth);
  inst->SetAccessor(NanNew("height"), GetHeight, SetHeight);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVPicture"), tpl->GetFunction());

  NODE_SET_METHOD(tpl->GetFunction(), "getSize", GetSize);
}

Local<Object> AVPicture::NewInstance(::AVPicture* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVPicture>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVPicture::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVPicture* AVPicture::This(::AVPicture* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

AVPicture::AVPicture() : pix_fmt_(AV_PIX_FMT_NONE), width_(0), height_(0) {
  this_ = (::AVPicture*)av_mallocz(sizeof(::AVPicture));
  if (this_ == nullptr)
    NanThrowTypeError("AVPicture: cannot allocation");
}

AVPicture::~AVPicture() {
  if (this_ != nullptr)
    av_freep(&this_);
}

NAN_METHOD(AVPicture::GetSize) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getSize: AVPixelFormat enum required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("getSize: width integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("getSize: height integer required");

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Int32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();

  int ret = avpicture_get_size(pix_fmt, width, height);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPicture::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVPicture* obj = new AVPicture();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVPicture::Alloc) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("alloc: AVPixelFormat enum required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("alloc: width integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("alloc: height integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Int32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();

  int ret = avpicture_alloc(obj->This(), pix_fmt, width, height);
  if (ret == 0) {
    obj->pix_fmt_ = pix_fmt;
    obj->width_ = width;
    obj->height_ = height;
  }

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPicture::Free) {
  NanScope();

  AVPicture* obj = Unwrap<AVPicture>(args.This());
  avpicture_free(obj->This());
  memset(obj->This(), 0, sizeof(::AVPicture));
  obj->pix_fmt_ = AV_PIX_FMT_NONE;
  obj->width_ = 0;
  obj->height_ = 0;

  NanReturnUndefined();
}

NAN_METHOD(AVPicture::Fill) {
  NanScope();

  bool hasUint8Array = false;
  void* data = nullptr;

  if (args[0]->IsObject()) {
    Local<Object> arg0 = args[0]->ToObject();
    hasUint8Array = arg0->HasIndexedPropertiesInExternalArrayData();
    data = arg0->GetIndexedPropertiesExternalArrayData();
    ExternalArrayType type = arg0->GetIndexedPropertiesExternalArrayDataType();
    if (!hasUint8Array || type != kExternalUint8Array)
      return NanThrowTypeError("fill: ptr Uint8Array required");
  }

  int argc = hasUint8Array ? 1 : 0;

  if (argc != 1)
    return NanThrowTypeError("fill: ptr Uint8Array required");
  if (!args[argc + 0]->IsNumber())
    return NanThrowTypeError("fill: AVPixelFormat enum required");
  if (!args[argc + 1]->IsNumber())
    return NanThrowTypeError("fill: width integer required");
  if (!args[argc + 2]->IsNumber())
    return NanThrowTypeError("fill: height integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  const uint8_t* ptr = nullptr;
  if (argc == 1)
    ptr = reinterpret_cast<const uint8_t*>(data);
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[argc + 0]->Int32Value());
  int width = args[argc + 1]->Int32Value();
  int height = args[argc + 2]->Int32Value();

  int ret = avpicture_fill(obj->This(), ptr, pix_fmt, width, height);
  if (ret > 0) {
    obj->pix_fmt_ = pix_fmt;
    obj->width_ = width;
    obj->height_ = height;
  }

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPicture::Layout) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("layout: AVPixelFormat enum required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("layout: width integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("layout: height integer required");
  if (!args[3]->IsObject())
    return NanThrowTypeError("layout: dest Uint8Array required");

  Local<Object> arg3 = args[3]->ToObject();
  bool hasUint8Array = arg3->HasIndexedPropertiesInExternalArrayData();
  void* data = arg3->GetIndexedPropertiesExternalArrayData();
  ExternalArrayType type = arg3->GetIndexedPropertiesExternalArrayDataType();
  int size = arg3->GetIndexedPropertiesExternalArrayDataLength();
  if (!hasUint8Array || type != kExternalUint8Array)
    return NanThrowTypeError("layout: dest Uint8Array required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[0]->Int32Value());
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  unsigned char* dest = reinterpret_cast<unsigned char*>(data);
  int dest_size = size;

  int ret = avpicture_layout(obj->This(),
    pix_fmt, width, height, dest, dest_size);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPicture::Copy) {
  NanScope();

  if (!AVPicture::HasInstance(args[0]) &&
      !avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("copy: src AVPicture/AVFrame instance required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("copy: pix_fmt AVPixelFormat enum required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("copy: width integer required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("copy: height integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  AVPicture* src;
  if (AVPicture::HasInstance(args[0]))
    src = Unwrap<AVPicture>(args[0]->ToObject());
  else {
    avutil::AVFrame* frame = Unwrap<avutil::AVFrame>(args[0]->ToObject());
    src = reinterpret_cast<AVPicture*>(frame);
  }
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[1]->Int32Value());
  int width = args[2]->Int32Value();
  int height = args[3]->Int32Value();

  av_picture_copy(obj->This(), src->This(), pix_fmt, width, height);
  obj->pix_fmt_ = pix_fmt;
  obj->width_ = width;
  obj->height_ = height;

  NanReturnUndefined();
}

NAN_METHOD(AVPicture::Crop) {
  NanScope();

  if (!AVPicture::HasInstance(args[0]) &&
      !avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("crop: src AVPicture/AVFrame instance required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("crop: pix_fmt AVPixelFormat enum required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("crop: top_band integer required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("crop: left_band integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  AVPicture* src;
  if (AVPicture::HasInstance(args[0]))
    src = Unwrap<AVPicture>(args[0]->ToObject());
  else {
    avutil::AVFrame* frame = Unwrap<avutil::AVFrame>(args[0]->ToObject());
    src = reinterpret_cast<AVPicture*>(frame);
  }
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[1]->Int32Value());
  int top_band = args[2]->Int32Value();
  int left_band = args[3]->Int32Value();

  int ret = av_picture_crop(obj->This(), src->This(),
    pix_fmt, top_band, left_band);
  if (ret == 0)
    obj->pix_fmt_ = pix_fmt;

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVPicture::Pad) {
  NanScope();

  if (!AVPicture::HasInstance(args[0]) &&
      !avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("pad: src AVPicture/AVFrame instance required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("pad: width integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("pad: height integer required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("pad: pix_fmt AVPixelFormat enum required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("pad: padtop integer required");
  if (!args[5]->IsNumber())
    return NanThrowTypeError("pad: padbottom integer required");
  if (!args[6]->IsNumber())
    return NanThrowTypeError("pad: padleft integer required");
  if (!args[7]->IsNumber())
    return NanThrowTypeError("pad: padright integer required");
  if (!args[8]->IsArray())
    return NanThrowTypeError("pad: color int* required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  AVPicture* src;
  if (AVPicture::HasInstance(args[0]))
    src = Unwrap<AVPicture>(args[0]->ToObject());
  else {
    avutil::AVFrame* frame = Unwrap<avutil::AVFrame>(args[0]->ToObject());
    src = reinterpret_cast<AVPicture*>(frame);
  }
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();
  enum ::AVPixelFormat pix_fmt =
    static_cast<enum ::AVPixelFormat>(args[3]->Int32Value());
  int padtop = args[4]->Int32Value();
  int padbottom = args[5]->Int32Value();
  int padleft = args[6]->Int32Value();
  int padright = args[7]->Int32Value();
  int color[4] = {0, };
  Local<Array> colors = args[8].As<Array>();
  for (uint32_t i = 0; i < 4 && i < colors->Length(); i++) {
    color[i] = colors->Get(i)->Uint32Value();
  }

  int ret = av_picture_pad(obj->This(), src->This(),
    height, width, pix_fmt,
    padtop, padbottom, padleft, padright, color);
  if (ret == 0) {
    obj->pix_fmt_ = pix_fmt;
    obj->width_ = width;
    obj->height_ = height;
  }

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_GETTER(AVPicture::GetData) {
  NanScope();

  AVPicture* obj = Unwrap<AVPicture>(args.This());
  ::AVPicture* wrap = obj->This();

  int total = avpicture_get_size(obj->pix_fmt_, obj->width_, obj->height_);
  ExternalArrayType type = kExternalUint8Array;
  int size[AV_NUM_DATA_POINTERS] = {0, };

  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    size[i] = 0;
    if (wrap->data[i]) {
      if (i < AV_NUM_DATA_POINTERS - 1 && wrap->data[i + 1])
        size[i] = wrap->data[i + 1] - wrap->data[i];
      else
        size[i] = total;
      total -= size[i];
    }
  }

  Local<Array> rets = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    Local<Object> ret = NanNew<Object>();
    uint8_t* data = wrap->data[i];
    if (data)
      ret->SetIndexedPropertiesToExternalArrayData(data, type, size[i]);
    rets->Set(i, ret);
  }

  NanReturnValue(rets);
}

NAN_GETTER(AVPicture::GetLinesize) {
  NanScope();

  ::AVPicture* wrap = Unwrap<AVPicture>(args.This())->This();

  Local<Array> rets = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    int linesize = wrap->linesize[i];
    rets->Set(i, NanNew<Int32>(linesize));
  }

  NanReturnValue(rets);
}

NAN_GETTER(AVPicture::GetPixFmt) {
  NanScope();

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  enum ::AVPixelFormat pix_fmt = obj->pix_fmt_;
  NanReturnValue(NanNew<Int32>(pix_fmt));
}

NAN_SETTER(AVPicture::SetPixFmt) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pix_fmt: AVPixelFormat num required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  obj->pix_fmt_ = static_cast<enum ::AVPixelFormat>(value->Int32Value());
}

NAN_GETTER(AVPicture::GetWidth) {
  NanScope();

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  int width = obj->width_;
  NanReturnValue(NanNew<Int32>(width));
}

NAN_SETTER(AVPicture::SetWidth) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("width: integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  obj->width_ = value->Int32Value();
}

NAN_GETTER(AVPicture::GetHeight) {
  NanScope();

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  int height = obj->height_;
  NanReturnValue(NanNew<Int32>(height));
}

NAN_SETTER(AVPicture::SetHeight) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("height: integer required");

  AVPicture* obj = Unwrap<AVPicture>(args.This());

  obj->height_ = value->Int32Value();
}

}  // namespace avcodec
}  // namespace ffmpeg
