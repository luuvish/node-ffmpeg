#include "avcodec.h"
#include "avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


void FFmpeg::AVCodec::Initialize(Handle<Object> target) {
  NanScope();

  AVPacketWrapper::Initialize(target);
  AVCodecContextWrapper::Initialize(target);
  AVCodecWrapper::Initialize(target);
  AVPictureWrapper::Initialize(target);
  AVSubtitleRectWrapper::Initialize(target);
  AVSubtitleWrapper::Initialize(target);

  // libavcodec/avcodec.h
  SetContant(target, "AVDISCARD_NONE", ::AVDISCARD_NONE);
  SetContant(target, "AVDISCARD_DEFAULT", ::AVDISCARD_DEFAULT);
  SetContant(target, "AVDISCARD_NONREF", ::AVDISCARD_NONREF);
  SetContant(target, "AVDISCARD_BIDIR", ::AVDISCARD_BIDIR);
  SetContant(target, "AVDISCARD_NONKEY", ::AVDISCARD_NONKEY);
  SetContant(target, "AVDISCARD_ALL", ::AVDISCARD_ALL);

  SetContant(target, "CODEC_FLAG_UNALIGNED", CODEC_FLAG_UNALIGNED);
  SetContant(target, "CODEC_FLAG_QSCALE", CODEC_FLAG_QSCALE);
  SetContant(target, "CODEC_FLAG_4MV", CODEC_FLAG_4MV);
  SetContant(target, "CODEC_FLAG_OUTPUT_CORRUPT", CODEC_FLAG_OUTPUT_CORRUPT);
  SetContant(target, "CODEC_FLAG_QPEL", CODEC_FLAG_QPEL);
  SetContant(target, "CODEC_FLAG_GMC", CODEC_FLAG_GMC);
  SetContant(target, "CODEC_FLAG_MV0", CODEC_FLAG_MV0);
  SetContant(target, "CODEC_FLAG_INPUT_PRESERVED", CODEC_FLAG_INPUT_PRESERVED);
  SetContant(target, "CODEC_FLAG_PASS1", CODEC_FLAG_PASS1);
  SetContant(target, "CODEC_FLAG_PASS2", CODEC_FLAG_PASS2);
  SetContant(target, "CODEC_FLAG_GRAY", CODEC_FLAG_GRAY);
  SetContant(target, "CODEC_FLAG_EMU_EDGE", CODEC_FLAG_EMU_EDGE);
  SetContant(target, "CODEC_FLAG_PSNR", CODEC_FLAG_PSNR);
  SetContant(target, "CODEC_FLAG_TRUNCATED", CODEC_FLAG_TRUNCATED);
  SetContant(target, "CODEC_FLAG_NORMALIZE_AQP", CODEC_FLAG_NORMALIZE_AQP);
  SetContant(target, "CODEC_FLAG_INTERLACED_DCT", CODEC_FLAG_INTERLACED_DCT);
  SetContant(target, "CODEC_FLAG_LOW_DELAY", CODEC_FLAG_LOW_DELAY);
  SetContant(target, "CODEC_FLAG_GLOBAL_HEADER", CODEC_FLAG_GLOBAL_HEADER);
  SetContant(target, "CODEC_FLAG_BITEXACT", CODEC_FLAG_BITEXACT);
  SetContant(target, "CODEC_FLAG_AC_PRED", CODEC_FLAG_AC_PRED);
  SetContant(target, "CODEC_FLAG_LOOP_FILTER", CODEC_FLAG_LOOP_FILTER);
  SetContant(target, "CODEC_FLAG_INTERLACED_ME", CODEC_FLAG_INTERLACED_ME);
  SetContant(target, "CODEC_FLAG_CLOSED_GOP", CODEC_FLAG_CLOSED_GOP);

  SetContant(target, "CODEC_FLAG2_FAST", CODEC_FLAG2_FAST);
  SetContant(target, "CODEC_FLAG2_NO_OUTPUT", CODEC_FLAG2_NO_OUTPUT);
  SetContant(target, "CODEC_FLAG2_LOCAL_HEADER", CODEC_FLAG2_LOCAL_HEADER);
  SetContant(target, "CODEC_FLAG2_DROP_FRAME_TIMECODE",
                     CODEC_FLAG2_DROP_FRAME_TIMECODE);
  SetContant(target, "CODEC_FLAG2_IGNORE_CROP", CODEC_FLAG2_IGNORE_CROP);
  SetContant(target, "CODEC_FLAG2_CHUNKS", CODEC_FLAG2_CHUNKS);
  SetContant(target, "CODEC_FLAG2_SHOW_ALL", CODEC_FLAG2_SHOW_ALL);

  SetContant(target, "CODEC_CAP_DRAW_HORIZ_BAND", CODEC_CAP_DRAW_HORIZ_BAND);
  SetContant(target, "CODEC_CAP_DR1", CODEC_CAP_DR1);
  SetContant(target, "CODEC_CAP_TRUNCATED", CODEC_CAP_TRUNCATED);
  SetContant(target, "CODEC_CAP_HWACCEL", CODEC_CAP_HWACCEL);
  SetContant(target, "CODEC_CAP_DELAY", CODEC_CAP_DELAY);
  SetContant(target, "CODEC_CAP_SMALL_LAST_FRAME", CODEC_CAP_SMALL_LAST_FRAME);
  SetContant(target, "CODEC_CAP_HWACCEL_VDPAU", CODEC_CAP_HWACCEL_VDPAU);
  SetContant(target, "CODEC_CAP_SUBFRAMES", CODEC_CAP_SUBFRAMES);
  SetContant(target, "CODEC_CAP_EXPERIMENTAL", CODEC_CAP_EXPERIMENTAL);
  SetContant(target, "CODEC_CAP_CHANNEL_CONF", CODEC_CAP_CHANNEL_CONF);
  SetContant(target, "CODEC_CAP_NEG_LINESIZES", CODEC_CAP_NEG_LINESIZES);
  SetContant(target, "CODEC_CAP_FRAME_THREADS", CODEC_CAP_FRAME_THREADS);
  SetContant(target, "CODEC_CAP_SLICE_THREADS", CODEC_CAP_SLICE_THREADS);
  SetContant(target, "CODEC_CAP_PARAM_CHANGE", CODEC_CAP_PARAM_CHANGE);
  SetContant(target, "CODEC_CAP_AUTO_THREADS", CODEC_CAP_AUTO_THREADS);
  SetContant(target, "CODEC_CAP_VARIABLE_FRAME_SIZE",
                     CODEC_CAP_VARIABLE_FRAME_SIZE);
  SetContant(target, "CODEC_CAP_INTRA_ONLY", CODEC_CAP_INTRA_ONLY);
  SetContant(target, "CODEC_CAP_LOSSLESS", CODEC_CAP_LOSSLESS);

  SetContant(target, "SUBTITLE_NONE", ::SUBTITLE_NONE);
  SetContant(target, "SUBTITLE_BITMAP", ::SUBTITLE_BITMAP);
  SetContant(target, "SUBTITLE_TEXT", ::SUBTITLE_TEXT);
  SetContant(target, "SUBTITLE_ASS", ::SUBTITLE_ASS);
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


Persistent<FunctionTemplate> FFmpeg::AVCodec::AVCodecWrapper::constructor;

FFmpeg::AVCodec::AVCodecWrapper::AVCodecWrapper(::AVCodec *codec)
  : _this(codec), _allocated(false) {
  if (!_this) {
    _this = (::AVCodec *)av_mallocz(sizeof(::AVCodec));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVCodecWrapper::~AVCodecWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVCodec::AVCodecWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVCodec"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("is_encoder"), GetIsEncoder);
  proto->SetAccessor(NanNew<String>("is_decoder"), GetIsDecoder);
  proto->SetAccessor(NanNew<String>("name"), GetName);
  proto->SetAccessor(NanNew<String>("long_name"), GetLongName);
  proto->SetAccessor(NanNew<String>("type"), GetType);
  proto->SetAccessor(NanNew<String>("id"), GetId);
  proto->SetAccessor(NanNew<String>("capabilities"), GetCapabilities);
  proto->SetAccessor(NanNew<String>("max_lowres"), GetMaxLowres);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVCodec"), creator);
  NODE_SET_METHOD(creator, "findDecoder", FindDecoder);
  NODE_SET_METHOD(creator, "findEncoder", FindEncoder);
  NODE_SET_METHOD(creator, "getType", GetMediaType);
  NODE_SET_METHOD(creator, "getName", GetCodecName);
}

Handle<Value> FFmpeg::AVCodec::AVCodecWrapper::newInstance(::AVCodec *codec) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(codec) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVCodecWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVCodec *codec = nullptr;
    if (args[0]->IsExternal())
      codec = static_cast<::AVCodec *>(External::Unwrap(args[0]));
    AVCodecWrapper *obj = new AVCodecWrapper(codec);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::FindDecoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  ::AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum ::AVCodecID codec_id =
      static_cast<enum ::AVCodecID>(args[0]->NumberValue());
    codec = avcodec_find_decoder(codec_id);
  }
  if (args[0]->IsString()) {
    String::Utf8Value codec_name(args[0]);
    codec = avcodec_find_decoder_by_name(*codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::FindEncoder) {
  NanScope();

  if (!args[0]->IsNumber() && !args[0]->IsString())
    return NanThrowTypeError("codec id or codec name required");

  ::AVCodec *codec = nullptr;

  if (args[0]->IsNumber()) {
    enum ::AVCodecID codec_id =
      static_cast<enum ::AVCodecID>(args[0]->Uint32Value());
    codec = avcodec_find_encoder(codec_id);
  }
  if (args[0]->IsString()) {
    String::Utf8Value codec_name(args[0]);
    codec = avcodec_find_encoder_by_name(*codec_name);
  }

  if (codec)
    NanReturnValue(newInstance(codec));
  else
    NanReturnNull();
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::GetMediaType) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("codec id required");
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[0]->Uint32Value());
  enum ::AVMediaType type = avcodec_get_type(codec_id);
  NanReturnValue(NanNew<Number>(type));
}

NAN_METHOD(FFmpeg::AVCodec::AVCodecWrapper::GetCodecName) {
  NanScope();
  if (!args[0]->IsNumber())
    return NanThrowTypeError("codec id required");
  enum ::AVCodecID codec_id =
    static_cast<enum ::AVCodecID>(args[0]->NumberValue());
  const char *name = avcodec_get_name(codec_id);
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetIsEncoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_encoder = av_codec_is_encoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_encoder));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetIsDecoder) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int is_decoder = av_codec_is_decoder(obj->_this);
  NanReturnValue(NanNew<Boolean>(is_decoder));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *name = obj->_this->name;
  NanReturnValue(NanNew<String>(name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetLongName) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  const char *long_name = obj->_this->long_name;
  NanReturnValue(NanNew<String>(long_name));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetType) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum ::AVMediaType type = obj->_this->type;
  NanReturnValue(NanNew<Number>(type));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetId) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  enum ::AVCodecID id = obj->_this->id;
  NanReturnValue(NanNew<Number>(id));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetCapabilities) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  int capabilities = obj->_this->capabilities;
  NanReturnValue(NanNew<Integer>(capabilities));
}

NAN_GETTER(FFmpeg::AVCodec::AVCodecWrapper::GetMaxLowres) {
  NanScope();
  AVCodecWrapper *obj = ObjectWrap::Unwrap<AVCodecWrapper>(args.This());
  uint8_t max_lowres = obj->_this->max_lowres;
  NanReturnValue(NanNew<Number>(max_lowres));
}


Persistent<FunctionTemplate> FFmpeg::AVCodec::AVPictureWrapper::constructor;

FFmpeg::AVCodec::AVPictureWrapper::
AVPictureWrapper(::AVPicture *picture, int w, int h)
  : _this(picture), _allocated(false), _w(w), _h(h) {
  if (!_this) {
    _this = (::AVPicture *)av_mallocz(sizeof(::AVPicture));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVPictureWrapper::~AVPictureWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVCodec::AVPictureWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVPicture"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("data"), GetData);
  proto->SetAccessor(NanNew<String>("linesize"), GetLinesize);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVPicture"), creator);
}

Handle<Value>
FFmpeg::AVCodec::AVPictureWrapper::
newInstance(::AVPicture *picture, int w, int h) {
  NanScope();
  const int argc = 3;
  Handle<Value> argv[argc] =
    { NanNew<External>(picture), NanNew<Number>(w), NanNew<Number>(h) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVPictureWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVPictureWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVPicture *picture = nullptr;
    if (args[0]->IsExternal())
      picture = static_cast<::AVPicture *>(External::Unwrap(args[0]));
    int w = args[1]->IsNumber() ? args[1]->Int32Value() : 0;
    int h = args[2]->IsNumber() ? args[2]->Int32Value() : 0;
    AVPictureWrapper *obj = new AVPictureWrapper(picture, w, h);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVCodec::AVPictureWrapper::GetData) {
  NanScope();
  AVPictureWrapper *obj = ObjectWrap::Unwrap<AVPictureWrapper>(args.This());
  Handle<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    Local<Value> v;
    if (obj->_this->data[i]) {
      char *data = reinterpret_cast<char *>(obj->_this->data[i]);
      int size = obj->_w * obj->_h;
      Buffer *slowBuffer = Buffer::New(size);
      memcpy(Buffer::Data(slowBuffer), data, size);
      Local<Object> globalObj = Context::GetCurrent()->Global();
      Local<Function> bufferConstructor =
        Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
      Handle<Value> constructorArgs[3] =
        { slowBuffer->handle_, Integer::New(size), Integer::New(0) };
      v = bufferConstructor->NewInstance(3, constructorArgs);
      // v = NanNewBufferHandle(data, size);
    } else
      v = NanNull();
    ret->Set(i, v);
  }
  NanReturnValue(ret);
}

NAN_GETTER(FFmpeg::AVCodec::AVPictureWrapper::GetLinesize) {
  NanScope();
  AVPictureWrapper *obj = ObjectWrap::Unwrap<AVPictureWrapper>(args.This());
  Handle<Array> ret = NanNew<Array>(AV_NUM_DATA_POINTERS);
  for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++)
    ret->Set(i, NanNew<Number>(obj->_this->linesize[i]));
  NanReturnValue(ret);
}


Persistent<FunctionTemplate>
FFmpeg::AVCodec::AVSubtitleRectWrapper::constructor;

FFmpeg::AVCodec::AVSubtitleRectWrapper::
AVSubtitleRectWrapper(::AVSubtitleRect *rect)
  : _this(rect), _allocated(false) {
  if (!_this) {
    _this = (::AVSubtitleRect *)av_mallocz(sizeof(::AVSubtitleRect));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVSubtitleRectWrapper::~AVSubtitleRectWrapper() {
  if (_this && _allocated)
    av_freep(&_this);
}

void FFmpeg::AVCodec::AVSubtitleRectWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVSubtitleRect"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  proto->SetAccessor(NanNew<String>("x"), GetX);
  proto->SetAccessor(NanNew<String>("y"), GetY);
  proto->SetAccessor(NanNew<String>("w"), GetW);
  proto->SetAccessor(NanNew<String>("h"), GetH);
  proto->SetAccessor(NanNew<String>("nb_colors"), GetNbColors);
  proto->SetAccessor(NanNew<String>("pict"), GetPict);
  proto->SetAccessor(NanNew<String>("type"), GetType);
  proto->SetAccessor(NanNew<String>("text"), GetText);
  proto->SetAccessor(NanNew<String>("ass"), GetAss);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVSubtitleRect"), creator);
}

Handle<Value>
FFmpeg::AVCodec::AVSubtitleRectWrapper::newInstance(::AVSubtitleRect *rect) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(rect) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVSubtitleRectWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVSubtitleRectWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVSubtitleRect *rect = nullptr;
    if (args[0]->IsExternal())
      rect = static_cast<::AVSubtitleRect *>(External::Unwrap(args[0]));
    AVSubtitleRectWrapper *obj = new AVSubtitleRectWrapper(rect);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetX) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int x = obj->_this->x;
  NanReturnValue(NanNew<Number>(x));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetY) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int y = obj->_this->y;
  NanReturnValue(NanNew<Number>(y));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetW) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int w = obj->_this->w;
  NanReturnValue(NanNew<Number>(w));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetH) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int h = obj->_this->h;
  NanReturnValue(NanNew<Number>(h));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetNbColors) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  int nb_colors = obj->_this->nb_colors;
  NanReturnValue(NanNew<Number>(nb_colors));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetPict) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  Handle<Value> pict = AVPictureWrapper::newInstance(&obj->_this->pict,
                                                     obj->_this->w,
                                                     obj->_this->h);
  NanReturnValue(pict);
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetType) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  enum ::AVSubtitleType type = obj->_this->type;
  NanReturnValue(NanNew<Number>(type));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetText) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  char *text = obj->_this->text;
  NanReturnValue(NanNew<String>(text ? text : ""));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleRectWrapper::GetAss) {
  NanScope();
  AVSubtitleRectWrapper *obj =
    ObjectWrap::Unwrap<AVSubtitleRectWrapper>(args.This());
  char *ass = obj->_this->ass;
  NanReturnValue(NanNew<String>(ass ? ass : ""));
}


Persistent<FunctionTemplate> FFmpeg::AVCodec::AVSubtitleWrapper::constructor;

FFmpeg::AVCodec::AVSubtitleWrapper::AVSubtitleWrapper(::AVSubtitle *subtitle)
  : _this(subtitle), _allocated(false) {
  if (!_this) {
    _this = (::AVSubtitle *)av_mallocz(sizeof(::AVSubtitle));
    _allocated = true;
  }
}

FFmpeg::AVCodec::AVSubtitleWrapper::~AVSubtitleWrapper() {
  if (_this && _allocated) {
    avsubtitle_free(_this);
    av_freep(&_this);
  }
}

void FFmpeg::AVCodec::AVSubtitleWrapper::Initialize(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew<String>("AVSubtitle"));

  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  NODE_SET_PROTOTYPE_METHOD(ctor, "free", Free);
  proto->SetAccessor(NanNew<String>("format"), GetFormat);
  proto->SetAccessor(NanNew<String>("start_display_time"), GetStartDisplayTime);
  proto->SetAccessor(NanNew<String>("end_display_time"), GetEndDisplayTime);
  proto->SetAccessor(NanNew<String>("rects"), GetRects);
  proto->SetAccessor(NanNew<String>("pts"), GetPts);

  Local<Function> creator = ctor->GetFunction();
  target->Set(NanNew<String>("AVSubtitle"), creator);
}

Handle<Value>
FFmpeg::AVCodec::AVSubtitleWrapper::newInstance(::AVSubtitle *subtitle) {
  NanScope();
  const int argc = 1;
  Handle<Value> argv[argc] = { NanNew<External>(subtitle) };
  NanReturnValue(constructor->GetFunction()->NewInstance(argc, argv));
}

bool FFmpeg::AVCodec::AVSubtitleWrapper::HasInstance(Handle<Object> obj) {
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(FFmpeg::AVCodec::AVSubtitleWrapper::New) {
  if (args.IsConstructCall()) {
    NanScope();
    ::AVSubtitle *subtitle = nullptr;
    if (args[0]->IsExternal())
      subtitle = static_cast<AVSubtitle *>(External::Unwrap(args[0]));
    AVSubtitleWrapper *obj = new AVSubtitleWrapper(subtitle);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  }
  NanScope();
  NanReturnValue(constructor->GetFunction()->NewInstance());
}

NAN_METHOD(FFmpeg::AVCodec::AVSubtitleWrapper::Free) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  avsubtitle_free(obj->_this);
  NanReturnUndefined();
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleWrapper::GetFormat) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint16_t format = obj->_this->format;
  NanReturnValue(NanNew<Number>(format));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleWrapper::GetStartDisplayTime) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint32_t start_display_time = obj->_this->start_display_time;
  NanReturnValue(NanNew<Number>(start_display_time));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleWrapper::GetEndDisplayTime) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  uint32_t end_display_time = obj->_this->end_display_time;
  NanReturnValue(NanNew<Number>(end_display_time));
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleWrapper::GetRects) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  Handle<Array> rects = NanNew<Array>(obj->_this->num_rects);
  for (unsigned int i = 0; i < obj->_this->num_rects; i++) {
    if (obj->_this->rects[i]) {
      Handle<Value> v =
        AVSubtitleRectWrapper::newInstance(obj->_this->rects[i]);
      rects->Set(i, v);
    }
  }
  NanReturnValue(rects);
}

NAN_GETTER(FFmpeg::AVCodec::AVSubtitleWrapper::GetPts) {
  NanScope();
  AVSubtitleWrapper *obj = ObjectWrap::Unwrap<AVSubtitleWrapper>(args.This());
  int64_t pts = obj->_this->pts;
  NanReturnValue(NanNew<Number>(pts));
}
