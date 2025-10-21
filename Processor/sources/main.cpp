#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "read.h"
#include "run.h"

int main(int argc, char * argv[]) {
  Processor processor = {};
  stackInit(&processor.value_stack);
  stackInit(&processor.func_stack);
  if (argc > 1) {
    Errors result = readFile(&processor, argv[1]);
    if (result != SUCCESS_P) {
      printf("%d\n", result);
    }
    result = run(&processor);
    if (result != SUCCESS_P) {
      errorParser(result);
    }
  }
  free(processor.commands);
  stackDestroy(processor.value_stack);
  stackDestroy(processor.func_stack);
}
