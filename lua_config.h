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

/* Returns a new configuration handle
 *
 * (path) is the location of the Lua configuration file
 *
 * returns NULL if the file is not valid Lua
 * returns a pointer if the file is valid
 */
struct config *
config_new(const char *path);

/* Closes a previously opened configuration handle
 */
void
config_delete(struct config *c);

/* Returns the value from the requested (key) converted to a string
 *
 * after parsing the configuration file, there is a table at the top of stack
 * that contains our configuration. Each index in this table *must* be a
 * string.
 *
 * (key) is the string index into this table.
 * (res) is the value at that index converted to a string.
 * (res_len) is the length of the returned value
 * Therefore *only* string and number value types are supported.
 *
 * 0 is returned to indicate the key was found and it was of the proper type
 * non-0 is returned in any other case
 */
int
config_get_key(struct config *c, const char *key, char **res, size_t *res_len);

/* Allows easily printing a key:value pair
 *
 * (key) is searched for in the table and printed to the screen according to
 * (fmt)
 *
 * Allows printing key:value pairs and whole tables with the same style easily
 *
 * Returns the result of config_get_key called with the given key
 */
int
config_print_keyval(struct config* c, const char* key, enum config_print_format fmt);

/* Prints each key:value pair to standard out
 *
 * each index in the table must be a string
 * each value must be either a string or number (convertible to string)
 *
 * 0 is returned if there are no type violations
 * non-0 is returned otherwise
 */
int
config_print_table(struct config *c, enum config_print_format fmt);

#endif
