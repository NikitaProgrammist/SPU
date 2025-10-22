#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#include "to_binary.h"
#include "my_printf.h"
#include "utils.h"

CompileErr convert(char * filename) {
  Code code = {.len_labels = 10};
  code.labels = (int *) calloc(code.len_labels, sizeof(int));
  FILE * file = fopen(filename, "r");
  if (file == NULL) {
    return fileError();
  }
  code.filename = filename;
  code.len = getFileSize(file);
  code.buffer = (char *) calloc((code.len + 1), sizeof(char));
  if (code.buffer == NULL) {
    return CALLOC_FAILED;
  }

  getBuffer(file, &code);
  code.array = (char **) calloc(code.total_command, sizeof(char *));

  if (code.array == NULL) {
    return CALLOC_FAILED;
  }

  getArray(&code);
  fclose(file);
  code.commands = (Command *) calloc(code.total_command, sizeof(Command));
  if (code.commands == NULL) {
    return CALLOC_FAILED;
  }
  CompileErr result = getLabels(&code);
  if (result != SUCCESS) {
    Destroy(&code);
    return result;
  }
  result = textToCommands(&code);
  Destroy(&code);
  return result;
}

void Destroy(Code * code) {
  free(code->buffer);
  free(code->array);
  free(code->commands);
  free(code->labels);
}

CompileErr checkStr(char * str, int * arg) {
  int result = 0, sign = 1;
  while (*str != '\0' && *str != ';') {
    if (*str == '-' || *str == '+') {
      sign *= (*str == '-' ? -1 : 1);
      str++;
      continue;
    }
    if (*str == ' ') {
      str++;
      continue;
    }
    if (!isdigit(*str)) {
      return INCORRECT_ARG;
    }
    result *= 10;
    result += *str - '0';
    str++;
  }
  *arg = result * sign;
  return SUCCESS;
}

size_t getFileSize(FILE * file) {
  struct stat st = {};
  if (fstat(fileno(file), &st) == 0) {
    return (size_t) st.st_size;
  }
  return 0;
}

void getBuffer(FILE * file, Code * code) {
  char symbol = '\0';
  size_t index = 0;
  code->total_command = 1;
  while ((symbol = (char) getc(file)) != EOF) {
    code->buffer[index++] = symbol;
    if (symbol == '\n') {
      code->total_command++;
    }
  }
  code->buffer[index] = '\0';
}

void getArray(Code * code) {
  code->array[0] = code->buffer;
  for (size_t index = 0, strindex = 1; index < code->len; index++) {
    if (code->buffer[index] == '\n') {
      code->buffer[index] = '\0';
      code->array[strindex++] = code->buffer + index + 1;
    }
  }
}

CompileErr fileError() {
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
