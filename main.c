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
