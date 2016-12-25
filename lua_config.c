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
config_get_key(struct config* c, const char* key, char** res, size_t* res_len)
{
  assert(c);
  assert(c->L);
  assert(key);
  assert(res);

  // Fail if the top value is not a table
  if (!lua_istable(c->L, -1))
    return -1;

  const uint8_t table_idx = (uint8_t) lua_gettop(c->L);
  char* value = NULL;
  size_t value_len = 0;
  int ret = -1; // assume error

  lua_pushnil(c->L); // first key
  while (lua_next(c->L, table_idx)) {

    // fail if key is not a string
    if (!lua_isstring(c->L, -2)) {
      break;
    }

    const char* search_key;
    search_key = lua_tolstring(c->L, -2, NULL);

    // return value if it iss a string or number
    if (!strcmp(key, search_key) &&
        (lua_isnumber(c->L, -1) || lua_isstring(c->L, -1))) {

      const char* tmp_val;
      tmp_val = lua_tolstring(c->L, -1, &value_len);

      value = malloc(value_len +1);
      strncpy(value, tmp_val, value_len);
      value[value_len] = '\0';

      ret = 0;
      break;
    }

    lua_pop(c->L, 1);
  }

  if (res_len)
    *res_len = value_len;
  *res = value;

  // restore stack to its original state
  lua_pop(c->L, lua_gettop(c->L) - table_idx);
  return ret;
}

/* Prints each key:value pair to standard out
 *
 * each index in the table must be a string
 * each value must be either a string or number (convertible to string)
 *
 * 0 is returned if there are no type violations
 * non-0 is returned otherwise
 */
int
config_print_table(struct config* c, enum config_print_format fmt)
{
  assert(c);
  assert(c->L);

  if (!lua_istable(c->L, -1))
    return -1;

  const uint8_t table_idx = (uint8_t) lua_gettop(c->L);
  int ret = -1;

  lua_pushnil(c->L); // first key
  while (lua_next(c->L, table_idx)) {
    const char *key, *value;
    key = value = NULL;

    if (lua_isstring(c->L, -2)) {
      key = lua_tolstring(c->L, -2, NULL);
    }

    if (lua_isstring(c->L, -1) || lua_isnumber(c->L, -1)) {
      value = lua_tolstring(c->L, -1, NULL);
    }

    if (key && value) {
      switch (fmt) {
        case CONFIG_PRINT_PRETTY:
        case CONFIG_PRINT_INDENT:
          printf("  ");
          break;
        case CONFIG_PRINT_NONE:
          break;
      }
      printf("%s : %s\n", key, value);
    }

    lua_pop(c->L, 1);
  }

  // restore stack to its original state
  lua_pop(c->L, lua_gettop(c->L) - table_idx);
  return ret;
}
