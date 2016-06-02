/* Temporary home of internal API functions */

#ifndef SRC_NODE_INTERNAL_API_H_
#define SRC_NODE_INTERNAL_API_H_

#include "node_api_types.h"
#include "node_api.h"

#include "nan.h"

using namespace Nan;

namespace node {
namespace api {
namespace internal {

class JSValueWrapper {
  // see https://github.com/jxcore/jxcore/blob/166784821f30e62503f04eb267ea26cf7c2c58d7/src/jxcore_type_wrap.h
public:
  Persistent<v8::Object> value;

  JSValueWrapper() {}

  ~JSValueWrapper()
  {
    if (!value.IsEmpty()) {
      value.Reset();
    }
  }

};

void JS_InitObjectInternal(JSValue *value, v8::Local<v8::Object> object)
{
  printf(" size is %p\n", sizeof(Persistent<v8::Object>));
}

void JS_DisposeObjectInternal(JSValue *value)
{

}

} // internal
} // namespace api
} // namespace node

#endif // SRC_NODE_INTERNAL_API_H_
