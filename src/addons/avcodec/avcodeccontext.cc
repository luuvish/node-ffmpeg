#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avcodec/avsubtitle.h"
#include "avutil/avutil.h"
#include "avutil/avframe.h"

using namespace v8;

namespace ffmpeg {
namespace avcodec {
/*
AVDecodeAudioWorker::
AVDecodeAudioWorker(std::list<NanAsyncWorker*> *q,
                    ::AVCodecContext *ctx,
                    ::AVFrame *pic, ::AVPacket *pkt,
                    NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), frame(pic), packet(pkt) {
}

AVDecodeAudioWorker::~AVDecodeAudioWorker() {
}

void AVDecodeAudioWorker::Execute() {
  result = avcodec_decode_audio4(context, frame, &got_frame_ptr, packet);
}

void AVDecodeAudioWorker::HandleOKCallback() {
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


AVDecodeVideoWorker::
AVDecodeVideoWorker(std::list<NanAsyncWorker*> *q,
                    ::AVCodecContext *ctx,
                    ::AVFrame *pic, ::AVPacket *pkt,
                    NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), frame(pic), packet(pkt) {
}

AVDecodeVideoWorker::~AVDecodeVideoWorker() {
}

void AVDecodeVideoWorker::Execute() {
  result = avcodec_decode_video2(context, frame, &got_frame_ptr, packet);
}

void AVDecodeVideoWorker::HandleOKCallback() {
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


AVDecodeSubtitleWorker::
AVDecodeSubtitleWorker(std::list<NanAsyncWorker*> *q,
                       ::AVCodecContext *ctx,
                       ::AVSubtitle *sub, ::AVPacket *pkt,
                       NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), subtt(sub), packet(pkt) {
}

AVDecodeSubtitleWorker::~AVDecodeSubtitleWorker() {
}

void AVDecodeSubtitleWorker::Execute() {
  result = avcodec_decode_subtitle2(context, subtt, &got_subtt_ptr, packet);
}

void AVDecodeSubtitleWorker::HandleOKCallback() {
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
*/

Persistent<FunctionTemplate> AVCodecContext::constructor;

void AVCodecContext::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVCodecContext"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "alloc", AllocContext3);
  NODE_SET_PROTOTYPE_METHOD(tpl, "free", FreeContext);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getDefault", GetContextDefault3);
  NODE_SET_PROTOTYPE_METHOD(tpl, "copy", CopyContext);
  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getDefaultBuffer", DefaultGetBuffer2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decodeAudio", DecodeAudio4);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decodeVideo", DecodeVideo2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decodeSubtitle", DecodeSubtitle2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "encodeAudio", EncodeAudio2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "encodeVideo", EncodeVideo2);
  NODE_SET_PROTOTYPE_METHOD(tpl, "encodeSubtitle", EncodeSubtitle);
  NODE_SET_PROTOTYPE_METHOD(tpl, "flushBuffers", FlushBuffers);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getAudioFrameDuration",
                            GetAudioFrameDuration);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("is_open"), GetIsOpen);
  inst->SetAccessor(NanNew("codec_type"), GetCodecType, SetCodecType);
  inst->SetAccessor(NanNew("codec"), GetCodec);
  inst->SetAccessor(NanNew("codec_id"), GetCodecId, SetCodecId);
  inst->SetAccessor(NanNew("bit_rate"), GetBitRate, SetBitRate);
  inst->SetAccessor(NanNew("bit_rate_tolerance"),
                    GetBitRateTolerance, SetBitRateTolerance);
  inst->SetAccessor(NanNew("global_quality"),
                    GetGlobalQuality, SetGlobalQuality);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("flags2"), GetFlags2, SetFlags2);
  inst->SetAccessor(NanNew("extradata"), GetExtradata, SetExtradata);
  inst->SetAccessor(NanNew("time_base"), GetTimeBase, SetTimeBase);
  inst->SetAccessor(NanNew("delay"), GetDelay, SetDelay);
  inst->SetAccessor(NanNew("width"), GetWidth, SetWidth);
  inst->SetAccessor(NanNew("height"), GetHeight, SetHeight);
  inst->SetAccessor(NanNew("coded_width"), GetCodedWidth, SetCodedWidth);
  inst->SetAccessor(NanNew("coded_height"), GetCodedHeight, SetCodedHeight);
  inst->SetAccessor(NanNew("gop_size"), GetGopSize, SetGopSize);
  inst->SetAccessor(NanNew("pix_fmt"), GetPixFmt, SetPixFmt);
  inst->SetAccessor(NanNew("max_b_frames"), GetMaxBFrames, SetMaxBFrames);
  inst->SetAccessor(NanNew("has_b_frames"), GetHasBFrames);
  inst->SetAccessor(NanNew("sample_aspect_ratio"),
                    GetSampleAspectRatio, SetSampleAspectRatio);
  inst->SetAccessor(NanNew("mb_decision"), GetMbDecision, SetMbDecision);
  inst->SetAccessor(NanNew("intra_matrix"), GetIntraMatrix, SetIntraMatrix);
  inst->SetAccessor(NanNew("inter_matrix"), GetInterMatrix, SetInterMatrix);
  inst->SetAccessor(NanNew("refs"), GetRefs, SetRefs);
  inst->SetAccessor(NanNew("color_primaries"),
                    GetColorPrimaries, SetColorPrimaries);
  inst->SetAccessor(NanNew("color_trc"), GetColorTrc, SetColorTrc);
  inst->SetAccessor(NanNew("colorspace"), GetColorspace, SetColorspace);
  inst->SetAccessor(NanNew("color_range"), GetColorRange, SetColorRange);
  inst->SetAccessor(NanNew("chroma_sample_location"),
                    GetChromaSampleLocation, SetChromaSampleLocation);
  inst->SetAccessor(NanNew("field_order"), GetFieldOrder, SetFieldOrder);
  inst->SetAccessor(NanNew("sample_rate"), GetSampleRate, SetSampleRate);
  inst->SetAccessor(NanNew("channels"), GetChannels, SetChannels);
  inst->SetAccessor(NanNew("sample_fmt"), GetSampleFmt, SetSampleFmt);
  inst->SetAccessor(NanNew("frame_size"), GetFrameSize, SetFrameSize);
  inst->SetAccessor(NanNew("frame_number"), GetFrameNumber);
  inst->SetAccessor(NanNew("block_align"), GetBlockAlign, SetBlockAlign);
  inst->SetAccessor(NanNew("channel_layout"),
                    GetChannelLayout, SetChannelLayout);
  inst->SetAccessor(NanNew("request_channel_layout"),
                    GetRequestChannelLayout, SetRequestChannelLayout);
  inst->SetAccessor(NanNew("audio_service_type"),
                    GetAudioServiceType, SetAudioServiceType);
  inst->SetAccessor(NanNew("refcounted_frames"),
                    GetRefcountedFrames, SetRefcountedFrames);
  inst->SetAccessor(NanNew("q_min"), GetQmin, SetQmin);
  inst->SetAccessor(NanNew("q_max"), GetQmax, SetQmax);
  inst->SetAccessor(NanNew("rc_buffer_size"), GetRcBufferSize, SetRcBufferSize);
  inst->SetAccessor(NanNew("rc_max_rate"), GetRcMaxRate);
  inst->SetAccessor(NanNew("timecode_frame_start"),
                    GetTimecodeFrameStart, SetTimecodeFrameStart);
  inst->SetAccessor(NanNew("stats_out"), GetStatsOut);
  inst->SetAccessor(NanNew("stats_in"), GetStatsIn, SetStatsIn);
  inst->SetAccessor(NanNew("strict_std_compliance"),
                    GetStrictStdCompliance, SetStrictStdCompliance);
  inst->SetAccessor(NanNew("bits_per_coded_sample"),
                    GetBitsPerCodedSample, SetBitsPerCodedSample);
  inst->SetAccessor(NanNew("bits_per_raw_sample"),
                    GetBitsPerRawSample, SetBitsPerRawSample);
  inst->SetAccessor(NanNew("coded_frame"), GetCodedFrame);
  inst->SetAccessor(NanNew("thread_count"), GetThreadCount, SetThreadCount);
  inst->SetAccessor(NanNew("thread_type"), GetThreadType, SetThreadType);
  inst->SetAccessor(NanNew("thread_safe_callbacks"),
                    GetThreadSafeCallbacks, SetThreadSafeCallbacks);
  inst->SetAccessor(NanNew("profile"), GetProfile, SetProfile);
  inst->SetAccessor(NanNew("level"), GetLevel, SetLevel);
  inst->SetAccessor(NanNew("subtitle_header"),
                    GetSubtitleHeader, SetSubtitleHeader);
  inst->SetAccessor(NanNew("initial_padding"),
                    GetInitialPadding, SetInitialPadding);
  inst->SetAccessor(NanNew("pkt_timebase"), GetPktTimebase, SetPktTimebase);
  inst->SetAccessor(NanNew("lowres"), GetLowres, SetLowres);
  inst->SetAccessor(NanNew("pts_correction_num_faulty_pts"),
                    GetPtsCorrectionNumFaultyPts);
  inst->SetAccessor(NanNew("pts_correction_num_faulty_dts"),
                    GetPtsCorrectionNumFaultyDts);
  inst->SetAccessor(NanNew("pts_correction_last_pts"), GetPtsCorrectionLastPts);
  inst->SetAccessor(NanNew("pts_correction_last_dts"), GetPtsCorrectionLastDts);
  inst->SetAccessor(NanNew("sub_charenc"), GetSubCharenc, SetSubCharenc);
  inst->SetAccessor(NanNew("sub_charenc_mode"),
                    GetSubCharencMode, SetSubCharencMode);
  inst->SetAccessor(NanNew("skip_alpha"), GetSkipAlpha, SetSkipAlpha);
  inst->SetAccessor(NanNew("seek_preroll"), GetSeekPreroll, SetSeekPreroll);
  inst->SetAccessor(NanNew("chroma_intra_matrix"),
                    GetChromaIntraMatrix, SetChromaIntraMatrix);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVCodecContext"), tpl->GetFunction());
}

Local<Object> AVCodecContext::NewInstance(::AVCodecContext* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVCodecContext>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVCodecContext::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVCodecContext* AVCodecContext::This(::AVCodecContext* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

NAN_METHOD(AVCodecContext::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVCodecContext* obj = new AVCodecContext();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    NanReturnUndefined();
  }
}

NAN_METHOD(AVCodecContext::AllocContext3) {
  NanScope();

  AVCodecContext* obj = Unwrap<AVCodecContext>(args.This());

  ::AVCodec* codec = nullptr;
  if (AVCodec::HasInstance(args[0]))
    codec = Unwrap<AVCodec>(args[0]->ToObject())->This();

  obj->This(avcodec_alloc_context3(codec));
  NanReturnValue(args.This());
}

NAN_METHOD(AVCodecContext::FreeContext) {
  NanScope();

  AVCodecContext* obj = Unwrap<AVCodecContext>(args.This());

  ::AVCodecContext* wrap = obj->This();
  if (wrap) avcodec_free_context(&wrap);
  obj->this_ = nullptr;

  NanReturnValue(args.This());
}

NAN_METHOD(AVCodecContext::GetContextDefault3) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = nullptr;
  if (AVCodec::HasInstance(args[0]))
    codec = Unwrap<AVCodec>(args[0]->ToObject())->This();

  int ret = avcodec_get_context_defaults3(wrap, codec);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::CopyContext) {
  NanScope();

  if (!AVCodecContext::HasInstance(args[0]))
    return NanThrowTypeError("copy: AVCodecContext instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodecContext* src = Unwrap<AVCodecContext>(args[0]->ToObject())->This();

  int ret = avcodec_copy_context(wrap, src);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::Open2) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = nullptr;
  ::AVDictionary* options = nullptr;
  int argc = 0;

  if (AVCodec::HasInstance(args[argc]))
    codec = Unwrap<AVCodec>(args[argc++]->ToObject())->This();

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> opts = args[argc]->ToObject();
    Local<Array> keys = opts->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++) {
      Local<Value> key = keys->Get(i);
      Local<Value> val = opts->Get(key);
      if (val->IsNumber() || val->IsString()) {
        NanUtf8String key_str(key);
        NanUtf8String val_str(val);
        av_dict_set(&options, *key_str, *val_str, 0);
      }
    }
    argc++;
  }

  int ret = avcodec_open2(wrap, codec, &options);

  ::AVDictionaryEntry* t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
    av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
    ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::Close) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = avcodec_close(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::DefaultGetBuffer2) {
  NanScope();

  if (!avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("getDefaultBuffer: AVFrame instance required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("getDefaultBuffer: flags integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[0]->ToObject())->This();
  int flags = args[1]->Int32Value();
  int ret = avcodec_default_get_buffer2(wrap, frame, flags);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::DecodeAudio4) {
  NanScope();

  if (!avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("decodeAudio: AVFrame instance required");
  if (!AVPacket::HasInstance(args[1]))
    return NanThrowTypeError("decodeAudio: AVPacket instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[0]->ToObject())->This();
  int got_frame_ptr;
  const ::AVPacket* avpkt = Unwrap<AVPacket>(args[1]->ToObject())->This();

/*
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
*/

  int ret = avcodec_decode_audio4(wrap, frame, &got_frame_ptr, avpkt);
  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Boolean>(got_frame_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(AVCodecContext::DecodeVideo2) {
  NanScope();

  if (!avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("decodeVideo: AVFrame instance required");
  if (!AVPacket::HasInstance(args[1]))
    return NanThrowTypeError("decodeVideo: AVPacket instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* picture = Unwrap<avutil::AVFrame>(args[0]->ToObject())->This();
  int got_picture_ptr;
  const ::AVPacket* avpkt = Unwrap<AVPacket>(args[1]->ToObject())->This();

/*
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
*/

  int ret = avcodec_decode_video2(wrap, picture, &got_picture_ptr, avpkt);

  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Boolean>(got_picture_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(AVCodecContext::DecodeSubtitle2) {
  NanScope();

  if (!AVSubtitle::HasInstance(args[0]))
    return NanThrowTypeError("decodeSubtitle: AVSubtitle instance required");
  if (!AVPacket::HasInstance(args[1]))
    return NanThrowTypeError("decodeSubtitle: AVPacket instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVSubtitle* subtitle = Unwrap<AVSubtitle>(args[0]->ToObject())->This();
  int got_sub_ptr;
  ::AVPacket* avpkt = Unwrap<AVPacket>(args[1]->ToObject())->This();

/*
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
*/

  int ret = avcodec_decode_subtitle2(wrap, subtitle, &got_sub_ptr, avpkt);
  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Boolean>(got_sub_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(AVCodecContext::EncodeAudio2) {
  NanScope();

  if (!AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("encodeAudio: AVPacket instance required");
  if (!avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("encodeAudio: AVFrame instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* avpkt = Unwrap<AVPacket>(args[0]->ToObject())->This();
  const ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[1]->ToObject())->This();
  int got_packet_ptr;

  int ret = avcodec_encode_audio2(wrap, avpkt, frame, &got_packet_ptr);
  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Boolean>(got_packet_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(AVCodecContext::EncodeVideo2) {
  NanScope();

  if (!AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("encodeVideo: AVPacket instance required");
  if (!avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("encodeVideo: AVFrame instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* avpkt = Unwrap<AVPacket>(args[0]->ToObject())->This();
  const ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[1]->ToObject())->This();
  int got_packet_ptr;

  int ret = avcodec_encode_video2(wrap, avpkt, frame, &got_packet_ptr);
  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  rets->Set(1, NanNew<Boolean>(got_packet_ptr));
  NanReturnValue(rets);
}

NAN_METHOD(AVCodecContext::EncodeSubtitle) {
  NanScope();

  if (!args[0]->IsObject())
    NanThrowTypeError("encodeSubtitle: Uint8Array required");

  Local<Object> arr = args[0]->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("encodeSubtitle: Uint8Array required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  if (type != kExternalUint8Array)
    NanThrowTypeError("encodeSubtitle: Uint8Array required");

  if (!AVSubtitle::HasInstance(args[1]))
    return NanThrowTypeError("encodeSubtitle: AVSubtitle instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  void* data = arr->GetIndexedPropertiesExternalArrayData();
  uint8_t* buf = reinterpret_cast<uint8_t*>(data);
  int buf_size = arr->GetIndexedPropertiesExternalArrayDataLength();
  const ::AVSubtitle* sub = Unwrap<AVSubtitle>(args[1]->ToObject())->This();

  int ret = avcodec_encode_subtitle(wrap, buf, buf_size, sub);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVCodecContext::FlushBuffers) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap)
    avcodec_flush_buffers(wrap);
  NanReturnUndefined();
}

NAN_METHOD(AVCodecContext::GetAudioFrameDuration) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getAudioFrameDuration: frame_bytes integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int frame_bytes = args[0]->Int32Value();
  int ret = av_get_audio_frame_duration(wrap, frame_bytes);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_GETTER(AVCodecContext::GetIsOpen) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int is_open = avcodec_is_open(wrap);
  NanReturnValue(NanNew<Boolean>(is_open));
}

NAN_GETTER(AVCodecContext::GetCodecType) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVMediaType codec_type = wrap->codec_type;
  NanReturnValue(NanNew<Int32>(codec_type));
}

NAN_SETTER(AVCodecContext::SetCodecType) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("codec_type: AVMediaType enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->codec_type = static_cast<enum ::AVMediaType>(value->Int32Value());
}

NAN_GETTER(AVCodecContext::GetCodec) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  if (wrap->codec) {
    ::AVCodec* codec = const_cast<::AVCodec*>(wrap->codec);
    NanReturnValue(AVCodec::NewInstance(codec));
  } else
    NanReturnNull();
}

NAN_GETTER(AVCodecContext::GetCodecId) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID codec_id = wrap->codec_id;
  NanReturnValue(NanNew<Uint32>(codec_id));
}

NAN_SETTER(AVCodecContext::SetCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("codec_id: AVCodecID enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->codec_id = static_cast<enum ::AVCodecID>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetBitRate) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int bit_rate = wrap->bit_rate;
  NanReturnValue(NanNew<Int32>(bit_rate));
}

NAN_SETTER(AVCodecContext::SetBitRate) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("bit_rate: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->bit_rate = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetBitRateTolerance) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int bit_rate_tolerance = wrap->bit_rate_tolerance;
  NanReturnValue(NanNew<Int32>(bit_rate_tolerance));
}

NAN_SETTER(AVCodecContext::SetBitRateTolerance) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("bit_rate_tolerance: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->bit_rate_tolerance = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetGlobalQuality) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int global_quality = wrap->global_quality;
  NanReturnValue(NanNew<Int32>(global_quality));
}

NAN_SETTER(AVCodecContext::SetGlobalQuality) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("global_quality: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->global_quality = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetFlags) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVCodecContext::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetFlags2) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags2 = wrap->flags2;
  NanReturnValue(NanNew<Int32>(flags2));
}

NAN_SETTER(AVCodecContext::SetFlags2) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags2: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->flags2 = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetExtradata) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint8_t* extradata = wrap->extradata;
  ExternalArrayType type = kExternalUint8Array;
  int extradata_size = wrap->extradata_size;

  if (!extradata || extradata_size == 0)
    NanReturnNull();

  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(extradata, type, extradata_size);
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetExtradata) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("extradata: Uint8Array required");

  Local<Object> arr = value->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("extradata: Uint8Array required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  if (type != kExternalUint8Array)
    NanThrowTypeError("extradata: Uint8Array required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    void* data = arr->GetIndexedPropertiesExternalArrayData();
    wrap->extradata = reinterpret_cast<uint8_t*>(data);
    wrap->extradata_size = arr->GetIndexedPropertiesExternalArrayDataLength();
  }
}

NAN_GETTER(AVCodecContext::GetTimeBase) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational time_base = wrap->time_base;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(time_base.num));
  ret->Set(NanNew("den"), NanNew<Int32>(time_base.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetTimeBase) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("time_base: AVRational instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    Local<Object> time_base = value->ToObject();
    wrap->time_base.num = time_base->Get(NanNew("num"))->Int32Value();
    wrap->time_base.den = time_base->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVCodecContext::GetDelay) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int delay = wrap->delay;
  NanReturnValue(NanNew<Int32>(delay));
}

NAN_SETTER(AVCodecContext::SetDelay) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("delay: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->delay = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetWidth) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int width = wrap->width;
  NanReturnValue(NanNew<Int32>(width));
}

NAN_SETTER(AVCodecContext::SetWidth) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("width: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->width = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetHeight) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int height = wrap->height;
  NanReturnValue(NanNew<Int32>(height));
}

NAN_SETTER(AVCodecContext::SetHeight) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("height: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->height = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetCodedWidth) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int coded_width = wrap->coded_width;
  NanReturnValue(NanNew<Int32>(coded_width));
}

NAN_SETTER(AVCodecContext::SetCodedWidth) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("coded_width: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->coded_width = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetCodedHeight) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int coded_height = wrap->coded_height;
  NanReturnValue(NanNew<Int32>(coded_height));
}

NAN_SETTER(AVCodecContext::SetCodedHeight) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("coded_height: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->coded_height = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetGopSize) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int gop_size = wrap->gop_size;
  NanReturnValue(NanNew<Int32>(gop_size));
}

NAN_SETTER(AVCodecContext::SetGopSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("gop_size: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->gop_size = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetPixFmt) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVPixelFormat pix_fmt = wrap->pix_fmt;
  NanReturnValue(NanNew<Int32>(pix_fmt));
}

NAN_SETTER(AVCodecContext::SetPixFmt) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("pix_fmt: AVPixelFormat enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->pix_fmt = static_cast<enum ::AVPixelFormat>(value->Int32Value());
}

NAN_GETTER(AVCodecContext::GetMaxBFrames) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int max_b_frames = wrap->max_b_frames;
  NanReturnValue(NanNew<Int32>(max_b_frames));
}

NAN_SETTER(AVCodecContext::SetMaxBFrames) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_b_frames: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->max_b_frames = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetHasBFrames) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int has_b_frames = wrap->has_b_frames;
  NanReturnValue(NanNew<Boolean>(has_b_frames));
}

NAN_GETTER(AVCodecContext::GetSampleAspectRatio) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational sar = wrap->sample_aspect_ratio;
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(sar.num));
  ret->Set(NanNew("den"), NanNew<Int32>(sar.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetSampleAspectRatio) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("sample_aspect_ratio: AVRational instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    Local<Object> sar = value->ToObject();
    wrap->sample_aspect_ratio.num = sar->Get(NanNew("num"))->Int32Value();
    wrap->sample_aspect_ratio.den = sar->Get(NanNew("den"))->Int32Value();
  }
}

NAN_GETTER(AVCodecContext::GetMbDecision) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int mb_decision = wrap->mb_decision;
  NanReturnValue(NanNew<Int32>(mb_decision));
}

NAN_SETTER(AVCodecContext::SetMbDecision) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("mb_decision: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->mb_decision = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetIntraMatrix) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint16_t* intra_matrix = wrap->intra_matrix;
  ExternalArrayType type = kExternalUint16Array;
  int size = 64;

  if (!intra_matrix)
    NanReturnNull();

  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(intra_matrix, type, size);
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetIntraMatrix) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("intra_matrix: Uint16Array[64] required");

  Local<Object> arr = value->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("intra_matrix: Uint16Array[64] required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  int size = arr->GetIndexedPropertiesExternalArrayDataLength();
  if (type != kExternalUint16Array || size != 64)
    NanThrowTypeError("intra_matrix: Uint16Array[64] required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    void* data = arr->GetIndexedPropertiesExternalArrayData();
    wrap->intra_matrix = reinterpret_cast<uint16_t*>(data);
  }
}

NAN_GETTER(AVCodecContext::GetInterMatrix) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint16_t* inter_matrix = wrap->inter_matrix;
  ExternalArrayType type = kExternalUint16Array;
  int size = 64;

  if (!inter_matrix)
    NanReturnNull();

  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(inter_matrix, type, size);
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetInterMatrix) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("inter_matrix: Uint16Array[64] required");

  Local<Object> arr = value->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("inter_matrix: Uint16Array[64] required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  int size = arr->GetIndexedPropertiesExternalArrayDataLength();
  if (type != kExternalUint16Array || size != 64)
    NanThrowTypeError("inter_matrix: Uint16Array[64] required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    void* data = arr->GetIndexedPropertiesExternalArrayData();
    wrap->inter_matrix = reinterpret_cast<uint16_t*>(data);
  }
}

NAN_GETTER(AVCodecContext::GetRefs) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int refs = wrap->refs;
  NanReturnValue(NanNew<Int32>(refs));
}

NAN_SETTER(AVCodecContext::SetRefs) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("refs: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->refs = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetColorPrimaries) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorPrimaries color_primaries = wrap->color_primaries;
  NanReturnValue(NanNew<Uint32>(color_primaries));
}

NAN_SETTER(AVCodecContext::SetColorPrimaries) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("color_primaries: AVColorPrimaries enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->color_primaries =
      static_cast<enum ::AVColorPrimaries>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetColorTrc) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorTransferCharacteristic color_trc = wrap->color_trc;
  NanReturnValue(NanNew<Uint32>(color_trc));
}

NAN_SETTER(AVCodecContext::SetColorTrc) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("color_trc: AVColorTransferCharacteristic enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->color_trc =
      static_cast<enum ::AVColorTransferCharacteristic>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetColorspace) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorSpace colorspace = wrap->colorspace;
  NanReturnValue(NanNew<Uint32>(colorspace));
}

NAN_SETTER(AVCodecContext::SetColorspace) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("colorspace: AVColorSpace enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->colorspace = static_cast<enum ::AVColorSpace>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetColorRange) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVColorRange color_range = wrap->color_range;
  NanReturnValue(NanNew<Uint32>(color_range));
}

NAN_SETTER(AVCodecContext::SetColorRange) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("color_range: AVColorRange enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->color_range = static_cast<enum ::AVColorRange>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetChromaSampleLocation) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVChromaLocation csl = wrap->chroma_sample_location;
  NanReturnValue(NanNew<Uint32>(csl));
}

NAN_SETTER(AVCodecContext::SetChromaSampleLocation) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("chroma_sample_location: AVChromaLocation enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->chroma_sample_location =
      static_cast<enum ::AVChromaLocation>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetFieldOrder) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVFieldOrder field_order = wrap->field_order;
  NanReturnValue(NanNew<Uint32>(field_order));
}

NAN_SETTER(AVCodecContext::SetFieldOrder) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("field_order: AVFieldOrder enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->field_order =
      static_cast<enum ::AVFieldOrder>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetSampleRate) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int sample_rate = wrap->sample_rate;
  NanReturnValue(NanNew<Int32>(sample_rate));
}

NAN_SETTER(AVCodecContext::SetSampleRate) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("sample_rate: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->sample_rate = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetChannels) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int channels = wrap->channels;
  NanReturnValue(NanNew<Int32>(channels));
}

NAN_SETTER(AVCodecContext::SetChannels) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("channels: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->channels = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetSampleFmt) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVSampleFormat sample_fmt = wrap->sample_fmt;
  NanReturnValue(NanNew<Int32>(sample_fmt));
}

NAN_SETTER(AVCodecContext::SetSampleFmt) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("sample_fmt: AVSampleFormat enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->sample_fmt = static_cast<enum ::AVSampleFormat>(value->Int32Value());
}

NAN_GETTER(AVCodecContext::GetFrameSize) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int frame_size = wrap->frame_size;
  NanReturnValue(NanNew<Int32>(frame_size));
}

NAN_SETTER(AVCodecContext::SetFrameSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("frame_size: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->frame_size = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetFrameNumber) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int frame_number = wrap->frame_number;
  NanReturnValue(NanNew<Int32>(frame_number));
}

NAN_GETTER(AVCodecContext::GetBlockAlign) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int block_align = wrap->block_align;
  NanReturnValue(NanNew<Int32>(block_align));
}

NAN_SETTER(AVCodecContext::SetBlockAlign) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("block_align: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->block_align = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetChannelLayout) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint64_t channel_layout = wrap->channel_layout;
  NanReturnValue(NanNew<Number>(channel_layout));
}

NAN_SETTER(AVCodecContext::SetChannelLayout) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("channel_layout: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->channel_layout = value->IntegerValue();
}

NAN_GETTER(AVCodecContext::GetRequestChannelLayout) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint64_t request_channel_layout = wrap->request_channel_layout;
  NanReturnValue(NanNew<Number>(request_channel_layout));
}

NAN_SETTER(AVCodecContext::SetRequestChannelLayout) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("request_channel_layout: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->request_channel_layout = value->IntegerValue();
}

NAN_GETTER(AVCodecContext::GetAudioServiceType) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVAudioServiceType ast = wrap->audio_service_type;
  NanReturnValue(NanNew<Uint32>(ast));
}

NAN_SETTER(AVCodecContext::SetAudioServiceType) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("audio_service_type: AVAudioServiceType enum required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->audio_service_type =
      static_cast<enum ::AVAudioServiceType>(value->Uint32Value());
}

NAN_GETTER(AVCodecContext::GetRefcountedFrames) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int refcounted_frames = wrap->refcounted_frames;
  NanReturnValue(NanNew<Int32>(refcounted_frames));
}

NAN_SETTER(AVCodecContext::SetRefcountedFrames) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("refcounted_frames: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->refcounted_frames = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetQmin) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int qmin = wrap->qmin;
  NanReturnValue(NanNew<Int32>(qmin));
}

NAN_SETTER(AVCodecContext::SetQmin) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("qmin: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->qmin = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetQmax) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int qmax = wrap->qmax;
  NanReturnValue(NanNew<Int32>(qmax));
}

NAN_SETTER(AVCodecContext::SetQmax) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("qmax: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->qmax = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetRcBufferSize) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int rc_buffer_size = wrap->rc_buffer_size;
  NanReturnValue(NanNew<Int32>(rc_buffer_size));
}

NAN_SETTER(AVCodecContext::SetRcBufferSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("rc_buffer_size: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->rc_buffer_size = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetRcMaxRate) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int rc_max_rate = wrap->rc_max_rate;
  NanReturnValue(NanNew<Int32>(rc_max_rate));
}

NAN_GETTER(AVCodecContext::GetTimecodeFrameStart) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t timecode_frame_start = wrap->timecode_frame_start;
  NanReturnValue(NanNew<Number>(timecode_frame_start));
}

NAN_SETTER(AVCodecContext::SetTimecodeFrameStart) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("timecode_frame_start: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->timecode_frame_start = value->IntegerValue();
}

NAN_GETTER(AVCodecContext::GetStatsOut) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* stats_out = wrap->stats_out;
  if (stats_out)
    NanReturnValue(NanNew<String>(stats_out));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVCodecContext::GetStatsIn) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* stats_in = wrap->stats_in;
  if (stats_in)
    NanReturnValue(NanNew<String>(stats_in));
  else
    NanReturnEmptyString();
}

NAN_SETTER(AVCodecContext::SetStatsIn) {
  NanScope();

  if (!value->IsString())
    NanThrowTypeError("stats_in: string required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    NanUtf8String stats_in(value);
    if (wrap->stats_in)
      av_freep(&wrap->stats_in);
    wrap->stats_in = av_strdup(*stats_in);
  }
}

NAN_GETTER(AVCodecContext::GetStrictStdCompliance) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int strict_std_compliance = wrap->strict_std_compliance;
  NanReturnValue(NanNew<Int32>(strict_std_compliance));
}

NAN_SETTER(AVCodecContext::SetStrictStdCompliance) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("strict_std_compliance: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->strict_std_compliance = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetBitsPerCodedSample) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int bits_per_coded_sample = wrap->bits_per_coded_sample;
  NanReturnValue(NanNew<Int32>(bits_per_coded_sample));
}

NAN_SETTER(AVCodecContext::SetBitsPerCodedSample) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("bits_per_coded_sample: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->bits_per_coded_sample = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetBitsPerRawSample) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int bits_per_raw_sample = wrap->bits_per_raw_sample;
  NanReturnValue(NanNew<Int32>(bits_per_raw_sample));
}

NAN_SETTER(AVCodecContext::SetBitsPerRawSample) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("bits_per_raw_sample: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->bits_per_raw_sample = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetCodedFrame) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVFrame* frame = wrap->coded_frame;
  if (frame)
    NanReturnValue(avutil::AVFrame::NewInstance(frame));
  else
    NanReturnNull();
}

NAN_GETTER(AVCodecContext::GetThreadCount) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int thread_count = wrap->thread_count;
  NanReturnValue(NanNew<Int32>(thread_count));
}

NAN_SETTER(AVCodecContext::SetThreadCount) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("thread_count: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->thread_count = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetThreadType) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int thread_type = wrap->thread_type;
  NanReturnValue(NanNew<Int32>(thread_type));
}

NAN_SETTER(AVCodecContext::SetThreadType) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("thread_type: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->thread_type = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetThreadSafeCallbacks) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int thread_safe_callbacks = wrap->thread_safe_callbacks;
  NanReturnValue(NanNew<Int32>(thread_safe_callbacks));
}

NAN_SETTER(AVCodecContext::SetThreadSafeCallbacks) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("thread_safe_callbacks: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->thread_safe_callbacks = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetProfile) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int profile = wrap->profile;
  NanReturnValue(NanNew<Int32>(profile));
}

NAN_SETTER(AVCodecContext::SetProfile) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("profile: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->profile = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetLevel) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int level = wrap->level;
  NanReturnValue(NanNew<Int32>(level));
}

NAN_SETTER(AVCodecContext::SetLevel) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("level: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->level = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetSubtitleHeader) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint8_t* subtitle_header = wrap->subtitle_header;
  ExternalArrayType type = kExternalUint8Array;
  int subtitle_header_size = wrap->subtitle_header_size;

  if (!subtitle_header || subtitle_header_size == 0)
    NanReturnNull();

  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(
    subtitle_header, type, subtitle_header_size);
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetSubtitleHeader) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("subtitle_header: Uint8Array required");

  Local<Object> arr = value->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("subtitle_header: Uint8Array required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  if (type != kExternalUint8Array)
    NanThrowTypeError("subtitle_header: Uint8Array required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    void* data = arr->GetIndexedPropertiesExternalArrayData();
    int size = arr->GetIndexedPropertiesExternalArrayDataLength();
    wrap->subtitle_header = reinterpret_cast<uint8_t*>(data);
    wrap->subtitle_header_size = size;
  }
}

NAN_GETTER(AVCodecContext::GetInitialPadding) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int initial_padding = wrap->initial_padding;
  NanReturnValue(NanNew<Int32>(initial_padding));
}

NAN_SETTER(AVCodecContext::SetInitialPadding) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("initial_padding: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->initial_padding = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetPktTimebase) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVRational pkt_timebase = av_codec_get_pkt_timebase(wrap);
  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(pkt_timebase.num));
  ret->Set(NanNew("den"), NanNew<Int32>(pkt_timebase.den));
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetPktTimebase) {
  NanScope();

  if (!value->IsObject() ||
      !value->ToObject()->HasOwnProperty(NanNew("num")) ||
      !value->ToObject()->HasOwnProperty(NanNew("den")))
    NanThrowTypeError("pkt_timebase: AVRational instance required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    ::AVRational pkt_timebase = {
      .num = value->ToObject()->Get(NanNew("num"))->Int32Value(),
      .den = value->ToObject()->Get(NanNew("den"))->Int32Value()
    };
    av_codec_set_pkt_timebase(wrap, pkt_timebase);
  }
}

NAN_GETTER(AVCodecContext::GetLowres) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int lowres = av_codec_get_lowres(wrap);
  NanReturnValue(NanNew<Int32>(lowres));
}

NAN_SETTER(AVCodecContext::SetLowres) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    int lowres = value->Int32Value();
    av_codec_set_lowres(wrap, lowres);
  }
}

NAN_GETTER(AVCodecContext::GetPtsCorrectionNumFaultyPts) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts_correction_num_faulty_pts = wrap->pts_correction_num_faulty_pts;
  NanReturnValue(NanNew<Number>(pts_correction_num_faulty_pts));
}

NAN_GETTER(AVCodecContext::GetPtsCorrectionNumFaultyDts) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts_correction_num_faulty_dts = wrap->pts_correction_num_faulty_dts;
  NanReturnValue(NanNew<Number>(pts_correction_num_faulty_dts));
}

NAN_GETTER(AVCodecContext::GetPtsCorrectionLastPts) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts_correction_last_pts = wrap->pts_correction_last_pts;
  NanReturnValue(NanNew<Number>(pts_correction_last_pts));
}

NAN_GETTER(AVCodecContext::GetPtsCorrectionLastDts) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t pts_correction_last_dts = wrap->pts_correction_last_dts;
  NanReturnValue(NanNew<Number>(pts_correction_last_dts));
}

NAN_GETTER(AVCodecContext::GetSubCharenc) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* sub_charenc = wrap->sub_charenc;
  if (sub_charenc)
    NanReturnValue(NanNew<String>(sub_charenc));
  else
    NanReturnEmptyString();
}

NAN_SETTER(AVCodecContext::SetSubCharenc) {
  NanScope();

  if (!value->IsString())
    NanThrowTypeError("sub_charenc: string required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    NanUtf8String sub_charenc(value);
    if (wrap->sub_charenc)
      av_freep(&wrap->sub_charenc);
    wrap->sub_charenc = av_strdup(*sub_charenc);
  }
}

NAN_GETTER(AVCodecContext::GetSubCharencMode) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int sub_charenc_mode = wrap->sub_charenc_mode;
  NanReturnValue(NanNew<Int32>(sub_charenc_mode));
}

NAN_SETTER(AVCodecContext::SetSubCharencMode) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("sub_charenc_mode: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->sub_charenc_mode = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetSkipAlpha) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int skip_alpha = wrap->skip_alpha;
  NanReturnValue(NanNew<Int32>(skip_alpha));
}

NAN_SETTER(AVCodecContext::SetSkipAlpha) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("skip_alpha: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap)
    wrap->skip_alpha = value->Int32Value();
}

NAN_GETTER(AVCodecContext::GetSeekPreroll) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int seek_preroll = av_codec_get_seek_preroll(wrap);
  NanReturnValue(NanNew<Int32>(seek_preroll));
}

NAN_SETTER(AVCodecContext::SetSeekPreroll) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("seek_preroll: integer required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    int seek_preroll = value->Int32Value();
    av_codec_set_seek_preroll(wrap, seek_preroll);
  }
}

NAN_GETTER(AVCodecContext::GetChromaIntraMatrix) {
  NanScope();

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint16_t* chroma_intra_matrix = av_codec_get_chroma_intra_matrix(wrap);
  ExternalArrayType type = kExternalUint16Array;
  int size = 64;

  if (!chroma_intra_matrix)
    NanReturnNull();

  Local<Object> ret = NanNew<Object>();
  ret->SetIndexedPropertiesToExternalArrayData(chroma_intra_matrix, type, size);
  NanReturnValue(ret);
}

NAN_SETTER(AVCodecContext::SetChromaIntraMatrix) {
  NanScope();

  if (!value->IsObject())
    NanThrowTypeError("chroma_intra_matrix: Uint16Array[64] required");

  Local<Object> arr = value->ToObject();
  if (!arr->HasIndexedPropertiesInExternalArrayData())
    NanThrowTypeError("chroma_intra_matrix: Uint16Array[64] required");

  ExternalArrayType type = arr->GetIndexedPropertiesExternalArrayDataType();
  int size = arr->GetIndexedPropertiesExternalArrayDataLength();
  if (type != kExternalUint16Array || size != 64)
    NanThrowTypeError("chroma_intra_matrix: Uint16Array[64] required");

  ::AVCodecContext* wrap = Unwrap<AVCodecContext>(args.This())->This();

  if (wrap) {
    void* data = arr->GetIndexedPropertiesExternalArrayData();
    uint16_t* chroma_intra_matrix = reinterpret_cast<uint16_t*>(data);
    av_codec_set_chroma_intra_matrix(wrap, chroma_intra_matrix);
  }
}

}  // namespace avcodec
}  // namespace ffmpeg
