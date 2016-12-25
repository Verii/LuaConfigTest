#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua_config.h"

int
main(int argc, char **argv)
{
  if (argc <= 2) {
    fprintf(stderr, "usage: %s <file> <search key>\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct config *c = config_new(argv[1]);
  assert(c);

  char *val;
  size_t len;
  config_get_key(c, argv[2], &val, &len);

  printf("%s : (%lu) %s\n", argv[2], len, val);

  config_delete(c);
  return EXIT_SUCCESS;
}
