extern "C" {
#include "libavutil/avstring.h"
}

#include "avformat/avformatcontext.h"
#include "avformat/avinputformat.h"
#include "avformat/avoutputformat.h"
#include "avformat/avprogram.h"
#include "avformat/avchapter.h"
#include "avformat/avstream.h"
#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace v8;
using namespace ffmpeg;

namespace ffmpeg {
namespace avformat {

/*
AVReadFrameWorker::
AVReadFrameWorker(std::list<NanAsyncWorker*> *q,
                  ::AVFormatContext *ctx, ::AVPacket *pkt,
                  NanCallback *callback)
  : NanAsyncWorker(callback), queue(q), context(ctx), packet(pkt) {
}

AVReadFrameWorker::~AVReadFrameWorker() {
}

void AVReadFrameWorker::Execute() {
  result = av_read_frame(context, packet);
}

void AVReadFrameWorker::HandleOKCallback() {
  NanScope();

  queue->pop_front();
  if (queue->size() > 0)
      NanAsyncQueueWorker(queue->front());

  Handle<Value> pkt = AVCodec::AVPacketWrapper::newInstance(packet);

  const int argc = 2;
  Handle<Value> argv[argc] = { NanNew<Number>(result), pkt };
  callback->Call(argc, argv);
}
*/

Persistent<FunctionTemplate> AVFormatContext::constructor;

AVFormatContext::AVFormatContext(::AVFormatContext *ref)
  : this_(ref), alloc_(false) {
  if (this_ == nullptr) {
    this_ = avformat_alloc_context();
    alloc_ = true;
  }
}

AVFormatContext::~AVFormatContext() {
  if (this_ != nullptr && alloc_ == true) {
    avformat_free_context(this_);
  }
}

void AVFormatContext::Init(Handle<Object> exports) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("AVFormatContext"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "openInput", OpenInput);
  NODE_SET_PROTOTYPE_METHOD(tpl, "closeInput", CloseInput);
/*
  NODE_SET_PROTOTYPE_METHOD(tpl, "findStreamInfo", FindStreamInfo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findProgramFromStream",
                            FindProgramFromStream);
  NODE_SET_PROTOTYPE_METHOD(tpl, "findBestStream", FindBestStream);
*/
  NODE_SET_PROTOTYPE_METHOD(tpl, "readFrame", ReadFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seekFrame", SeekFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "seekFile", SeekFile);
  NODE_SET_PROTOTYPE_METHOD(tpl, "readPlay", ReadPlay);
  NODE_SET_PROTOTYPE_METHOD(tpl, "readPause", ReadPause);

  //NODE_SET_PROTOTYPE_METHOD(tpl, "writeHeader", WriteHeader);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeFrame", WriteFrame);
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeInterleavedFrame",
                            WriteInterleavedFrame);
  //
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeTrailer", WriteTrailer);
  NODE_SET_PROTOTYPE_METHOD(tpl, "dumpFormat", DumpFormat);
/*
  NODE_SET_PROTOTYPE_METHOD(tpl, "guessSampleAspectRatio",
                            GuessSampleAspectRatio);
  NODE_SET_PROTOTYPE_METHOD(tpl, "guessFrameRate", GuessFrameRate);
*/

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();

  //inst->SetAccessor(NanNew("iformat"), GetIFormat);
  //inst->SetAccessor(NanNew("oformat"), GetOFormat, SetOFormat);
  inst->SetAccessor(NanNew("ctx_flags"), GetCtxFlags);
  //inst->SetAccessor(NanNew("streams"), GetStreams);
  inst->SetAccessor(NanNew("filename"), GetFilename, SetFilename);
  inst->SetAccessor(NanNew("start_time"), GetStartTime);
  inst->SetAccessor(NanNew("duration"), GetDuration);
  inst->SetAccessor(NanNew("bit_rate"), GetBitRate);
  inst->SetAccessor(NanNew("packet_size"), GetPacketSize);
  inst->SetAccessor(NanNew("max_delay"), GetMaxDelay);
  inst->SetAccessor(NanNew("flags"), GetFlags, SetFlags);
  //inst->SetAccessor(NanNew("programs"), GetPrograms);
  inst->SetAccessor(NanNew("video_codec_id"), GetVideoCodecId, SetVideoCodecId);
  inst->SetAccessor(NanNew("audio_codec_id"), GetAudioCodecId, SetAudioCodecId);
  inst->SetAccessor(NanNew("subtitle_codec_id"),
                    GetSubtitleCodecId, SetSubtitleCodecId);
  inst->SetAccessor(NanNew("max_index_size"), GetMaxIndexSize, SetMaxIndexSize);
  inst->SetAccessor(NanNew("max_picture_buffer"), GetMaxPictureBuffer);
  //inst->SetAccessor(NanNew("chapters"), GetChapters);
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
  inst->SetAccessor(NanNew("data_offset"), GetDataOffset);
  inst->SetAccessor(NanNew("raw_packet_buffer_remaining_size"),
                    GetRawPacketBufferRemainingSize);
  inst->SetAccessor(NanNew("offset"), GetOffset);
  inst->SetAccessor(NanNew("offset_timebase"), GetOffsetTimebase);
  inst->SetAccessor(NanNew("io_repositioned"), GetIoRepositioned);
  //inst->SetAccessor(NanNew("video_codec"), GetVideoCodec, SetVideoCodec);
  //inst->SetAccessor(NanNew("audio_codec"), GetAudioCodec, SetAudioCodec);
  //inst->SetAccessor(NanNew("subtitle_codec"),
  //                  GetSubtitleCodec, SetSubtitleCodec);
  inst->SetAccessor(NanNew("metadata_header_padding"),
                    GetMetadataHeaderPadding);
  inst->SetAccessor(NanNew("output_ts_offset"), GetOutputTsOffset);
  inst->SetAccessor(NanNew("max_analyze_duration2"), GetMaxAnalyzeDuration2);
  inst->SetAccessor(NanNew("probesize2"), GetProbesize2);
  inst->SetAccessor(NanNew("dump_separator"), GetDumpSeparator);

  NanAssignPersistent(constructor, tpl);
  exports->Set(NanNew("AVFormatContext"), tpl->GetFunction());
}

Local<Object> AVFormatContext::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const int argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> ctor = constructor->GetFunction();
  Local<Object> instance = ctor->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

bool AVFormatContext::HasInstance(Handle<Value> value) {
  if (!value->IsObject()) return false;
  Local<Object> obj = value->ToObject();
  return NanHasInstance(constructor, obj);
}

NAN_METHOD(AVFormatContext::New) {
  NanEscapableScope();

  if (args.IsConstructCall()) {
    ::AVFormatContext *ref = nullptr;
    if (args[0]->IsExternal())
      ref = static_cast<::AVFormatContext *>(External::Unwrap(args[0]));
    AVFormatContext *obj = new AVFormatContext(ref);
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> ctor = constructor->GetFunction();
    NanReturnValue(ctor->NewInstance(argc, argv));
  }
}

NAN_METHOD(AVFormatContext::OpenInput) {
  NanEscapableScope();

  if (!args[0]->IsString())
    return NanThrowTypeError("openInput: filename string required");

  String::Utf8Value filename(args[0]);
  ::AVInputFormat *iformat = nullptr;
  ::AVDictionary *options = nullptr;
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
        String::Utf8Value key_str(key);
        String::Utf8Value val_str(val);
        av_dict_set(&options, *key_str, *val_str, 0);
      }
    }
    argc++;
  }

  if (argc != args.Length()) {
    av_dict_free(&options);
    return NanThrowTypeError("openInput: invalid arguments");
  }

  AVFormatContext *obj = Unwrap<AVFormatContext>(args.This());

  int ret = avformat_open_input(&obj->this_, *filename, iformat, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  // FIXME hack, ffplay maybe should not use url_feof() to test for the end
  if (obj->this_ && obj->this_->pb)
      obj->this_->pb->eof_reached = 0;

  if (!obj->this_)
    obj->this_ = avformat_alloc_context();

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::CloseInput) {
  NanEscapableScope();

  AVFormatContext *obj = Unwrap<AVFormatContext>(args.This());

  avformat_close_input(&obj->this_);

  if (!obj->this_)
    obj->this_ = avformat_alloc_context();

  NanReturnUndefined();
}

/*
NAN_METHOD(AVFormatContext::FindStreamInfo) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();

  if (!ref->nb_streams)
    NanReturnNull();

  ::AVDictionary **opts =
    (::AVDictionary **)av_mallocz(ref->nb_streams * sizeof(::AVDictionary *));
  if (!opts)
    return NanThrowError("findStreamInfo: dictionary of streams memory alloaction error");

  int ret = avformat_find_stream_info(ref, opts);

  for (uint32_t i = 0; i < ref->nb_streams; i++)
    av_dict_free(&opts[i]);
  av_freep(&opts);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::FindProgramFromStream) {
  NanEscapableScope();

  ::AVProgram *last = nullptr;
  if (args[0]->IsObject()) {
    if (!AVProgram::HasInstance(args[0]))
      return NanThrowTypeError("findProgramFromStream: program required");
    last = Unwrap<AVProgram>(args[0]->ToObject())->This();
  }
  if (!args[1]->IsNumber())
    return NanThrowTypeError("stream_index required");
  int stream_index = args[1]->Int32Value();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();

  ::AVProgram *program = av_find_program_from_stream(ref, last, stream_index);
  if (!program)
    NanReturnNull();
  Local<Value> ret = AVProgram::NewInstance(program);

  NanReturnValue(ret);
}

NAN_METHOD(AVFormatContext::FindBestStream) {
  NanEscapableScope();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() ||
      !args[2]->IsNumber() || !args[3]->IsNumber())
    return NanThrowTypeError("media type, wanted_stream_nb, "
                             "related_stream, flags required");

  enum ::AVMediaType type =
    static_cast<enum ::AVMediaType>(args[0]->Uint32Value());
  int wanted_stream_nb = args[1]->Int32Value();
  int related_stream = args[2]->Int32Value();
  int flags = args[3]->Int32Value();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();

  ::AVCodec *decoder_ret = nullptr;
  int ret = av_find_best_stream(ref,
                                type, wanted_stream_nb, related_stream,
                                &decoder_ret, flags);

  NanReturnValue(NanNew<Integer>(ret));
}
*/
NAN_METHOD(AVFormatContext::ReadFrame) {
  NanEscapableScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("readFrame: AVPacket instance expected");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVPacket *pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  if (args[1]->IsFunction()) {
    //NanCallback *callback = new NanCallback(Local<Function>::Cast(args[1]));
    //AVReadFrameWorker *worker =
    //  new AVReadFrameWorker(&obj->_async_queue, ref, pkt, callback);
    //obj->_async_queue.push_back(worker);
    //if (obj->_async_queue.size() == 1)
    //  NanAsyncQueueWorker(obj->_async_queue.front());
    NanReturnUndefined();
  } else {
    int ret = av_read_frame(ref, pkt);

    NanReturnValue(NanNew<Integer>(ret));
  }
}

NAN_METHOD(AVFormatContext::SeekFrame) {
  NanEscapableScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("seekFrame: stream_index integer required");
  if (!args[1]->IsNumber())
    return NanThrowTypeError("seekFrame: timestamp integer required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("seekFrame: flags integer required");

  int stream_index = args[0]->Int32Value();
  int64_t timestamp = args[1]->IntegerValue();
  int flags = args[2]->Int32Value();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = av_seek_frame(ref, stream_index, timestamp, flags);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::SeekFile) {
  NanEscapableScope();

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

  int stream_index = args[0]->Int32Value();
  int64_t min_ts = args[1]->IntegerValue();
  int64_t ts = args[2]->IntegerValue();
  int64_t max_ts = args[3]->IntegerValue();
  int flags = args[4]->Int32Value();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = avformat_seek_file(ref, stream_index, min_ts, ts, max_ts, flags);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::ReadPlay) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = av_read_play(ref);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::ReadPause) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = av_read_pause(ref);

  NanReturnValue(NanNew<Integer>(ret));
}
/*
NAN_METHOD(AVFormatContext::WriteHeader) {
  NanEscapableScope();

  ::AVDictionary *options = nullptr;

  if (!args[0]->IsUndefined() && args[0]->IsObject()) {
    Local<Object> opts = args[0]->ToObject();
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
  }

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = avformat_write_header(ref, &options);

  ::AVDictionaryEntry *t = nullptr;
  if ((t = av_dict_get(options, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
      av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
      ret = AVERROR_OPTION_NOT_FOUND;
  }
  av_dict_free(&options);

  NanReturnValue(NanNew<Integer>(ret));
}
*/
NAN_METHOD(AVFormatContext::WriteFrame) {
  NanEscapableScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("writeFrame: AVPacket instance expected");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVPacket *pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  int ret = av_write_frame(ref, pkt);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::WriteInterleavedFrame) {
  NanEscapableScope();

  if (!avcodec::AVPacket::HasInstance(args[0]))
    return NanThrowTypeError("writeInterleavedFrame: AVPacket instance expected");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVPacket *pkt = Unwrap<avcodec::AVPacket>(args[0]->ToObject())->This();

  int ret = av_interleaved_write_frame(ref, pkt);

  NanReturnValue(NanNew<Integer>(ret));
}
/*



 */
NAN_METHOD(AVFormatContext::WriteTrailer) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ret = av_write_trailer(ref);

  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(AVFormatContext::DumpFormat) {
  NanEscapableScope();

  if (!args[0]->IsNumber())
    return NanThrowTypeError("dumpFormat: index integer required");
  if (!args[1]->IsString())
    return NanThrowTypeError("dumpFormat: url string required");
  if (!args[2]->IsNumber())
    return NanThrowTypeError("dumpFormat: is_output integer required");

  int index = args[0]->Int32Value();
  String::Utf8Value url(args[1]);
  int is_output = args[2]->Int32Value();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  if ((!is_output && ref->iformat) || (is_output && ref->oformat))
    av_dump_format(ref, index, *url, is_output);

  NanReturnUndefined();
}
/*
NAN_METHOD(AVFormatContext::GuessSampleAspectRatio) {
  NanEscapableScope();

  ::AVStream *stream = nullptr;
  ::AVFrame *frame = nullptr;
  int argc = 0;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg0 = args[argc]->ToObject();
    if (AVStream::HasInstance(arg0)) {
      stream = Unwrap<AVStream>(arg0)->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg1 = args[argc]->ToObject();
    if (AVUtil::AVFrame::HasInstance(arg1)) {
      frame = Unwrap<AVUtil::AVFrame>(arg1)->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVRational sample_aspect_ratio =
    av_guess_sample_aspect_ratio(ref, stream, frame);

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Integer>(sample_aspect_ratio.num));
  ret->Set(NanNew<String>("den"), NanNew<Integer>(sample_aspect_ratio.den));

  NanReturnValue(ret);
}

NAN_METHOD(AVFormatContext::GuessFrameRate) {
  NanEscapableScope();

  ::AVStream *stream = nullptr;
  ::AVFrame *frame = nullptr;
  int argc = 0;

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg0 = args[argc]->ToObject();
    if (AVStream::HasInstance(arg0)) {
      stream = Unwrap<AVStream>(arg0)->This();
      argc++;
    }
  }

  if (!args[argc]->IsUndefined() && args[argc]->IsObject()) {
    Local<Object> arg1 = args[argc]->ToObject();
    if (AVUtil::AVFrame::HasInstance(arg1)) {
      frame = Unwrap<AVUtil::AVFrame>(arg1)->This();
      argc++;
    }
  }

  if (argc != args.Length())
    return NanThrowTypeError("invalid arguments");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVRational frame_rate = av_guess_frame_rate(ref, stream, frame);

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Integer>(frame_rate.num));
  ret->Set(NanNew<String>("den"), NanNew<Integer>(frame_rate.den));

  NanReturnValue(ret);
}

NAN_GETTER(AVFormatContext::GetIFormat) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  if (!ref->iformat)
    NanReturnNull();
  Local<Value> ret = AVInputFormat::newInstance(ref->iformat);

  NanReturnValue(ret);
}

NAN_GETTER(AVFormatContext::GetOFormat) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  if (!ref->oformat)
    NanReturnNull();

  Local<Value> ret = AVOutputFormat::NewInstance(ref->oformat);
  NanReturnValue(ret);
}
*/
NAN_GETTER(AVFormatContext::GetCtxFlags) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int ctx_flags = ref->ctx_flags;

  NanReturnValue(NanNew<Integer>(ctx_flags));
}
/*
NAN_GETTER(AVFormatContext::GetStreams) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  Local<Array> streams = NanNew<Array>(ref->nb_streams);
  for (uint32_t i = 0; i < ref->nb_streams; i++) {
    if (ref->streams[i]) {
      Local<Value> v = AVStream::NewInstance(ref->streams[i]);
      streams->Set(i, v);
    }
  }

  NanReturnValue(streams);
}
*/
NAN_GETTER(AVFormatContext::GetFilename) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  const char *filename = ref->filename;

  if (filename)
    NanReturnValue(NanNew<String>(filename));
  else
    NanReturnEmptyString();
}

NAN_SETTER(AVFormatContext::SetFilename) {
  NanScope();

  if (!value->IsString())
    NanThrowTypeError("filename: string required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  String::Utf8Value filename(value);
  av_strlcpy(ref->filename, *filename, sizeof(ref->filename));
}

NAN_GETTER(AVFormatContext::GetStartTime) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t start_time = ref->start_time;

  NanReturnValue(NanNew<Number>(start_time));
}

NAN_GETTER(AVFormatContext::GetDuration) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t duration = ref->duration;

  NanReturnValue(NanNew<Number>(duration));
}

NAN_GETTER(AVFormatContext::GetBitRate) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int bit_rate = ref->bit_rate;

  NanReturnValue(NanNew<Integer>(bit_rate));
}

NAN_GETTER(AVFormatContext::GetPacketSize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  unsigned int packet_size = ref->packet_size;

  NanReturnValue(NanNew<Integer>(packet_size));
}

NAN_GETTER(AVFormatContext::GetMaxDelay) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int max_delay = ref->max_delay;

  NanReturnValue(NanNew<Integer>(max_delay));
}

NAN_GETTER(AVFormatContext::GetFlags) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int flags = ref->flags;

  NanReturnValue(NanNew<Integer>(flags));
}

NAN_SETTER(AVFormatContext::SetFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("flags: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->flags = value->Int32Value();
}

/*
NAN_GETTER(AVFormatContext::GetPrograms) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  Local<Array> programs = NanNew<Array>(ref->nb_programs);
  for (uint32_t i = 0; i < ref->nb_programs; i++) {
    if (ref->programs[i]) {
      Local<Value> v = AVProgram::NewInstance(ref->programs[i]);
      programs->Set(i, v);
    }
  }

  NanReturnValue(programs);
}
*/

NAN_GETTER(AVFormatContext::GetVideoCodecId) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  enum ::AVCodecID video_codec_id = ref->video_codec_id;

  NanReturnValue(NanNew<Integer>(video_codec_id));
}

NAN_SETTER(AVFormatContext::SetVideoCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("video_codec_id: enum required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->video_codec_id = static_cast<enum ::AVCodecID>(value->Int32Value());
}

NAN_GETTER(AVFormatContext::GetAudioCodecId) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  enum ::AVCodecID audio_codec_id = ref->audio_codec_id;

  NanReturnValue(NanNew<Integer>(audio_codec_id));
}

NAN_SETTER(AVFormatContext::SetAudioCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("audio_codec_id: enum required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->audio_codec_id = static_cast<enum ::AVCodecID>(value->Int32Value());
}

NAN_GETTER(AVFormatContext::GetSubtitleCodecId) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  enum ::AVCodecID subtitle_codec_id = ref->subtitle_codec_id;

  NanReturnValue(NanNew<Integer>(subtitle_codec_id));
}

NAN_SETTER(AVFormatContext::SetSubtitleCodecId) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("subtitle_codec_id: enum required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->subtitle_codec_id = static_cast<enum ::AVCodecID>(value->Int32Value());
}

NAN_GETTER(AVFormatContext::GetMaxIndexSize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  unsigned int max_index_size = ref->max_index_size;

  NanReturnValue(NanNew<Integer>(max_index_size));
}

NAN_SETTER(AVFormatContext::SetMaxIndexSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_index_size: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->max_index_size = value->Uint32Value();
}

NAN_GETTER(AVFormatContext::GetMaxPictureBuffer) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  unsigned int max_picture_buffer = ref->max_picture_buffer;

  NanReturnValue(NanNew<Integer>(max_picture_buffer));
}

/*
NAN_GETTER(AVFormatContext::GetChapters) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  Local<Array> chapters = NanNew<Array>(ref->nb_chapters);
  for (uint32_t i = 0; i < ref->nb_chapters; i++) {
    if (ref->chapters[i]) {
      Local<Value> v = AVChapter::NewInstance(ref->chapters[i]);
      chapters->Set(i, v);
    }
  }

  NanReturnValue(chapters);
}
*/

NAN_GETTER(AVFormatContext::GetMetadata) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  Local<Object> ret = NanNew<Object>();
  AVDictionary *metadata = ref->metadata;
  AVDictionaryEntry *t = nullptr;
  while ((t = av_dict_get(metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    ret->Set(NanNew<String>(t->key), NanNew<String>(t->value));

  NanReturnValue(ret);
}

NAN_GETTER(AVFormatContext::GetStartTimeRealtime) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t start_time_realtime = ref->start_time_realtime;

  NanReturnValue(NanNew<Number>(start_time_realtime));
}

NAN_SETTER(AVFormatContext::SetStartTimeRealtime) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("start_time_realtime: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->start_time_realtime = value->IntegerValue();
}

NAN_GETTER(AVFormatContext::GetFpsProbeSize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int fps_probe_size = ref->fps_probe_size;

  NanReturnValue(NanNew<Integer>(fps_probe_size));
}

NAN_SETTER(AVFormatContext::SetFpsProbeSize) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("fps_probe_size: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->fps_probe_size = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetErrorRecognition) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int error_recognition = ref->error_recognition;

  NanReturnValue(NanNew<Integer>(error_recognition));
}

NAN_SETTER(AVFormatContext::SetErrorRecognition) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("error_recognition: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->error_recognition = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetMaxInterleaveDelta) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t max_interleave_delta = ref->max_interleave_delta;

  NanReturnValue(NanNew<Number>(max_interleave_delta));
}

NAN_SETTER(AVFormatContext::SetMaxInterleaveDelta) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_interleave_delta: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->max_interleave_delta = value->IntegerValue();
}

NAN_GETTER(AVFormatContext::GetStrictStdCompliance) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int strict_std_compliance = ref->strict_std_compliance;

  NanReturnValue(NanNew<Integer>(strict_std_compliance));
}

NAN_SETTER(AVFormatContext::SetStrictStdCompliance) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("strict_std_compliance: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->strict_std_compliance = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetEventFlags) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int event_flags = ref->event_flags;

  NanReturnValue(NanNew<Integer>(event_flags));
}

NAN_SETTER(AVFormatContext::SetEventFlags) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("event_flags: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->event_flags = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetMaxTsProbe) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int max_ts_probe = ref->max_ts_probe;

  NanReturnValue(NanNew<Integer>(max_ts_probe));
}

NAN_SETTER(AVFormatContext::SetMaxTsProbe) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("max_ts_probe: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->max_ts_probe = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetAvoidNegativeTs) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int avoid_negative_ts = ref->avoid_negative_ts;

  NanReturnValue(NanNew<Integer>(avoid_negative_ts));
}

NAN_SETTER(AVFormatContext::SetAvoidNegativeTs) {
  NanScope();

  if (!value->IsNumber())
    NanThrowTypeError("avoid_negative_ts: integer required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ref->avoid_negative_ts = value->Int32Value();
}

NAN_GETTER(AVFormatContext::GetAudioPreload) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int audio_preload = ref->audio_preload;

  NanReturnValue(NanNew<Integer>(audio_preload));
}

NAN_GETTER(AVFormatContext::GetMaxChunkDuration) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int max_chunk_duration = ref->max_chunk_duration;

  NanReturnValue(NanNew<Integer>(max_chunk_duration));
}

NAN_GETTER(AVFormatContext::GetMaxChunkSize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int max_chunk_size = ref->max_chunk_size;

  NanReturnValue(NanNew<Integer>(max_chunk_size));
}

NAN_GETTER(AVFormatContext::GetUseWallclockAsTimestamps) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int use_wallclock_as_timestamps = ref->use_wallclock_as_timestamps;

  NanReturnValue(NanNew<Integer>(use_wallclock_as_timestamps));
}

NAN_GETTER(AVFormatContext::GetAvioFlags) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int avio_flags = ref->avio_flags;

  NanReturnValue(NanNew<Integer>(avio_flags));
}

NAN_GETTER(AVFormatContext::GetDurationEstimationMethod) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  enum ::AVDurationEstimationMethod duration_estimation_method =
    ref->duration_estimation_method;

  NanReturnValue(NanNew<Integer>(duration_estimation_method));
}

NAN_GETTER(AVFormatContext::GetSkipInitialBytes) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t skip_initial_bytes = ref->skip_initial_bytes;

  NanReturnValue(NanNew<Number>(skip_initial_bytes));
}

NAN_GETTER(AVFormatContext::GetCorrectTsOverflow) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  unsigned int correct_ts_overflow = ref->correct_ts_overflow;

  NanReturnValue(NanNew<Integer>(correct_ts_overflow));
}

NAN_GETTER(AVFormatContext::GetSeek2Any) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int seek2any = ref->seek2any;

  NanReturnValue(NanNew<Integer>(seek2any));
}

NAN_GETTER(AVFormatContext::GetFlushPackets) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int flush_packets = ref->flush_packets;

  NanReturnValue(NanNew<Integer>(flush_packets));
}

NAN_GETTER(AVFormatContext::GetProbeScore) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int probe_score = ref->probe_score;

  NanReturnValue(NanNew<Integer>(probe_score));
}

NAN_GETTER(AVFormatContext::GetFormatProbesize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int format_probesize = ref->format_probesize;

  NanReturnValue(NanNew<Integer>(format_probesize));
}

NAN_GETTER(AVFormatContext::GetCodecWhitelist) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  char *codec_whitelist = ref->codec_whitelist;

  if (codec_whitelist)
    NanReturnValue(NanNew<String>(codec_whitelist));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVFormatContext::GetFormatWhitelist) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  char *format_whitelist = ref->format_whitelist;

  if (format_whitelist)
    NanReturnValue(NanNew<String>(format_whitelist));
  else
    NanReturnEmptyString();
}

NAN_GETTER(AVFormatContext::GetDataOffset) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t data_offset = ref->data_offset;

  NanReturnValue(NanNew<Number>(data_offset));
}

NAN_GETTER(AVFormatContext::GetRawPacketBufferRemainingSize) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int raw_packet_buffer_remaining_size = ref->raw_packet_buffer_remaining_size;

  NanReturnValue(NanNew<Integer>(raw_packet_buffer_remaining_size));
}

NAN_GETTER(AVFormatContext::GetOffset) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t offset = ref->offset;

  NanReturnValue(NanNew<Number>(offset));
}

NAN_GETTER(AVFormatContext::GetOffsetTimebase) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVRational offset_timebase = ref->offset_timebase;

  Local<Object> ret = NanNew<Object>();
  ret->Set(NanNew<String>("num"), NanNew<Integer>(offset_timebase.num));
  ret->Set(NanNew<String>("den"), NanNew<Integer>(offset_timebase.den));

  NanReturnValue(ret);
}

NAN_GETTER(AVFormatContext::GetIoRepositioned) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int io_repositioned = ref->io_repositioned;

  NanReturnValue(NanNew<Integer>(io_repositioned));
}

/*
NAN_GETTER(AVFormatContext::GetVideoCodec) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVCodec *codec = av_format_get_video_codec(ref);
  if (!codec)
    NanReturnNull();
  Local<Value> ret = AVCodec::AVCodec::NewInstance(codec);

  NanReturnValue(ret);
}

NAN_SETTER(AVFormatContext::SetVideoCodec) {
  NanEscapableScope();

  if (!AVCodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVCodec *codec = Unwrap<AVCodec::AVCodec>(value->ToObject())->This();

  av_format_set_video_codec(ref, codec);
}

NAN_GETTER(AVFormatContext::GetAudioCodec) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVCodec *codec = av_format_get_audio_codec(ref);
  if (!codec)
    NanReturnNull();
  Local<Value> ret = AVCodec::AVCodec::NewInstance(codec);

  NanReturnValue(ret);
}

NAN_SETTER(AVFormatContext::SetAudioCodec) {
  NanEscapableScope();

  if (!AVCodec::AVCodec::HasInstance(value))
    return NanThrowTypeError("codec required");

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVCodec *codec = Unwrap<AVCodec::AVCodec>(value->ToObject())->This();

  av_format_set_audio_codec(ref, codec);
}

NAN_GETTER(AVFormatContext::GetSubtitleCodec) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  ::AVCodec *codec = av_format_get_subtitle_codec(ref);

  if (!codec)
    NanReturnNull();
  Local<Value> ret = AVCodec::AVCodec::NewInstance(codec);

  NanReturnValue(ret);
}
*/

NAN_GETTER(AVFormatContext::GetMetadataHeaderPadding) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int metadata_header_padding = ref->metadata_header_padding;

  NanReturnValue(NanNew<Integer>(metadata_header_padding));
}

NAN_GETTER(AVFormatContext::GetOutputTsOffset) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t output_ts_offset = ref->output_ts_offset;

  NanReturnValue(NanNew<Number>(output_ts_offset));
}

NAN_GETTER(AVFormatContext::GetMaxAnalyzeDuration2) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t max_analyze_duration2 = ref->max_analyze_duration2;

  NanReturnValue(NanNew<Number>(max_analyze_duration2));
}

NAN_GETTER(AVFormatContext::GetProbesize2) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  int64_t probesize2 = ref->probesize2;

  NanReturnValue(NanNew<Number>(probesize2));
}

NAN_GETTER(AVFormatContext::GetDumpSeparator) {
  NanEscapableScope();

  ::AVFormatContext *ref = Unwrap<AVFormatContext>(args.This())->This();
  uint8_t *dump_separator = ref->dump_separator;

  if (dump_separator)
    NanReturnValue(NanNew<String>(dump_separator));
  else
    NanReturnEmptyString();
}

}  // namespace avformat
}  // namespace ffmpeg
