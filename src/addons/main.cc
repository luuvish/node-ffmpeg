#include <node.h>
#include <nan.h>

#include "avformat.h"
#include "avcodec.h"
#include "avutil.h"

using namespace v8;


void Init(Handle<Object> exports) {
  NanScope();

  av_register_all();
  avcodec_register_all();
  avformat_network_init();

  FFmpeg::AVFormat::Initialize(exports);
  FFmpeg::AVCodec::Initialize(exports);
  FFmpeg::AVUtil::Initialize(exports);

  exports->Set(NanNew<String>("INT64_MIN"), NanNew<Number>(INT64_MIN));
  exports->Set(NanNew<String>("INT64_MAX"), NanNew<Number>(INT64_MAX));
}


NODE_MODULE(ffmpeg, Init)
