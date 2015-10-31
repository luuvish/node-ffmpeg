#ifndef SRC_ADDONS_FFMPEG_H_
#define SRC_ADDONS_FFMPEG_H_

#include <nan.h>

namespace ffmpeg {

inline v8::Local<v8::Object> NewUint8Array(void* data, int size) {
  NanEscapableScope();
  v8::Local<v8::Context> ctx = NanGetCurrentContext();
  v8::Local<v8::Value> array = ctx->Global()->Get(NanNew("Uint8Array"));
  v8::Local<v8::Function> cons = array.As<v8::Function>();

  v8::ExternalArrayType type = v8::kExternalUint8Array;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { NanNew<v8::Uint32>(size) };
  v8::Local<v8::Object> ret = cons->NewInstance(argc, argv);

  ret->SetIndexedPropertiesToExternalArrayData(data, type, size);
  return NanEscapeScope(ret);
}

inline bool GetUint8Array(v8::Local<v8::Object> obj, void* &data, int &size) {
  if (!obj->HasIndexedPropertiesInExternalArrayData())
    return false;
  if (obj->GetIndexedPropertiesExternalArrayDataType() !=
      v8::kExternalUint8Array)
    return false;

  data = obj->GetIndexedPropertiesExternalArrayData();
  size = obj->GetIndexedPropertiesExternalArrayDataLength();
  return true;
}

}  // namespace ffmpeg

#endif  // SRC_ADDONS_FFMPEG_H_
