#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "lua_config.h"

static const char *whitelist[] = {
  "assert", "ipairs", "next", "pairs", "print", "tonumber", "tostring", "type",
  NULL // terminator
};

static void
l_sandbox(lua_State* L, const char** wl)
{
  (void) L; (void) wl;
  // get global _G
  // compare each entry to whitelist
  // set to nil any entry not in whitelist
  return;
}

/* Returns a new configuration handle
 *
 * (path) is the location of the Lua configuration file
 *
 * returns NULL if the file is not valid Lua
 * returns a pointer if the file is valid
 */
struct config*
config_new(const char* path)
{
  assert(path);

  struct config* conf = NULL;

  struct config* tmp_conf = calloc(1, sizeof(*tmp_conf));
  assert(tmp_conf);

  tmp_conf->L = luaL_newstate();
  assert(tmp_conf->L);

  luaopen_base(tmp_conf->L); // print, next, ...
  l_sandbox(tmp_conf->L, whitelist);

  // Only return NULL if we can't read the file
  if (luaL_dofile(tmp_conf->L, path) != 0) {
    lua_close(tmp_conf->L);
    tmp_conf->L = NULL;

    free(tmp_conf);
    tmp_conf = NULL;
  }

  conf = tmp_conf;
  return conf;
}

/* Closes a previously opened configuration handle
 */
void
config_delete(struct config* c)
{
  assert(c);
  assert(c->L);
  lua_close(c->L);
  c->L = NULL;
  free(c);
}

/* Returns the value from the requested (key) converted to a string
 *
 * after parsing the configuration file, there is a table at the top of stack
 * that contains our configuration. Each index in this table *must* be a
 * string.
 *
 * (key) is the string index into this table.
 * (res) is the value at that index converted to a string.
 * Therefore *only* string and number value types are supported.
 *
 * 0 is returned to indicate the key was found and it was of the proper type
 * non-0 is returned in any other case
 */
int
config_get_key(struct config* c, const char* key, char** res, size_t* len)
{
  assert(c);
  assert(key);
  assert(res);

  lua_getglobal(c->L, "table");
  if (lua_isnil(c->L, -1))
    fprintf(stderr, "Top of stack is nil, \"table\" does not exist\n");
  lua_pop(c->L, 1);

  // Fail if the top value is not a table
  if (!lua_istable(c->L, -1))
    return -1;

  const uint8_t stack_top = lua_gettop(c->L);
  char* value = NULL;
  int ret = -1; // assume error

  lua_pushnil(c->L); // first key
  while (lua_next(c->L, stack_top)) {

    // fail if it's not a string
    if (!lua_isstring(c->L, -2)) {
      break;
    }

    const char* search_key;
    search_key = lua_tolstring(c->L, -2, NULL);

    // store key or fail if it's not a string or number
    if (!strcmp(key, search_key) &&
        (lua_isnumber(c->L, -1) || lua_isstring(c->L, -1))) {
      size_t val_len = -1;
      const char* tmp_val;
      tmp_val = lua_tolstring(c->L, -1, &val_len);

      value = malloc(val_len);
      strncpy(value, tmp_val, val_len);

      if (len)
        *len = val_len;
      *res = value;
      ret = 0;
      break;
    }

    lua_pop(c->L, 1);
  }

  // restore stack to its original state
  lua_pop(c->L, stack_top - lua_gettop(c->L));
  return ret;
}
