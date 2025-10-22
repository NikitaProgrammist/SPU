#include <stdio.h>
#include <string.h>

#include "to_binary.h"
#include "utils.h"
#include "my_printf.h"

CompileErr checkFiles(int argc, char * argv[]);

int main(int argc, char * argv[]) {
  CompileErr result = checkFiles(argc, argv);
  if (result != SUCCESS) {
    printf("%d\n", result);
  }
}

CompileErr checkFiles(int argc, char * argv[]) {
  const size_t maxsize = 256;
  const char * dir = "./programs/";
  char filepath[maxsize] = {};

  for (int i = 1; i < argc; i++) {
    if (maxsize > strlen(dir) + strlen(argv[i])) {
      snprintf(filepath, maxsize, "%s%s", dir, argv[i]);
      CompileErr error = convert(filepath);

      if (error != SUCCESS) {
        return error;
      }
    }

    else {
      return LONG_NAME;
    }
  }
  return SUCCESS;
}
