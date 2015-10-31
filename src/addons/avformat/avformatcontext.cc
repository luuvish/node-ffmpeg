extern "C" {
#include "libavutil/avstring.h"
}

#include "avformat/avformatcontext.h"
#include "avformat/avinputformat.h"
#include "avformat/avoutputformat.h"
#include "avformat/avstream.h"
#include "avformat/avprogram.h"
#include "avformat/avchapter.h"
#include "avcodec/avcodec.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"
#include "avutil/avframe.h"

using namespace v8;

namespace ffmpeg {
namespace avformat {

class AVReadFrameWorker : public NanAsyncWorker {
 public:
  explicit AVReadFrameWorker(
    NanCallback* callback, ::AVFormatContext* ctx, ::AVPacket* pkt)
    : NanAsyncWorker(callback), ctx_(ctx), pkt_(pkt) {}
  ~AVReadFrameWorker() {}

  void Execute() {
    ret_ = av_read_frame(ctx_, pkt_);
  }

  void HandleOKCallback() {
    NanScope();

    const unsigned argc = 3;
    Local<Value> argv[argc] = {
      NanNull(),
      NanNew<Int32>(ret_),
      avcodec::AVPacket::NewInstance(pkt_)
    };
    callback->Call(argc, argv);
  }

 private:
  ::AVFormatContext* ctx_;
  ::AVPacket* pkt_;
  int ret_;
};

class AVWriteFrameWorker : public NanAsyncWorker {
 public:
  explicit AVWriteFrameWorker(
    NanCallback* callback, ::AVFormatContext* ctx, ::AVPacket* pkt)
    : NanAsyncWorker(callback), ctx_(ctx), pkt_(pkt) {}
  ~AVWriteFrameWorker() {}

  void Execute() {
    ret_ = av_write_frame(ctx_, pkt_);
  }

  void HandleOKCallback() {
    NanScope();

    const unsigned argc = 3;
    Local<Value> argv[argc] = {
      NanNull(),
      NanNew<Int32>(ret_),
      avcodec::AVPacket::NewInstance(pkt_)
    };
    callback->Call(argc, argv);
  }

 private:
  ::AVFormatContext* ctx_;
  ::AVPacket* pkt_;
  int ret_;
};

class AVWriteInterleavedFrameWorker : public NanAsyncWorker {
 public:
  explicit AVWriteInterleavedFrameWorker(
    NanCallback* callback, ::AVFormatContext* ctx, ::AVPacket* pkt)
    : NanAsyncWorker(callback), ctx_(ctx), pkt_(pkt) {}
  ~AVWriteInterleavedFrameWorker() {}

  void Execute() {
    ret_ = av_interleaved_write_frame(ctx_, pkt_);
  }

  void HandleOKCallback() {
    NanScope();

    const unsigned argc = 3;
    Local<Value> argv[argc] = {
      NanNull(),
      NanNew<Int32>(ret_),
      avcodec::AVPacket::NewInstance(pkt_)
    };
    callback->Call(argc, argv);
  }

 private:
  ::AVFormatContext* ctx_;
  ::AVPacket* pkt_;
  int ret_;
};

class AVWriteUncodedFrameWorker : public NanAsyncWorker {
 public:
  explicit AVWriteUncodedFrameWorker(
    NanCallback* callback, ::AVFormatContext* ctx, int index, ::AVFrame* frame)
    : NanAsyncWorker(callback), ctx_(ctx), index_(index), frame_(frame) {}
  ~AVWriteUncodedFrameWorker() {}

  void Execute() {
    ret_ = av_write_uncoded_frame(ctx_, index_, frame_);
  }

  void HandleOKCallback() {
    NanScope();

    const unsigned argc = 4;
    Local<Value> argv[argc] = {
      NanNull(),
      NanNew<Int32>(ret_),
      NanNew<Int32>(index_),
      avutil::AVFrame::NewInstance(frame_)
    };
    callback->Call(argc, argv);
  }

 private:
  ::AVFormatContext* ctx_;
  int index_;
  ::AVFrame* frame_;
  int ret_;
};

class AVWriteInterleavedUncodedFrameWorker : public NanAsyncWorker {
 public:
  explicit AVWriteInterleavedUncodedFrameWorker(
    NanCallback* callback, ::AVFormatContext* ctx, int index, ::AVFrame* frame)
    : NanAsyncWorker(callback), ctx_(ctx), index_(index), frame_(frame) {}
  ~AVWriteInterleavedUncodedFrameWorker() {}

  void Execute() {
    ret_ = av_interleaved_write_uncoded_frame(ctx_, index_, frame_);
  }

  void HandleOKCallback() {
    NanScope();

    const unsigned argc = 4;
    Local<Value> argv[argc] = {
      NanNull(),
      NanNew<Int32>(ret_),
      NanNew<Int32>(index_),
      avutil::AVFrame::NewInstance(frame_)
    };
    callback->Call(argc, argv);
  }

 private:
  ::AVFormatContext* ctx_;
  int index_;
  ::AVFrame* frame_;
  int ret_;
};


Persistent<FunctionTemplate> AVFormatContext::constructor;

void AVFormatContext::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVFormatContext"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "newStream", NewStream);
  NODE_SET_PROTOTYPE_METHOD(tpl, "newProgram", NewProgram);
  NODE_SET_PROTOTYPE_METHOD(tpl, "openOutput", OpenOutput);
  NODE_SET_PROTOTYPE_METHOD(tpl, "openInput", OpenInput);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findStreamInfo", FindStreamInfo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findProgramFromStream",
                            FindProgramFromStream);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findBestStream", FindBestStream);
  NODE_SET_PROTOTYPE_METHOD(tpl, "readFrame", ReadFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seekFrame", SeekFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seekFile", SeekFile);
  NODE_SET_PROTOTYPE_METHOD(tpl, "flush", Flush);
  NODE_SET_PROTOTYPE_METHOD(tpl, "readPlay", ReadPlay);
  NODE_SET_PROTOTYPE_METHOD(tpl, "readPause", ReadPause);
  NODE_SET_PROTOTYPE_METHOD(tpl, "closeOutput", CloseOutput);
  NODE_SET_PROTOTYPE_METHOD(tpl, "closeInput", CloseInput);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeHeader", WriteHeader);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeFrame", WriteFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeInterleavedFrame",
                            WriteInterleavedFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeUncodedFrame", WriteUncodedFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeInterleavedUncodedFrame",
                            WriteInterleavedUncodedFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeUncodedFrameQuery",
                            WriteUncodedFrameQuery);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeTrailer", WriteTrailer);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getOutputTimestamp", GetOutputTimestamp);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findDefaultStreamIndex",
                            FindDefaultStreamIndex);
  NODE_SET_PROTOTYPE_METHOD(tpl, "dumpFormat", DumpFormat);
  NODE_SET_PROTOTYPE_METHOD(tpl, "guessSampleAspectRatio",
                            GuessSampleAspectRatio);
  NODE_SET_PROTOTYPE_METHOD(tpl, "guessFrameRate", GuessFrameRate);
  NODE_SET_PROTOTYPE_METHOD(tpl, "matchStreamSpecifier", MatchStreamSpecifier);
  NODE_SET_PROTOTYPE_METHOD(tpl, "queueAttachedPictures",
                            QueueAttachedPictures);
  NODE_SET_PROTOTYPE_METHOD(tpl, "injectGlobalSideData", InjectGlobalSideData);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  inst->SetAccessor(NanNew("iformat"), GetIFormat);
  inst->SetAccessor(NanNew("oformat"), GetOFormat, SetOFormat);
  inst->SetAccessor(NanNew("ctx_flags"), GetCtxFlags);
  inst->SetAccessor(NanNew("streams"), GetStreams);
  inst->SetAccessor(NanNew("filename"), GetFilename, SetFilename);
  inst->SetAccessor(NanNew("start_time"), GetStartTime);
  inst->SetAccessor(NanNew("duration"), GetDuration);
  inst->SetAccessor(NanNew("bit_rate"), GetBitRate);
  inst->SetAccessor(NanNew("packet_size"), GetPacketSize);
  inst->SetAccessor(NanNew("max_delay"), GetMaxDelay);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  inst->SetAccessor(NanNew("programs"), GetPrograms);
  inst->SetAccessor(NanNew("video_codec_id"), GetVideoCodecId, SetVideoCodecId);
  inst->SetAccessor(NanNew("audio_codec_id"), GetAudioCodecId, SetAudioCodecId);
  inst->SetAccessor(NanNew("subtitle_codec_id"),
                    GetSubtitleCodecId, SetSubtitleCodecId);
  inst->SetAccessor(NanNew("max_index_size"), GetMaxIndexSize, SetMaxIndexSize);
  inst->SetAccessor(NanNew("max_picture_buffer"), GetMaxPictureBuffer);
  inst->SetAccessor(NanNew("chapters"), GetChapters);
  inst->SetAccessor(NanNew("metadata"), GetMetadata);
  inst->SetAccessor(NanNew("start_time_realtime"),
                    GetStartTimeRealtime, SetStartTimeRealtime);
  inst->SetAccessor(NanNew("fps_probe_size"), GetFpsProbeSize, SetFpsProbeSize);
  inst->SetAccessor(NanNew("error_recognition"),
                    GetErrorRecognition, SetErrorRecognition);
  inst->SetAccessor(NanNew("max_interleave_delta"),
                    GetMaxInterleaveDelta, SetMaxInterleaveDelta);
  inst->SetAccessor(NanNew("strict_std_compliance"),
                    GetStrictStdCompliance, SetStrictStdCompliance);
  inst->SetAccessor(NanNew("event_flags"), GetEventFlags, SetEventFlags);
  inst->SetAccessor(NanNew("max_ts_probe"), GetMaxTsProbe, SetMaxTsProbe);
  inst->SetAccessor(NanNew("avoid_negative_ts"),
                    GetAvoidNegativeTs, SetAvoidNegativeTs);
  inst->SetAccessor(NanNew("audio_preload"), GetAudioPreload);
  inst->SetAccessor(NanNew("max_chunk_duration"), GetMaxChunkDuration);
  inst->SetAccessor(NanNew("max_chunk_size"), GetMaxChunkSize);
  inst->SetAccessor(NanNew("use_wallclock_as_timestamps"),
                    GetUseWallclockAsTimestamps);
  inst->SetAccessor(NanNew("avio_flags"), GetAvioFlags);
  inst->SetAccessor(NanNew("duration_estimation_method"),
                    GetDurationEstimationMethod);
  inst->SetAccessor(NanNew("skip_initial_bytes"), GetSkipInitialBytes);
  inst->SetAccessor(NanNew("correct_ts_overflow"), GetCorrectTsOverflow);
  inst->SetAccessor(NanNew("seek2any"), GetSeek2Any);
  inst->SetAccessor(NanNew("flush_packets"), GetFlushPackets);
  inst->SetAccessor(NanNew("probe_score"), GetProbeScore);
  inst->SetAccessor(NanNew("format_probesize"), GetFormatProbesize);
  inst->SetAccessor(NanNew("codec_whitelist"), GetCodecWhitelist);
  inst->SetAccessor(NanNew("format_whitelist"), GetFormatWhitelist);
  inst->SetAccessor(NanNew("io_repositioned"), GetIoRepositioned);
  inst->SetAccessor(NanNew("video_codec"), GetVideoCodec, SetVideoCodec);
  inst->SetAccessor(NanNew("audio_codec"), GetAudioCodec, SetAudioCodec);
  inst->SetAccessor(NanNew("subtitle_codec"),
                    GetSubtitleCodec, SetSubtitleCodec);
  inst->SetAccessor(NanNew("data_codec"), GetDataCodec, SetDataCodec);
  inst->SetAccessor(NanNew("metadata_header_padding"),
                    GetMetadataHeaderPadding);
  inst->SetAccessor(NanNew("output_ts_offset"), GetOutputTsOffset);
  inst->SetAccessor(NanNew("max_analyze_duration2"), GetMaxAnalyzeDuration2);
  inst->SetAccessor(NanNew("probesize2"), GetProbesize2);
  inst->SetAccessor(NanNew("dump_separator"), GetDumpSeparator);
  inst->SetAccessor(NanNew("data_codec_id"), GetDataCodecId, SetDataCodecId);

  NanAssignPersistent(constructor, tpl);

  exports->Set(NanNew("AVFormatContext"), tpl->GetFunction());
}

Local<Object> AVFormatContext::NewInstance(::AVFormatContext* wrap) {
  NanEscapableScope();

  Local<Function> cons = NanNew(constructor)->GetFunction();
  Local<Object> instance = cons->NewInstance(0, nullptr);
  ObjectWrap::Unwrap<AVFormatContext>(instance)->This(wrap);

  return NanEscapeScope(instance);
}

bool AVFormatContext::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

::AVFormatContext* AVFormatContext::This(::AVFormatContext* wrap) {
  if (wrap != nullptr) this_ = wrap;
  return this_;
}

AVFormatContext::AVFormatContext() : this_(nullptr) {
  this_ = avformat_alloc_context();
  if (this_ == nullptr)
    NanThrowTypeError("AVFormatContext: cannot allocation");
}

AVFormatContext::~AVFormatContext() {
  if (this_ != nullptr)
    avformat_free_context(this_);
}

NAN_METHOD(AVFormatContext::New) {
  NanScope();

  if (args.IsConstructCall()) {
    AVFormatContext* obj = new AVFormatContext();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    Local<Function> cons = NanNew(constructor)->GetFunction();
    NanReturnValue(cons->NewInstance(0, nullptr));
  }
}

NAN_METHOD(AVFormatContext::NewStream) {
  NanScope();

  if (!avcodec::AVCodec::HasInstance(args[0]))
    return NanThrowTypeError("newStream: AVCodec instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = Unwrap<avcodec::AVCodec>(args[0]->ToObject())->This();
  ::AVStream* stream = avformat_new_stream(wrap, codec);
  if (stream)
    NanReturnValue(AVStream::NewInstance(stream));
  else
    NanReturnNull();
}

NAN_METHOD(AVFormatContext::NewProgram) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("newProgram: id integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int id = args[0]->Int32Value();
  ::AVProgram* program = av_new_program(wrap, id);
  if (program)
    NanReturnValue(AVProgram::NewInstance(program));
  else
    NanReturnNull();
}

NAN_METHOD(AVFormatContext::OpenOutput) {
  NanScope();

  ::AVFormatContext* ctx = nullptr;
  ::AVOutputFormat* oformat = nullptr;
  NanUtf8String* format = nullptr;
  NanUtf8String* filename = nullptr;
  int argc = 0;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    if (AVOutputFormat::HasInstance(args[argc])) {
      oformat = Unwrap<AVOutputFormat>(args[argc]->ToObject())->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined()) {
    if (!args[argc]->IsNull() && !args[argc]->IsString())
      return NanThrowTypeError("openOutput: format string required");
    format = new NanUtf8String(args[argc]);
    argc++;
  }

  if (!args[argc]->IsUndefined()) {
    if (!args[argc]->IsNull() && !args[argc]->IsString())
      return NanThrowTypeError("openOutput: filename string required");
    filename = new NanUtf8String(args[argc]);
    argc++;
  }

  AVFormatContext* obj = Unwrap<AVFormatContext>(args.This());

  int ret = avformat_alloc_output_context2(
    &ctx, oformat,
    format != nullptr ? **format : nullptr,
    filename != nullptr ? **filename : nullptr);

  if (format) delete format;
  if (filename) delete filename;

  if (ret == 0) {
    ::AVFormatContext* wrap = obj->This();
    if (wrap) avformat_free_context(wrap);
    obj->This(ctx);
  }

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::OpenInput) {
  NanScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("openInput: filename string required");

  ::AVFormatContext* ctx = nullptr;
  NanUtf8String filename(args[0]);
  ::AVInputFormat* iformat = nullptr;
  ::AVDictionary* options = nullptr;
  int argc = 1;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    if (AVInputFormat::HasInstance(args[argc])) {
      iformat = Unwrap<AVInputFormat>(args[argc]->ToObject())->This();
      argc++;
    }
  }

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

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("openInput: invalid arguments");
  }

  AVFormatContext* obj = Unwrap<AVFormatContext>(args.This());

  int ret = avformat_open_input(&ctx, *filename, iformat, &options);

  ::AVDictionaryEntry* t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
    av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
    ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  // FIXME hack, ffplay maybe should not use url_feof() to test for the end
  if (ctx && ctx->pb)
      ctx->pb->eof_reached = 0;

  if (ret == 0) {
    ::AVFormatContext* wrap = obj->This();
    if (wrap) avformat_close_input(&wrap);
    obj->This(ctx);
  }

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::FindStreamInfo) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  if (wrap->nb_streams == 0)
    NanReturnNull();

  ::AVDictionary **opts = (::AVDictionary**)
    av_mallocz_array(wrap->nb_streams, sizeof(*opts));
  if (!opts)
    return NanThrowError("findStreamInfo: dictionary of streams memory alloaction error");
  for (uint32_t i = 0; i < wrap->nb_streams; i++)
    opts[i] = nullptr;
  int ret = avformat_find_stream_info(wrap, opts);

  Local<Array> infos = NanNew<Array>(wrap->nb_streams);
  for (uint32_t i = 0; i < wrap->nb_streams; i++) {
    Local<Object> info = NanNew<Object>();
    AVDictionaryEntry* t = nullptr;
    while ((t = av_dict_get(opts[i], "", t, AV_DICT_IGNORE_SUFFIX)))
      info->Set(NanNew<String>(t->key), NanNew<String>(t->value));
    infos->Set(i, info);
  }

  for (uint32_t i = 0; i < wrap->nb_streams; i++)
    av_dict_free(&opts[i]);
  av_freep(&opts);

  if (ret == 0)
    NanReturnValue(infos);
  else
    NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::FindProgramFromStream) {
  NanScope();

  ::AVProgram* last = nullptr;
  int argc = 0;

  if (AVProgram::HasInstance(args[argc])) {
    last = Unwrap<AVProgram>(args[argc]->ToObject())->This();
    argc++;
  }

  if (!args[argc]->IsNumber())
    return NanThrowTypeError("findProgramFromStream: stream_index integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int s = args[argc]->Int32Value();
  ::AVProgram* ret = av_find_program_from_stream(wrap, last, s);
  if (ret != nullptr)
    NanReturnValue(AVProgram::NewInstance(ret));
  else
    NanReturnNull();
}

NAN_METHOD(AVFormatContext::FindBestStream) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("findBestStream: media type enum required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("findBestStream: wanted_stream_nb integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("findBestStream: related_stream integer required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("findBestStream: flags integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[0]->Int32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  int flags = args[3]->Int32Value();

  ::AVCodec* decoder_ret = nullptr;
  int ret = av_find_best_stream(
    wrap, type, wanted_stream_nb, related_stream,
    &decoder_ret, flags);

  Local<Array> rets = NanNew<Array>(2);
  rets->Set(0, NanNew<Int32>(ret));
  if (ret >= 0)
    rets->Set(1, avcodec::AVCodec::NewInstance(decoder_ret));
  NanReturnValue(rets);
}

NAN_METHOD(AVFormatContext::ReadFrame) {
  NanScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("readFrame: AVPacket instance expected");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  if (args[1]->IsFunction()) {
    NanCallback* callback = new NanCallback(args[1].As<Function>());
    NanAsyncQueueWorker(new AVReadFrameWorker(callback, wrap, pkt));
    NanReturnUndefined();
  } else {
    int ret = av_read_frame(wrap, pkt);
    NanReturnValue(NanNew<Int32>(ret));
  }
}

NAN_METHOD(AVFormatContext::SeekFrame) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("seekFrame: stream_index integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("seekFrame: timestamp integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("seekFrame: flags integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = args[0]->Int32Value();
  int64_t timestamp = args[1]->IntegerValue();
  int flags = args[2]->Int32Value();

  int ret = av_seek_frame(wrap, stream_index, timestamp, flags);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::SeekFile) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("seekFile: stream_index integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("seekFile: min_ts integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("seekFile: ts integer required");
  if (!args[3]->IsNumber())
    return NanThrowTypeError("seekFile: max_ts integer required");
  if (!args[4]->IsNumber())
    return NanThrowTypeError("seekFile: flags integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = args[0]->Int32Value();
  int64_t min_ts = args[1]->IntegerValue();
  int64_t ts = args[2]->IntegerValue();
  int64_t max_ts = args[3]->IntegerValue();
  int flags = args[4]->Int32Value();

  int ret = avformat_seek_file(wrap, stream_index, min_ts, ts, max_ts, flags);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::Flush) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = avformat_flush(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::ReadPlay) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_read_play(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::ReadPause) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_read_pause(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::CloseOutput) {
  NanScope();

  AVFormatContext* obj = Unwrap<AVFormatContext>(args.This());

  ::AVFormatContext* wrap = obj->This();
  if (wrap) avformat_free_context(wrap);
  obj->this_ = nullptr;

  NanReturnValue(args.This());
}

NAN_METHOD(AVFormatContext::CloseInput) {
  NanScope();

  AVFormatContext* obj = Unwrap<AVFormatContext>(args.This());

  ::AVFormatContext* wrap = obj->This();
  if (wrap) avformat_close_input(&wrap);
  obj->this_ = nullptr;

  NanReturnValue(args.This());
}

NAN_METHOD(AVFormatContext::WriteHeader) {
  NanScope();

  ::AVDictionary* options = nullptr;

  if (!args[0]->IsUndefined() && args[0]->IsObject()) {
    Local<Object> opts = args[0]->ToObject();
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
  }

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = avformat_write_header(wrap, &options);

  ::AVDictionaryEntry* t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
    av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
    ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::WriteFrame) {
  NanScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("writeFrame: AVPacket instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  if (args[1]->IsFunction()) {
    NanCallback* callback = new NanCallback(args[1].As<Function>());
    NanAsyncQueueWorker(new AVWriteFrameWorker(callback, wrap, pkt));
    NanReturnUndefined();
  } else {
    int ret = av_write_frame(wrap, pkt);
    NanReturnValue(NanNew<Int32>(ret));
  }
}

NAN_METHOD(AVFormatContext::WriteInterleavedFrame) {
  NanScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("writeInterleavedFrame: AVPacket instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVPacket* pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  if (args[1]->IsFunction()) {
    NanCallback* callback = new NanCallback(args[1].As<Function>());
    NanAsyncQueueWorker(new AVWriteInterleavedFrameWorker(callback, wrap, pkt));
    NanReturnUndefined();
  } else {
    int ret = av_interleaved_write_frame(wrap, pkt);
    NanReturnValue(NanNew<Int32>(ret));
  }
}

NAN_METHOD(AVFormatContext::WriteUncodedFrame) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("writeUncodedFrame: stream_index integer required");
  if (!avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("writeUncodedFrame: AVFrame instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = args[0]->Int32Value();
  ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[1]->ToObject())->This();

  if (args[1]->IsFunction()) {
    NanCallback* callback = new NanCallback(args[1].As<Function>());
    AVWriteUncodedFrameWorker* worker =
      new AVWriteUncodedFrameWorker(callback, wrap, stream_index, frame);
    NanAsyncQueueWorker(worker);
    NanReturnUndefined();
  } else {
    int ret = av_write_uncoded_frame(wrap, stream_index, frame);
    NanReturnValue(NanNew<Int32>(ret));
  }
}

NAN_METHOD(AVFormatContext::WriteInterleavedUncodedFrame) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("writeInterleavedUncodedFrame: stream_index integer required");
  if (!avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("writeInterleavedUncodedFrame: AVFrame instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = args[0]->Int32Value();
  ::AVFrame* frame = Unwrap<avutil::AVFrame>(args[1]->ToObject())->This();

  if (args[1]->IsFunction()) {
    NanCallback* callback = new NanCallback(args[1].As<Function>());
    AVWriteInterleavedUncodedFrameWorker* worker =
      new AVWriteInterleavedUncodedFrameWorker(callback, wrap, stream_index, frame);
    NanAsyncQueueWorker(worker);
    NanReturnUndefined();
  } else {
    int ret = av_interleaved_write_uncoded_frame(wrap, stream_index, frame);
    NanReturnValue(NanNew<Int32>(ret));
  }
}

NAN_METHOD(AVFormatContext::WriteUncodedFrameQuery) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("writeUncodedFrameQuery: stream_index integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream_index = args[0]->Int32Value();
  int ret = av_write_uncoded_frame_query(wrap, stream_index);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::WriteTrailer) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_write_trailer(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::GetOutputTimestamp) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("getOutputTimestamp: stream integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int stream = args[0]->Int32Value();
  int64_t dts;
  int64_t wall;

  int ret = av_get_output_timestamp(wrap, stream, &dts, &wall);
  if (ret == 0) {
    Local<Object> rets = NanNew<Object>();
    rets->Set(NanNew("dts"), NanNew<Number>(dts));
    rets->Set(NanNew("wall"), NanNew<Number>(wall));
    NanReturnValue(rets);
  } else
    NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::FindDefaultStreamIndex) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = av_find_default_stream_index(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::DumpFormat) {
  NanScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("dumpFormat: index integer required");
  if (!args[1]->IsString())
    return NanThrowTypeError("dumpFormat: url string required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("dumpFormat: is_output integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int index = args[0]->Int32Value();
  NanUtf8String url(args[1]);
  int is_output = args[2]->Int32Value();

  if ((!is_output && wrap->iformat) || (is_output && wrap->oformat))
    av_dump_format(wrap, index, *url, is_output);
  NanReturnUndefined();
}

NAN_METHOD(AVFormatContext::GuessSampleAspectRatio) {
  NanScope();

  if (!AVStream::HasInstance(args[0]) && !avutil::AVFrame::HasInstance(args[0]))
    return NanThrowTypeError("guessSampleAspectRatio: AVStream instance required");
  if (!AVStream::HasInstance(args[0]) && !avutil::AVFrame::HasInstance(args[0]) &&
      !avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("guessSampleAspectRatio: AVFrame instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVStream* stream = nullptr;
  ::AVFrame* frame = nullptr;
  int argc = 0;

  if (AVStream::HasInstance(args[argc]))
    stream = Unwrap<AVStream>(args[argc++]->ToObject())->This();
  if (avutil::AVFrame::HasInstance(args[argc]))
    frame = Unwrap<avutil::AVFrame>(args[argc++]->ToObject())->This();

  ::AVRational sar = av_guess_sample_aspect_ratio(wrap, stream, frame);

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(sar.num));
  ret->Set(NanNew("den"), NanNew<Int32>(sar.den));
  NanReturnValue(ret);
}

NAN_METHOD(AVFormatContext::GuessFrameRate) {
  NanScope();

  if (!AVStream::HasInstance(args[0]))
    return NanThrowTypeError("guessFrameRate: AVStream instance required");
  if (!args[1]->IsUndefined() && !avutil::AVFrame::HasInstance(args[1]))
    return NanThrowTypeError("guessFrameRate: AVFrame instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVStream* stream = Unwrap<AVStream>(args[0]->ToObject())->This();
  ::AVFrame* frame = nullptr;
  if (avutil::AVFrame::HasInstance(args[1]))
    frame = Unwrap<avutil::AVFrame>(args[1]->ToObject())->This();

  ::AVRational frame_rate = av_guess_frame_rate(wrap, stream, frame);

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew("num"), NanNew<Int32>(frame_rate.num));
  ret->Set(NanNew("den"), NanNew<Int32>(frame_rate.den));
  NanReturnValue(ret);
}

NAN_METHOD(AVFormatContext::MatchStreamSpecifier) {
  NanScope();

  if (!AVStream::HasInstance(args[0]))
    return NanThrowTypeError("matchStreamSpecifier: AVStream instance required");
  if (!args[1]->IsString())
    return NanThrowTypeError("matchStreamSpecifier: spec string required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVStream* stream = Unwrap<AVStream>(args[0]->ToObject())->This();
  NanUtf8String spec(args[1]);

  int ret = avformat_match_stream_specifier(wrap, stream, *spec);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::QueueAttachedPictures) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ret = avformat_queue_attached_pictures(wrap);
  NanReturnValue(NanNew<Int32>(ret));
}

NAN_METHOD(AVFormatContext::InjectGlobalSideData) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap)
    av_format_inject_global_side_data(wrap);
  NanReturnUndefined();
}

NAN_GETTER(AVFormatContext::GetIFormat) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  if (wrap->iformat != nullptr)
    NanReturnValue(AVInputFormat::NewInstance(wrap->iformat));
  else
    NanReturnNull();
}

NAN_GETTER(AVFormatContext::GetOFormat) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  if (wrap->oformat != nullptr)
    NanReturnValue(AVOutputFormat::NewInstance(wrap->oformat));
  else
    NanReturnNull();
}

NAN_SETTER(AVFormatContext::SetOFormat) {
  NanScope();

  if (!AVOutputFormat::HasInstance(value))
    NanThrowTypeError("oformat: AVOutputFormat instance required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->oformat = Unwrap<AVOutputFormat>(value->ToObject())->This();
}

NAN_GETTER(AVFormatContext::GetCtxFlags) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int ctx_flags = wrap->ctx_flags;
  NanReturnValue(NanNew<Int32>(ctx_flags));
}

NAN_GETTER(AVFormatContext::GetStreams) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> streams = NanNew<Array>(wrap->nb_streams);
  for (uint32_t i = 0; i < wrap->nb_streams; i++) {
    if (wrap->streams[i]) {
      Local<Value> v = AVStream::NewInstance(wrap->streams[i]);
      streams->Set(i, v);
    }
  }

  NanReturnValue(streams);
}

NAN_GETTER(AVFormatContext::GetFilename) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  const char* filename = wrap->filename;
  if (filename)
    NanReturnValue(NanNew<String>(filename));
  else
    NanReturnEmptyString();
}

NAN_SETTER(AVFormatContext::SetFilename) {
  NanScope();

  if (!value->IsString())
    NanThrowTypeError("filename: string required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap) {
    NanUtf8String filename(value);
    av_strlcpy(wrap->filename, *filename, sizeof(wrap->filename));
  }
}

NAN_GETTER(AVFormatContext::GetStartTime) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t start_time = wrap->start_time;
  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(AVFormatContext::GetDuration) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t duration = wrap->duration;
  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(AVFormatContext::GetBitRate) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int bit_rate = wrap->bit_rate;
  NanReturnValue(NanNew<Int32>(bit_rate));
}

NAN_GETTER(AVFormatContext::GetPacketSize) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  unsigned int packet_size = wrap->packet_size;
  NanReturnValue(NanNew<Uint32>(packet_size));
}

NAN_GETTER(AVFormatContext::GetMaxDelay) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int max_delay = wrap->max_delay;
  NanReturnValue(NanNew<Int32>(max_delay));
}

NAN_GETTER(AVFormatContext::GetFlags) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flags = wrap->flags;
  NanReturnValue(NanNew<Int32>(flags));
}

NAN_SETTER(AVFormatContext::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->flags = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetPrograms) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> programs = NanNew<Array>(wrap->nb_programs);
  for (uint32_t i = 0; i < wrap->nb_programs; i++) {
    if (wrap->programs[i]) {
      Local<Value> v = AVProgram::NewInstance(wrap->programs[i]);
      programs->Set(i, v);
    }
  }

  NanReturnValue(programs);
}

NAN_GETTER(AVFormatContext::GetVideoCodecId) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID video_codec_id = wrap->video_codec_id;
  NanReturnValue(NanNew<Uint32>(video_codec_id));
}

NAN_SETTER(AVFormatContext::SetVideoCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("video_codec_id: enum required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->video_codec_id = static_cast<enum ::AVCodecID>(value->Uint32Value());
}

NAN_GETTER(AVFormatContext::GetAudioCodecId) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID audio_codec_id = wrap->audio_codec_id;
  NanReturnValue(NanNew<Uint32>(audio_codec_id));
}

NAN_SETTER(AVFormatContext::SetAudioCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("audio_codec_id: enum required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->audio_codec_id = static_cast<enum ::AVCodecID>(value->Uint32Value());
}

NAN_GETTER(AVFormatContext::GetSubtitleCodecId) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID subtitle_codec_id = wrap->subtitle_codec_id;
  NanReturnValue(NanNew<Uint32>(subtitle_codec_id));
}

NAN_SETTER(AVFormatContext::SetSubtitleCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("subtitle_codec_id: enum required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->subtitle_codec_id =
      static_cast<enum ::AVCodecID>(value->Uint32Value());
}

NAN_GETTER(AVFormatContext::GetMaxIndexSize) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  unsigned int max_index_size = wrap->max_index_size;
  NanReturnValue(NanNew<Uint32>(max_index_size));
}

NAN_SETTER(AVFormatContext::SetMaxIndexSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_index_size: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->max_index_size = value->Uint32Value();
}

NAN_GETTER(AVFormatContext::GetMaxPictureBuffer) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  unsigned int max_picture_buffer = wrap->max_picture_buffer;
  NanReturnValue(NanNew<Uint32>(max_picture_buffer));
}

NAN_GETTER(AVFormatContext::GetChapters) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Array> chapters = NanNew<Array>(wrap->nb_chapters);
  for (uint32_t i = 0; i < wrap->nb_chapters; i++) {
    if (wrap->chapters[i]) {
      Local<Value> v = AVChapter::NewInstance(wrap->chapters[i]);
      chapters->Set(i, v);
    }
  }

  NanReturnValue(chapters);
}

NAN_GETTER(AVFormatContext::GetMetadata) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  Local<Object> ret = NanNew<Object>();
  AVDictionary* metadata = wrap->metadata;
  AVDictionaryEntry* t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));
  NanReturnValue(ret);
}

NAN_GETTER(AVFormatContext::GetStartTimeRealtime) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t start_time_realtime = wrap->start_time_realtime;
  NanReturnValue(NanNew<Number>(start_time_realtime));
}

NAN_SETTER(AVFormatContext::SetStartTimeRealtime) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("start_time_realtime: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->start_time_realtime = value->IntegerValue();
}

NAN_GETTER(AVFormatContext::GetFpsProbeSize) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int fps_probe_size = wrap->fps_probe_size;
  NanReturnValue(NanNew<Int32>(fps_probe_size));
}

NAN_SETTER(AVFormatContext::SetFpsProbeSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("fps_probe_size: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->fps_probe_size = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetErrorRecognition) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int error_recognition = wrap->error_recognition;
  NanReturnValue(NanNew<Int32>(error_recognition));
}

NAN_SETTER(AVFormatContext::SetErrorRecognition) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("error_recognition: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->error_recognition = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetMaxInterleaveDelta) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t max_interleave_delta = wrap->max_interleave_delta;
  NanReturnValue(NanNew<Number>(max_interleave_delta));
}

NAN_SETTER(AVFormatContext::SetMaxInterleaveDelta) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_interleave_delta: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->max_interleave_delta = value->IntegerValue();
}

NAN_GETTER(AVFormatContext::GetStrictStdCompliance) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int strict_std_compliance = wrap->strict_std_compliance;
  NanReturnValue(NanNew<Int32>(strict_std_compliance));
}

NAN_SETTER(AVFormatContext::SetStrictStdCompliance) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("strict_std_compliance: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->strict_std_compliance = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetEventFlags) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int event_flags = wrap->event_flags;
  NanReturnValue(NanNew<Int32>(event_flags));
}

NAN_SETTER(AVFormatContext::SetEventFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("event_flags: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->event_flags = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetMaxTsProbe) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int max_ts_probe = wrap->max_ts_probe;
  NanReturnValue(NanNew<Int32>(max_ts_probe));
}

NAN_SETTER(AVFormatContext::SetMaxTsProbe) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_ts_probe: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->max_ts_probe = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetAvoidNegativeTs) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int avoid_negative_ts = wrap->avoid_negative_ts;
  NanReturnValue(NanNew<Int32>(avoid_negative_ts));
}

NAN_SETTER(AVFormatContext::SetAvoidNegativeTs) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("avoid_negative_ts: integer required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->avoid_negative_ts = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetAudioPreload) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int audio_preload = wrap->audio_preload;
  NanReturnValue(NanNew<Int32>(audio_preload));
}

NAN_GETTER(AVFormatContext::GetMaxChunkDuration) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int max_chunk_duration = wrap->max_chunk_duration;
  NanReturnValue(NanNew<Int32>(max_chunk_duration));
}

NAN_GETTER(AVFormatContext::GetMaxChunkSize) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int max_chunk_size = wrap->max_chunk_size;
  NanReturnValue(NanNew<Int32>(max_chunk_size));
}

NAN_GETTER(AVFormatContext::GetUseWallclockAsTimestamps) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int use_wallclock_as_timestamps = wrap->use_wallclock_as_timestamps;
  NanReturnValue(NanNew<Int32>(use_wallclock_as_timestamps));
}

NAN_GETTER(AVFormatContext::GetAvioFlags) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int avio_flags = wrap->avio_flags;
  NanReturnValue(NanNew<Int32>(avio_flags));
}

NAN_GETTER(AVFormatContext::GetDurationEstimationMethod) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVDurationEstimationMethod duration_estimation_method =
    av_fmt_ctx_get_duration_estimation_method(wrap);
  NanReturnValue(NanNew<Uint32>(duration_estimation_method));
}

NAN_GETTER(AVFormatContext::GetSkipInitialBytes) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t skip_initial_bytes = wrap->skip_initial_bytes;
  NanReturnValue(NanNew<Number>(skip_initial_bytes));
}

NAN_GETTER(AVFormatContext::GetCorrectTsOverflow) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  unsigned int correct_ts_overflow = wrap->correct_ts_overflow;
  NanReturnValue(NanNew<Uint32>(correct_ts_overflow));
}

NAN_GETTER(AVFormatContext::GetSeek2Any) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int seek2any = wrap->seek2any;
  NanReturnValue(NanNew<Int32>(seek2any));
}

NAN_GETTER(AVFormatContext::GetFlushPackets) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int flush_packets = wrap->flush_packets;
  NanReturnValue(NanNew<Int32>(flush_packets));
}

NAN_GETTER(AVFormatContext::GetProbeScore) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int probe_score = av_format_get_probe_score(wrap);
  NanReturnValue(NanNew<Int32>(probe_score));
}

NAN_GETTER(AVFormatContext::GetFormatProbesize) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int format_probesize = wrap->format_probesize;
  NanReturnValue(NanNew<Int32>(format_probesize));
}

NAN_GETTER(AVFormatContext::GetCodecWhitelist) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* codec_whitelist = wrap->codec_whitelist;
  if (codec_whitelist)
    NanReturnValue(NanNew<String>(codec_whitelist));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVFormatContext::GetFormatWhitelist) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  char* format_whitelist = wrap->format_whitelist;
  if (format_whitelist)
    NanReturnValue(NanNew<String>(format_whitelist));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVFormatContext::GetIoRepositioned) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int io_repositioned = wrap->io_repositioned;
  NanReturnValue(NanNew<Int32>(io_repositioned));
}

NAN_GETTER(AVFormatContext::GetVideoCodec) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = av_format_get_video_codec(wrap);
  if (codec != nullptr)
    NanReturnValue(avcodec::AVCodec::NewInstance(codec));
  else
    NanReturnNull();
}

NAN_SETTER(AVFormatContext::SetVideoCodec) {
  NanScope();

  if (!value->IsNull() && !avcodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec: AVCodec required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap) {
    ::AVCodec* codec = nullptr;
    if (avcodec::AVCodec::HasInstance(value))
      codec = Unwrap<avcodec::AVCodec>(value->ToObject())->This();
    av_format_set_video_codec(wrap, codec);
  }
}

NAN_GETTER(AVFormatContext::GetAudioCodec) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = av_format_get_audio_codec(wrap);
  if (codec != nullptr)
    NanReturnValue(avcodec::AVCodec::NewInstance(codec));
  else
    NanReturnNull();
}

NAN_SETTER(AVFormatContext::SetAudioCodec) {
  NanScope();

  if (!value->IsNull() && !avcodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec: AVCodec required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap) {
    ::AVCodec* codec = nullptr;
    if (avcodec::AVCodec::HasInstance(value))
      codec = Unwrap<avcodec::AVCodec>(value->ToObject())->This();
    av_format_set_audio_codec(wrap, codec);
  }
}

NAN_GETTER(AVFormatContext::GetSubtitleCodec) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = av_format_get_subtitle_codec(wrap);
  if (codec != nullptr)
    NanReturnValue(avcodec::AVCodec::NewInstance(codec));
  else
    NanReturnNull();
}

NAN_SETTER(AVFormatContext::SetSubtitleCodec) {
  NanScope();

  if (!value->IsNull() && !avcodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec: AVCodec required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap) {
    ::AVCodec *codec = nullptr;
    if (avcodec::AVCodec::HasInstance(value))
      codec = Unwrap<avcodec::AVCodec>(value->ToObject())->This();
    av_format_set_subtitle_codec(wrap, codec);
  }
}

NAN_GETTER(AVFormatContext::GetDataCodec) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  ::AVCodec* codec = av_format_get_data_codec(wrap);
  if (codec != nullptr)
    NanReturnValue(avcodec::AVCodec::NewInstance(codec));
  else
    NanReturnNull();
}

NAN_SETTER(AVFormatContext::SetDataCodec) {
  NanScope();

  if (!value->IsNull() && !avcodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec: AVCodec required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap) {
    ::AVCodec *codec = nullptr;
    if (avcodec::AVCodec::HasInstance(value))
      codec = Unwrap<avcodec::AVCodec>(value->ToObject())->This();
    av_format_set_data_codec(wrap, codec);
  }
}

NAN_GETTER(AVFormatContext::GetMetadataHeaderPadding) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int metadata_header_padding = wrap->metadata_header_padding;
  NanReturnValue(NanNew<Int32>(metadata_header_padding));
}

NAN_GETTER(AVFormatContext::GetOutputTsOffset) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t output_ts_offset = wrap->output_ts_offset;
  NanReturnValue(NanNew<Number>(output_ts_offset));
}

NAN_GETTER(AVFormatContext::GetMaxAnalyzeDuration2) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t max_analyze_duration2 = wrap->max_analyze_duration2;
  NanReturnValue(NanNew<Number>(max_analyze_duration2));
}

NAN_GETTER(AVFormatContext::GetProbesize2) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  int64_t probesize2 = wrap->probesize2;
  NanReturnValue(NanNew<Number>(probesize2));
}

NAN_GETTER(AVFormatContext::GetDumpSeparator) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  uint8_t* dump_separator = wrap->dump_separator;
  if (dump_separator)
    NanReturnValue(NanNew<String>(dump_separator));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVFormatContext::GetDataCodecId) {
  NanScope();

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();
  if (wrap == nullptr)
    NanReturnUndefined();

  enum ::AVCodecID data_codec_id = wrap->data_codec_id;
  NanReturnValue(NanNew<Uint32>(data_codec_id));
}

NAN_SETTER(AVFormatContext::SetDataCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("subtitle_codec_id: enum required");

  ::AVFormatContext* wrap = Unwrap<AVFormatContext>(args.This())->This();

  if (wrap)
    wrap->data_codec_id = static_cast<enum ::AVCodecID>(value->Uint32Value());
}

}  // namespace avformat
}  // namespace ffmpeg
