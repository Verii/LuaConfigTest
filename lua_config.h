#ifndef lua_config_h
#define lua_config_h

#include <stddef.h>
#include <lua.h>

struct config {
  const char *path;
  size_t path_len;

  lua_State *L;
};

#if 0
enum config_key_reqs {
  CONFIG_KEY_REQUIRED,
  CONFIG_KEY_OPTIONAL,
};

struct {
  const char *keyname;
  enum config_key_reqs req;
  uint8_t lua_type;
} config_key_defs[] = {
  { "key1", CONFIG_KEY_REQUIRED, LUA_TSTRING },
  { "age",  CONFIG_KEY_OPTIONAL, LUA_TNUMBER },
  { NULL, 0, 0 }
};
#endif


/* Return a new handle to a configuration parser
 */
struct config *
config_new(const char *path);

void
config_delete(struct config *);

int
config_get_key(struct config *, const char *key, char **, size_t *);

#endif
