#include "node_ni.h"
#include "node.h"

namespace node {
  namespace ni {

// ***********************************
//          Global State
// ***********************************

  struct Environment
  {
    //TODO
  };

// ***********************************
//          Initialization
// ***********************************

  struct node_module {
    int nm_version;
    unsigned int nm_flags;
    void* nm_dso_handle;
    const char* nm_filename;
    node::addon_register_func nm_register_func; // TODO No longer provided, do we change core too?
    node::addon_context_register_func nm_context_register_func;
    const char* nm_modname;
    void* nm_priv;
    struct node_module* nm_link;
  };

  ErrorCode module_init(
        const char * const name,
        const char * const filename,
        addon_context_register_func regfunc) {
    // TODO Note that it's unfortunate that we'll have to dynamically allocate the struct, especially
    // since there is no support for unloading native modules, therefore it will never be dealloc'd. Fix this?
    node_module * module = calloc(1, sizeof(node_module));
    if (NULL == module)
      goto error;
    module->nm_version = NODE_MODULE_VERSION; //TODO What is the meaning of the version number w.r.t to ABI?
    module->nm_filename = filename;
    module->nm_register_func = regfunc;
    node_module_register(module);
    return ErrorCode::None;
error:
    return ErrorCode::Internal;
  }

  }
}
