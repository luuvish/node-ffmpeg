#ifndef SRC_ADDONS_AVFORMAT_AVFORMAT_H_
#define SRC_ADDONS_AVFORMAT_AVFORMAT_H_

#include <node.h>
#include <nan.h>
#include <list>

extern "C" {
#include "libavformat/avformat.h"
}

namespace FFmpeg {
namespace AVFormat {

void Initialize(v8::Handle<v8::Object> target);

}  // namespace AVFormat
}  // namespace FFmpeg

#endif  // SRC_ADDONS_AVFORMAT_AVFORMAT_H_
