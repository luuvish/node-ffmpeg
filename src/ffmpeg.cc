#include <node.h>
#include <nan.h>

#include "avformat.h"
#include "avcodec.h"

using namespace v8;


void Init(Handle<Object> exports) {
  NanScope();

  FFmpeg::AVStreamWrapper::Initialize(exports);
  FFmpeg::AVProgramWrapper::Initialize(exports);
  FFmpeg::AVChapterWrapper::Initialize(exports);
  FFmpeg::AVFormatContextWrapper::Initialize(exports);

  FFmpeg::AVCodecContextWrapper::Initialize(exports);
}


NODE_MODULE(ffmpeg, Init)
