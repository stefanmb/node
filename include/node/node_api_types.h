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

// Fundamental type wrappers

enum JSTypes  {
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

struct JSValue {
  void* env;
  void* data_;
  enum JSTypes type_;
};

// Type checking helpers

#define JS_DECLARE_TypeChecker(type) \
  NODE_MODULE_EXPORT bool JS_Is##type(JSValue *value)

#define JS_DEFINE_TypeChecker(type) \
    JS_DECLARE_TypeChecker(type) { \
    if (NULL == value) return false; \
    if (JS_##type == value->type_) { \
      return true; \
    } \
    return false; \
  }

JS_DECLARE_TypeChecker(Int32);
JS_DECLARE_TypeChecker(Double);
JS_DECLARE_TypeChecker(Boolean);
JS_DECLARE_TypeChecker(String);
JS_DECLARE_TypeChecker(Object);
JS_DECLARE_TypeChecker(Buffer);
JS_DECLARE_TypeChecker(Undefined);
JS_DECLARE_TypeChecker(Null);
JS_DECLARE_TypeChecker(Error);
JS_DECLARE_TypeChecker(Function);

// Setters/Getters

NODE_MODULE_EXPORT void JS_SetNamedProperty(JSValue *object, const char * name, JSValue *value);

#ifdef __cplusplus
}
#endif
#endif // SRC_NODE_API_TYPES_H_
