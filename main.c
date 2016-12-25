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

  struct config *c = config_new(argv[1]);
  assert(c);

  argc -= 2;
  argv += 2;

  if (!(*argv)) {
    config_print_table(c, CONFIG_PRINT_NONE);
  } else {
    do {
      char *val;
      size_t len;
      config_get_key(c, *argv, &val, &len);
      printf("%s : (%lu) %s\n", *argv, len, val);
      free(val);
    } while (*(++argv));
  }

  config_delete(c);
  return EXIT_SUCCESS;
}
