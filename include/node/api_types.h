#ifndef SRC_NODE_API_TYPES_H_
#define SRC_NODE_API_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Investigate where to move symbol exports below.
#ifdef _WIN32
# define NODE_MODULE_EXPORT __declspec(dllexport)
#else
# define NODE_MODULE_EXPORT __attribute__((visibility("default")))
#endif

/*************************************
 *             Primitives
 *************************************/

/* Based on initial submission here:
 * https://github.com/nodejs/api/blob/master/native/data_types.md
 */

enum _JSTypes2  {
  JS_Int32 = 1,
  JS_Double = 2,
  JS_Boolean = 3,
  JS_String = 4,
  JS_Object = 5,
  JS_Buffer = 6,
  JS_Undefined = 7,
  JS_Null = 8,
  JS_Error = 9,
  JS_Function = 10
};

struct _JSValue {
  void* env_;
  void* data_;
  uintptr_t flags_;
};

// TODO Setters/Getters

#ifdef __cplusplus
}
#endif
#endif // SRC_NODE_API_TYPES_H_
