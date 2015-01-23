#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avcodec/avsubtitle.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


FFmpeg::AVCodec::AVDecodeAudioWorker::
AVDecodeAudioWorker(std::list<NanAsyncWorker*> *q,
                    ::AVCodecContext *ctx,
                    ::AVFrame *pic, ::AVPacket *pkt,
                    NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), frame(pic), packet(pkt) {
}

FFmpeg::AVCodec::AVDecodeAudioWorker::~AVDecodeAudioWorker() {
}

void FFmpeg::AVCodec::AVDecodeAudioWorker::Execute() {
  result = avcodec_decode_audio4(context, frame, &got_frame_ptr, packet);
}

void FFmpeg::AVCodec::AVDecodeAudioWorker::HandleOKCallback() {
  NanScope();

  queue->pop_front();
  if (queue->size() > 0)
      NanAsyncQueueWorker(queue->front());

  Handle<Value> pkt = AVCodec::AVPacketWrapper::newInstance(packet);
  Handle<Value> pic = AVUtil::AVFrameWrapper::newInstance(frame);

  const int argc = 4;
  Handle<Value> argv[argc] = {
    NanNew<Number>(result),
    NanNew<Number>(got_frame_ptr),
    pic,
    pkt
  };
  callback->Call(argc, argv);
}


FFmpeg::AVCodec::AVDecodeVideoWorker::
AVDecodeVideoWorker(std::list<NanAsyncWorker*> *q,
                    ::AVCodecContext *ctx,
                    ::AVFrame *pic, ::AVPacket *pkt,
                    NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), frame(pic), packet(pkt) {
}

FFmpeg::AVCodec::AVDecodeVideoWorker::~AVDecodeVideoWorker() {
}

void FFmpeg::AVCodec::AVDecodeVideoWorker::Execute() {
  result = avcodec_decode_video2(context, frame, &got_frame_ptr, packet);
}

void FFmpeg::AVCodec::AVDecodeVideoWorker::HandleOKCallback() {
  NanScope();

  queue->pop_front();
  if (queue->size() > 0)
      NanAsyncQueueWorker(queue->front());

  Handle<Value> pkt = AVCodec::AVPacketWrapper::newInstance(packet);
  Handle<Value> pic = AVUtil::AVFrameWrapper::newInstance(frame);

  const int argc = 4;
  Handle<Value> argv[argc] = {
    NanNew<Number>(result),
    NanNew<Number>(got_frame_ptr),
    pic,
    pkt
  };
  callback->Call(argc, argv);
}


FFmpeg::AVCodec::AVDecodeSubtitleWorker::
AVDecodeSubtitleWorker(std::list<NanAsyncWorker*> *q,
                       ::AVCodecContext *ctx,
                       ::AVSubtitle *sub, ::AVPacket *pkt,
                       NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), subtt(sub), packet(pkt) {
}

FFmpeg::AVCodec::AVDecodeSubtitleWorker::~AVDecodeSubtitleWorker() {
}

void FFmpeg::AVCodec::AVDecodeSubtitleWorker::Execute() {
  result = avcodec_decode_subtitle2(context, subtt, &got_subtt_ptr, packet);
}

void FFmpeg::AVCodec::AVDecodeSubtitleWorker::HandleOKCallback() {
  NanScope();

  queue->pop_front();
  if (queue->size() > 0)
      NanAsyncQueueWorker(queue->front());

  Handle<Value> pkt = AVCodec::AVPacketWrapper::newInstance(packet);
  Handle<Value> sub = AVCodec::AVSubtitleWrapper::newInstance(subtt);

  const int argc = 4;
  Handle<Value> argv[argc] = {
    NanNew<Number>(result),
    NanNew<Number>(got_subtt_ptr),
    sub,
    pkt
  };
  callback->Call(argc, argv);
}


Persistent<FunctionTemplate>
FFmpeg::AVCodec::AVCodecContextWrapper::constructor;

FFmpeg::AVCodec::AVCodecContextWrapper::
AVCodecContextWrapper(::AVCodecContext *ctx)
  : _this(ctx), _allocated(false) {
  if (!_this) {
    _this = (::AVCodecContext *)av_mallocz(sizeof(::AVCodecContext));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVCodecContextWrapper::~AVCodecContextWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVCodec::AVCodecContextWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVCodecContext"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(ctor, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeAudio", DecodeAudio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeVideo", DecodeVideo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "decodeSubtitle", DecodeSubtitle);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeAudio", EncodeAudio);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeVideo", EncodeVideo);
  NODE_SET_PROTOTYPE_METHOD(ctor, "encodeSubtitle", EncodeSubtitle);
  NODE_SET_PROTOTYPE_METHOD(ctor, "flushBuffers", FlushBuffers);
  proto->SetAccessor(NanNew<String>("is_open"), GetIsOpen);
  proto->SetAccessor(NanNew<String>("codec_type"), GetCodecType);
  proto->SetAccessor(NanNew<String>("codec"), GetCodec);
  proto->SetAccessor(NanNew<String>("codec_id"), GetCodecId);
  proto->SetAccessor(NanNew<String>("flags"), GetFlags, SetFlags);
  proto->SetAccessor(NanNew<String>("flags2"), GetFlags2, SetFlags2);
  proto->SetAccessor(NanNew<String>("time_base"), GetTimeBase);
  proto->SetAccessor(NanNew<String>("width"), GetWidth);
  proto->SetAccessor(NanNew<String>("height"), GetHeight);
  proto->SetAccessor(NanNew<String>("pix_fmt"), GetPixFmt);
  proto->SetAccessor(NanNew<String>("sample_rate"), GetSampleRate);
  proto->SetAccessor(NanNew<String>("channels"), GetChannels);
  proto->SetAccessor(NanNew<String>("sample_fmt"), GetSampleFmt);
  proto->SetAccessor(NanNew<String>("channel_layout"), GetChannelLayout);
  proto->SetAccessor(NanNew<String>("workaround_bugs"),
                     GetWorkaroundBugs, SetWorkaroundBugs);
  proto->SetAccessor(NanNew<String>("error_concealment"),
                     GetErrorConcealment, SetErrorConcealment);
  proto->SetAccessor(NanNew<String>("lowres"), GetLowres, SetLowres);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVCodecContext"), creator);
}

Handle<Value>
FFmpeg::AVCodec::AVCodecContextWrapper::newInstance(::AVCodecContext *ctx) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(ctx) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVCodecContextWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVCodecContext *ctx = nullptr;
    if (args[0]->IsExternal())
      ctx = static_cast<::AVCodecContext *>(External::Unwrap(args[0]));
    AVCodecContextWrapper *obj = new AVCodecContextWrapper(ctx);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::Open) {
  NanScope();

  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());

  ::AVCodec *codec = nullptr;
  ::AVDictionary *options = nullptr;
  int argc = 0;

  if (!args[0]->IsUndefined()) {
    if (args[0]->IsObject()) {
      Local<Object> arg0 = args[0]->ToObject();
      if (AVCodecWrapper::HasInstance(arg0)) {
        codec = ObjectWrap::Unwrap<AVCodecWrapper>(arg0)->This();
        argc++;
      }
    } else if (args[0]->IsNumber()) {
      enum ::AVCodecID codec_id =
        static_cast<enum ::AVCodecID>(args[0]->Uint32Value());
      codec = avcodec_find_decoder(codec_id);
      argc++;
    } else if (args[0]->IsString()) {
      String::Utf8Value codec_name(args[0]);
      codec = avcodec_find_decoder_by_name(*codec_name);
      argc++;
    }
  }
  if (argc == 0)
    codec = avcodec_find_decoder(obj->_this->codec_id);

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> opts = args[argc]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString()) {
        String::Utf8Value key_str(key);
        String::Utf8Value val_str(val);
        av_dict_set(&options, *key_str, *val_str, 0);
      }
    }
    argc++;
  }

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("invalid arguments");
  }

  obj->_this->codec_id = codec->id;

  int ret = avcodec_open2(obj->_this, codec, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::Close) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int ret = avcodec_close(obj->_this);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::DecodeAudio) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVUtil::AVFrameWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("frame required");
  if (!args[1]->IsObject() ||
      !AVPacketWrapper::HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("avpkt required");

  ::AVFrame *frame =
    ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(args[0]->ToObject())->This();
  int got_frame_ptr;
  const ::AVPacket *avpkt =
    ObjectWrap::Unwrap<AVPacketWrapper>(args[1]->ToObject())->This();

  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());

  if (args[2]->IsFunction()) {
    NanCallback *callback = new NanCallback(Local<Function>::Cast(args[2]));
    AVDecodeAudioWorker *worker =
      new AVDecodeAudioWorker(&obj->_async_queue,
                              obj->_this,
                              frame,
                              const_cast<::AVPacket*>(avpkt),
                              callback);
    obj->_async_queue.push_back(worker);
    if (obj->_async_queue.size() == 1)
      NanAsyncQueueWorker(obj->_async_queue.front());
    NanReturnUndefined();
  }

  int ret = avcodec_decode_audio4(obj->_this, frame, &got_frame_ptr, avpkt);
  Handle<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Number>(ret));
  rets->Set(1, NanNew<Number>(got_frame_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::DecodeVideo) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVUtil::AVFrameWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("picture required");
  if (!args[1]->IsObject() ||
      !AVPacketWrapper::HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("avpkt required");

  ::AVFrame *picture =
    ObjectWrap::Unwrap<AVUtil::AVFrameWrapper>(args[0]->ToObject())->This();
  int got_picture_ptr;
  const ::AVPacket *avpkt =
    ObjectWrap::Unwrap<AVPacketWrapper>(args[1]->ToObject())->This();

  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());

  if (args[2]->IsFunction()) {
    NanCallback *callback = new NanCallback(Local<Function>::Cast(args[2]));
    AVDecodeVideoWorker *worker =
      new AVDecodeVideoWorker(&obj->_async_queue,
                              obj->_this,
                              picture,
                              const_cast<::AVPacket*>(avpkt),
                              callback);
    obj->_async_queue.push_back(worker);
    if (obj->_async_queue.size() == 1)
      NanAsyncQueueWorker(obj->_async_queue.front());
    NanReturnUndefined();
  }

  int ret = avcodec_decode_video2(obj->_this, picture, &got_picture_ptr, avpkt);
  Handle<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Number>(ret));
  rets->Set(1, NanNew<Number>(got_picture_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::DecodeSubtitle) {
  NanScope();

  if (!args[0]->IsObject() ||
      !AVSubtitleWrapper::HasInstance(args[0]->ToObject()))
    return NanThrowTypeError("subtitle required");
  if (!args[1]->IsObject() ||
      !AVPacketWrapper::HasInstance(args[1]->ToObject()))
    return NanThrowTypeError("avpkt required");

  ::AVSubtitle *subtitle =
    ObjectWrap::Unwrap<AVSubtitleWrapper>(args[0]->ToObject())->This();
  int got_sub_ptr;
  ::AVPacket *avpkt =
    ObjectWrap::Unwrap<AVPacketWrapper>(args[1]->ToObject())->This();

  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());

  if (args[2]->IsFunction()) {
    NanCallback *callback = new NanCallback(Local<Function>::Cast(args[2]));
    AVDecodeSubtitleWorker *worker =
      new AVDecodeSubtitleWorker(&obj->_async_queue,
                                 obj->_this, subtitle, avpkt, callback);
    obj->_async_queue.push_back(worker);
    if (obj->_async_queue.size() == 1)
      NanAsyncQueueWorker(obj->_async_queue.front());
    NanReturnUndefined();
  }

  int ret = avcodec_decode_subtitle2(obj->_this, subtitle, &got_sub_ptr, avpkt);
  Handle<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Number>(ret));
  rets->Set(1, NanNew<Number>(got_sub_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::EncodeAudio) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::EncodeVideo) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::EncodeSubtitle) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecContextWrapper::FlushBuffers) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  avcodec_flush_buffers(obj->_this);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetIsOpen) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int is_open = avcodec_is_open(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_open));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetCodecType) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum AVMediaType codec_type = obj->_this->codec_type;
  NanReturnValue(NanNew<Number>(codec_type));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetCodec) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  if (!obj->_this->codec)
    NanReturnNull();
  Handle<Value> ret =
    AVCodecWrapper::newInstance(const_cast<::AVCodec *>(obj->_this->codec));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetCodecId) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum ::AVCodecID codec_id = obj->_this->codec_id;
  NanReturnValue(NanNew<Number>(codec_id));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetFlags) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int flags = obj->_this->flags;
  NanReturnValue(NanNew<Integer>(flags));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetFlags2) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int flags2 = obj->_this->flags2;
  NanReturnValue(NanNew<Integer>(flags2));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetTimeBase) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  ::AVRational time_base = obj->_this->time_base;
  Handle<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Number>(time_base.num));
  ret->Set(NanNew<String>("den"), NanNew<Number>(time_base.den));
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetWidth) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int width = obj->_this->width;
  NanReturnValue(NanNew<Number>(width));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetHeight) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int height = obj->_this->height;
  NanReturnValue(NanNew<Number>(height));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetPixFmt) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum ::AVPixelFormat pix_fmt = obj->_this->pix_fmt;
  NanReturnValue(NanNew<Number>(pix_fmt));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetSampleRate) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int sample_rate = obj->_this->sample_rate;
  NanReturnValue(NanNew<Number>(sample_rate));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetChannels) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int channels = obj->_this->channels;
  NanReturnValue(NanNew<Number>(channels));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetSampleFmt) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  enum ::AVSampleFormat sample_fmt = obj->_this->sample_fmt;
  NanReturnValue(NanNew<Number>(sample_fmt));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetChannelLayout) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  uint64_t channel_layout = obj->_this->channel_layout;
  NanReturnValue(NanNew<Number>(channel_layout));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetWorkaroundBugs) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int workaround_bugs = obj->_this->workaround_bugs;
  NanReturnValue(NanNew<Number>(workaround_bugs));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetErrorConcealment) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int error_concealment = obj->_this->error_concealment;
  NanReturnValue(NanNew<Number>(error_concealment));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecContextWrapper::GetLowres) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  int lowres = obj->_this->lowres;
  NanReturnValue(NanNew<Number>(lowres));
}

NAN_SETTER(FFmpeg::AVCodec::AVCodecContextWrapper::SetFlags) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->flags = value->Int32Value();
}

NAN_SETTER(FFmpeg::AVCodec::AVCodecContextWrapper::SetFlags2) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->flags2 = value->Int32Value();
}

NAN_SETTER(FFmpeg::AVCodec::AVCodecContextWrapper::SetWorkaroundBugs) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->workaround_bugs = value->NumberValue();
}

NAN_SETTER(FFmpeg::AVCodec::AVCodecContextWrapper::SetErrorConcealment) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->error_concealment = value->NumberValue();
}

NAN_SETTER(FFmpeg::AVCodec::AVCodecContextWrapper::SetLowres) {
  NanScope();
  AVCodecContextWrapper *obj =
    ObjectWrap::Unwrap<AVCodecContextWrapper>(args.This());
  obj->_this->lowres = value->NumberValue();
}
