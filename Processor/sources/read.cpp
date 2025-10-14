#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "read.h"

Errors readFile(Processor * processor, char * filename) {
  const size_t maxsize = 256;
  const char * dir = "./programs/";
  char filepath[maxsize] = {};
  if (maxsize > strlen(dir) + strlen(filename)) {
      snprintf(filepath, maxsize, "%s%s", dir, filename);
      Errors result = getCommands(processor, filepath);
      if (result != SUCCESS_P) {
        return result;
      }
  }
  else {
    return LONG_NAME;
  }
  return SUCCESS_P;
}

Errors getCommands(Processor * processor, char * filename) {
  printf("%s\n", filename);
  FILE * file = fopen(filename, "r");
  if (file == NULL) {
    return fileError();
  }
  int res = fscanf(file, "%zu", &processor->commands_count);
  if (res != 1) {
    return SCANF_ERR;
  }
  processor->commands = (Command *) calloc(processor->commands_count, sizeof(Command));
  if (processor->commands == NULL) {
    return CALLOC_FAILED_P;
  }
  for (size_t i = 0; i < processor->commands_count - 1; i++) {
    res = fscanf(file, "%d", &processor->commands[i].number);
    if (res != 1) {
      return SCANF_ERR;
    }
    if (processor->commands[i].number == 42 || processor->commands[i].number == 33 || processor->commands[i].number == 1 || processor->commands[i].number >= 50) {
      res = fscanf(file, "%d", &processor->commands[i].value);
      if (res != 1) {
        return SCANF_ERR;
      }
    }
  }
  fclose(file);
  return SUCCESS_P;
}

Errors fileError() {
  switch (errno) {
    case ENOENT:
      return NO_FILE;
    case EACCES:
      return NO_ROOT;
    case EISDIR:
      return FILE_DIR;
    case EMFILE:
      return MAX_FILE_PROCESS;
    case ENFILE:
      return MAX_FILE_SYSTEM;
    case EROFS:
      return WRITE_ERROR;
    case EEXIST:
      return FILES_CONFLICT;
    case ENOSPC:
      return MEMORY_ERROR;
    case ENOTDIR:
      return DIR_ERROR;
    default:
      return FILE_ERROR;
  }
}
