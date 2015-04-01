#include <nan.h>

#include "avformat/avformat.h"
#include "avcodec/avcodec.h"
#include "avutil/avutil.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  NanScope();

  ffmpeg::avformat::Init(exports);
  ffmpeg::avcodec::Init(exports);
  ffmpeg::avutil::Init(exports);

  NODE_DEFINE_CONSTANT(exports, INT64_MIN);
  NODE_DEFINE_CONSTANT(exports, INT64_MAX);
}

NODE_MODULE(ffmpeg, InitAll)
