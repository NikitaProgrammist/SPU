#include <stdio.h>

#include "stack.h"

int main() {
  Stack * stack = NULL;
  StackErr result = stackInit(&stack);
  size_t * p = (size_t *) stack;
  *p = 0;
  stackPush(stack, 10);
  stackDestroy(stack);
}
