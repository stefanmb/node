/* Temporary home of new add-on API */

#ifndef SRC_NODE_API_H_
#define SRC_NODE_API_H_

#include "node_api_types.h"
#include "node_version.h"  // NODE_MODULE_VERSION

namespace node {
namespace api {

/*************************************
 *        Module Registration
 *************************************/

typedef void (*addon_register_func)(
    v8::Local<v8::Object> exports,
    v8::Local<v8::Value> module,
    void* priv);

typedef void (*addon_context_register_func)(
    v8::Local<v8::Object> exports,
    v8::Local<v8::Value> module,
    v8::Local<v8::Context> context, /* TODO: Can this be made generic or should it be deprecated? */
    void* priv);

#define NM_F_BUILTIN 0x01
#define NM_F_LINKED  0x02

struct node_module {
  int nm_version;
  unsigned int nm_flags;
  void* nm_dso_handle;
  const char* nm_filename;
  node::api::addon_register_func nm_register_func;
  node::api::addon_context_register_func nm_context_register_func;
  const char* nm_modname;
  void* nm_priv;
  struct node_module* nm_link;
};

#if defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define NODE_C_CTOR(fn)                                               \
  static void __cdecl fn(void);                                       \
  __declspec(dllexport, allocate(".CRT$XCU"))                         \
      void (__cdecl*fn ## _)(void) = fn;                              \
  static void __cdecl fn(void)
#else
#define NODE_C_CTOR(fn)                                               \
  static void fn(void) __attribute__((constructor));                  \
  static void fn(void)
#endif

#define NODE_MODULE_X(modname, regfunc, priv, flags)                  \
  extern "C" {                                                        \
    static node::api::node_module _module =                           \
    {                                                                 \
      NODE_MODULE_VERSION,                                            \
      flags,                                                          \
      NULL,                                                           \
      __FILE__,                                                       \
      (node::api::addon_register_func) (regfunc),                     \
      NULL,                                                           \
      NODE_STRINGIFY(modname),                                        \
      priv,                                                           \
      NULL                                                            \
    };                                                                \
    NODE_C_CTOR(_register_ ## modname) {                              \
      node::api::node_module_register(&_module);                      \
    }                                                                 \
  }

#define NODE_MODULE_CONTEXT_AWARE_X(modname, regfunc, priv, flags)    \
  extern "C" {                                                        \
    static node::api::node_module _module =                           \
    {                                                                 \
      NODE_MODULE_VERSION,                                            \
      flags,                                                          \
      NULL,                                                           \
      __FILE__,                                                       \
      NULL,                                                           \
      (node::api::addon_context_register_func) (regfunc),             \
      NODE_STRINGIFY(modname),                                        \
      priv,                                                           \
      NULL                                                            \
    };                                                                \
    NODE_C_CTOR(_register_ ## modname) {                              \
      node::api::node_module_register(&_module);                       \
    }                                                                 \
  }

#define NODE_MODULE(modname, regfunc)                                 \
  NODE_MODULE_X(modname, regfunc, NULL, 0)

#define NODE_MODULE_CONTEXT_AWARE(modname, regfunc)                   \
  NODE_MODULE_CONTEXT_AWARE_X(modname, regfunc, NULL, 0)

node_module* get_builtin_module(const char *name);
node_module* get_linked_module(const char *name);

extern "C" NODE_EXTERN void node_module_register(void* mod);

} // namespace api
} // namespace node

#endif // SRC_NODE_API_H_
