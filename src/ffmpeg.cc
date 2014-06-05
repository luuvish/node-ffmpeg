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

  FFmpeg::AVInputFormatWrapper::Initialize(exports);
  FFmpeg::AVOutputFormatWrapper::Initialize(exports);
  FFmpeg::AVStreamWrapper::Initialize(exports);
  FFmpeg::AVProgramWrapper::Initialize(exports);
  FFmpeg::AVChapterWrapper::Initialize(exports);
  FFmpeg::AVFormatContextWrapper::Initialize(exports);

  FFmpeg::AVCodecWrapper::Initialize(exports);
  FFmpeg::AVCodecContextWrapper::Initialize(exports);
}


NODE_MODULE(ffmpeg, Init)
