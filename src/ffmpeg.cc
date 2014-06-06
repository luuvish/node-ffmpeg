#include <node.h>
#include <nan.h>

#include "avformat.h"
#include "avcodec.h"

using namespace v8;


void Init(Handle<Object> exports) {
  NanScope();

  av_register_all();
  avformat_network_init();
  avcodec_register_all();

  FFmpeg::AVOutputFormatWrapper::Initialize(exports);
  FFmpeg::AVInputFormatWrapper::Initialize(exports);
  FFmpeg::AVStreamWrapper::Initialize(exports);
  FFmpeg::AVProgramWrapper::Initialize(exports);
  FFmpeg::AVChapterWrapper::Initialize(exports);
  FFmpeg::AVFormatContextWrapper::Initialize(exports);

  FFmpeg::AVPacketWrapper::Initialize(exports);
  FFmpeg::AVCodecContextWrapper::Initialize(exports);
  FFmpeg::AVCodecWrapper::Initialize(exports);
  FFmpeg::AVPictureWrapper::Initialize(exports);
  FFmpeg::AVSubtitleRectWrapper::Initialize(exports);
  FFmpeg::AVSubtitleWrapper::Initialize(exports);

  exports->Set(NanNew<String>("AVMEDIA_TYPE_UNKNOWN"), NanNew<Number>(AVMEDIA_TYPE_UNKNOWN));
  exports->Set(NanNew<String>("AVMEDIA_TYPE_VIDEO"), NanNew<Number>(AVMEDIA_TYPE_VIDEO));
  exports->Set(NanNew<String>("AVMEDIA_TYPE_AUDIO"), NanNew<Number>(AVMEDIA_TYPE_AUDIO));
  exports->Set(NanNew<String>("AVMEDIA_TYPE_DATA"), NanNew<Number>(AVMEDIA_TYPE_DATA));
  exports->Set(NanNew<String>("AVMEDIA_TYPE_SUBTITLE"), NanNew<Number>(AVMEDIA_TYPE_SUBTITLE));
  exports->Set(NanNew<String>("AVMEDIA_TYPE_ATTACHMENT"), NanNew<Number>(AVMEDIA_TYPE_ATTACHMENT));

  exports->Set(NanNew<String>("AV_NOPTS_VALUE"), NanNew<Number>(AV_NOPTS_VALUE));
  exports->Set(NanNew<String>("AV_TIME_BASE"), NanNew<Number>(AV_TIME_BASE));
  Handle<Object> time_base_q = NanNew<Object>();
  time_base_q->Set(NanNew<String>("num"), NanNew<Number>(1));
  time_base_q->Set(NanNew<String>("den"), NanNew<Number>(AV_TIME_BASE));
  exports->Set(NanNew<String>("AV_TIME_BASE_Q"), time_base_q);
}


NODE_MODULE(ffmpeg, Init)
