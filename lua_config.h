/*
    Copyright 2016 James Smith <james@apertum.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef lua_config_h
#define lua_config_h

#include <stddef.h>

struct config;

enum config_print_format {
  CONFIG_PRINT_NONE,
  CONFIG_PRINT_INDENT,
  CONFIG_PRINT_PRETTY,
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

/* Delete a previously allocated configuration handle */
void
config_delete(struct config *c);

int
config_get_key(struct config *c, const char *key, char **res, size_t *res_len);

int
config_print_keyval(struct config* c, const char* key, enum config_print_format fmt);

int
config_print_table(struct config *c, enum config_print_format fmt);

#endif
