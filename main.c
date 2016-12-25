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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua_config.h"

int
main(int argc, char **argv)
{
  if (argc <= 1) {
    fprintf(stderr, "usage: %s <file> [search key ...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  // read the configuration in file provided by the first arg
  struct config *c = config_new(argv[1]);
  if (!c) {
    fprintf(stderr, "Unable to parse configuration file: %s\n", argv[1]);
    fprintf(stderr, "Aborting\n");
    return EXIT_FAILURE;
  }

  argc -= 2;
  argv += 2;

  // if the user doesn't give us any keys to print, print them all
  if (!(*argv)) {
    config_print_table(c, CONFIG_PRINT_PRETTY);

  // loop over each supplied key and print its value
  } else {
    do {
      config_print_keyval(c, *argv, CONFIG_PRINT_PRETTY);
    } while (*(++argv));
  }

  config_delete(c);
  return EXIT_SUCCESS;
}
