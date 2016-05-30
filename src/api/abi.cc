#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "api.h"


// TODO: Temp fix:
namespace node {
bool node_is_initialized = false;
node_module* modpending;
}

namespace node {
namespace api {

static node_module* modlist_builtin;
static node_module* modlist_linked;
extern "C" void node_module_register(void* m) {
  struct node_module* mp = reinterpret_cast<struct node_module*>(m);

  if (mp->nm_flags & NM_F_BUILTIN) {
    mp->nm_link = modlist_builtin;
    modlist_builtin = mp;
  } else if (!node_is_initialized) {
    // "Linked" modules are included as part of the node project.
    // Like builtins they are registered *before* node::Init runs.
    mp->nm_flags = NM_F_LINKED;
    mp->nm_link = modlist_linked;
    modlist_linked = mp;
  } else {
    modpending = mp;
  }
}

struct node_module* get_builtin_module(const char* name) {
  struct node_module* mp;

  for (mp = modlist_builtin; mp != nullptr; mp = mp->nm_link) {
    if (strcmp(mp->nm_modname, name) == 0)
      break;
  }

  CHECK(mp == nullptr || (mp->nm_flags & NM_F_BUILTIN) != 0);
  return (mp);
}

struct node_module* get_linked_module(const char* name) {
  struct node_module* mp;

  for (mp = modlist_linked; mp != nullptr; mp = mp->nm_link) {
    if (strcmp(mp->nm_modname, name) == 0)
      break;
  }

  CHECK(mp == nullptr || (mp->nm_flags & NM_F_LINKED) != 0);
  return mp;
}

} // namespace api
} // namespace node
