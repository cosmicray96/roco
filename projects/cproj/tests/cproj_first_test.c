#include "cproj/cproj.h"

#include <assert.h>
#include <stdio.h>

int main() {

  assert(cproj_add(1, 2) == 3);
  printf("1 + 2 = %d", cproj_add(1, 2));

  return 0;
}
