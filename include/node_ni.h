/*********************************************************************
 * Node Native Interface (NI) Prototype
 *
 * Design doc: https://goo.gl/rzTQ5T
 *
 * Based on NAN - Native Abstractions for Node.js (2.2.1)
 *
 * Copyright (c) 2016 NAN contributors:
 *   - Rod Vagg <https://github.com/rvagg>
 *   - Benjamin Byholm <https://github.com/kkoopa>
 *   - Trevor Norris <https://github.com/trevnorris>
 *   - Nathan Rajlich <https://github.com/TooTallNate>
 *   - Brett Lawson <https://github.com/brett19>
 *   - Ben Noordhuis <https://github.com/bnoordhuis>
 *   - David Siegel <https://github.com/agnat>
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 *
 * Version 2.2.1: current Node 5.9.1
 *
 *********************************************************************/

#ifndef NODE_NI_H_
#define NODE_NI_H_

#include "v8.h" // TODO: Goal is to remove this dependency entirely.

namespace node {
  namespace ni {

// ***********************************
//          Global State
// ***********************************

// Documentation

struct NI_Args
{
  void * env; // -> opaque pointer to whatever global state needs to be kept.
  //TODO In the case of isolates (as per https://github.com/nodejs/nan/issues/185) how does it affect thread safety?
  // stefan: Problem seems similar to MM_Environment in OMR.
};

// ***********************************
//          Error Handling
// ***********************************

// Documentation

enum ErrorCode { None, Internal }; //TODO Look at https://github.com/Microsoft/ChakraCore/wiki/JsErrorCode

// ***********************************
//          Initialization
// ***********************************

// Documentation

/* NODE_MODULE requires a registration function be be passed,
 * and it also allocates the module descriptor struct statically.
 *
 * In Nan a convenience macro is provided ("NAN_MODULE_INIT") that
 * defines the registration function of the module.
 * According to https://github.com/nodejs/nan/issues/185 the
 * V8::Isolate::GetCurrent() will be deprecated/phased out, so
 * going forward use only the context-aware version of the
 * module registration function.
 *
 * TODO: Clarify state of https://bugs.chromium.org/p/chromium/issues/detail?id=359977
 *
 */

// Types

/* Obviously the V8 specific types have to be gotten rid of.
 * The context is particularly interesting because
 */

  typedef void (*addon_context_register_func)(
      v8::Local<v8::Object> exports,
      v8::Local<v8::Value> module,
      v8::Local<v8::Context> context,
      void* priv);

// Functions

/* Copied from node.h, this is necessary to export function on Win32/64 */
//TODO What about other non-Linux platforms?
#ifdef _WIN32
# ifndef BUILDING_NODE_EXTENSION
#   define NODE_NI_EXTERN __declspec(dllexport)
# else
#   define NODE_NI_EXTERN __declspec(dllimport)
# endif
#else
# define NODE_NI_EXTERN /* nothing */
#endif

#define NODE_NI_MODULE_INIT(name, regfunc) module_init(name, __FILE__, regfunc)
  NODE_NI_EXTERN ErrorCode module_init(
      const char * const name,
      const char * const filename,
      addon_context_register_func regfunc);

//TODO No way to unload a native module, but that may be out of scope.

// ***********************************
//           Methods
// ***********************************

// Documentation

/* Nan macro-fies the standard V8 FunctionCallbackInfo, method signature should be:
 *    void foo(const FunctionCallbackInfo<Value>& args) { ... }
 *
 *
 *
 */



  }
}


#endif  // NODE_NI_H_
