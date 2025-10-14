#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#include "to_binary.h"
#include "my_printf.h"

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

CompileErr getLabels(Code * code) {
  size_t command = 0;
  size_t shift = 0;
  while (command < code->total_command) {
    char * str = code->array[command];
    if (str[0] == ':') {
      int arg = 0;
      CompileErr result = checkStr(str + 1, &arg);
      if (result != SUCCESS) {
        Error("error: не числовая метка %s", code->filename, command, str, str);
        return result;
      }
      else if (arg <= 0) {
        Error("error: отрицательная метка %s", code->filename, command, str, str);
        return NEGATIVE_LABEL;
      }
      else if (arg < code->len_labels) {
        code->labels[arg] = command - shift;
      }
      else {
        code->len_labels *= 2;
        code->labels = (int *) realloc(code->labels, code->len_labels * sizeof(int));
      }
      shift++;
    }
    if (str[0] == ';') {
      shift++;
    }
    command++;
  }
  code->real_command = code->total_command - shift;
  return SUCCESS;
}

CompileErr textToCommands(Code * code) {
  size_t command = 0, stack_count = 0;
  while (command < code->total_command) {
    char * str = code->array[command];
    if (*str == '\0') {
      break;
    }
    if (!strncmp(str, "PUSHR", 5)) {
      stack_count++;
      if (str[5] != ' ' || str[6] != 'R' || str[8] != 'X' || 'A' > str[7] || str[7] > 'K') {
        Error("error: некорректный регистр %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      if (str[9] != '\0' && str[10] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 42, .barg = str[7] - 'A'};
    }
    else if (!strncmp(str, "POPR", 4)) {
      if (stack_count == 0) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count--;
      if (str[4] != ' ' || str[5] != 'R' || str[7] != 'X' || 'A' > str[6] || str[6] > 'K') {
        Error("error: некорректный регистр %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      if (str[8] != '\0' && str[9] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 33, .barg = str[7] - 'A'};
    }
    else if (!strncmp(str, "PUSH", 4)) {
      stack_count++;
      int arg = 0;
      CompileErr result = checkStr(str + 4, &arg);
      if (result != SUCCESS) {
        Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
        return result;
      }
      code->commands[command] = {.bname = 1, .barg = arg};
    }
    else if (!strncmp(str, "POP", 3)) {
      if (stack_count == 0) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count--;
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 2, .barg = 0};
    }
    else if (!strncmp(str, "ADD", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 3, .barg = 0};
    }
    else if (!strncmp(str, "SUB", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 4, .barg = 0};
    }
    else if (!strncmp(str, "MUL", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 5, .barg = 0};
    }
    else if (!strncmp(str, "DIV", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 10, .barg = 0};
    }
    else if (!strncmp(str, "SQRT", 4)) {
      if (str[4] != '\0' && str[5] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 6, .barg = 0};
    }
    else if (!strncmp(str, "IN", 2)) {
      stack_count++;
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 7, .barg = 0};
    }
    else if (!strncmp(str, "OUT", 3)) {
      if (stack_count == 0) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count--;
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 8, .barg = 0};
    }
    else if (!strncmp(str, "JB", 2)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[2] == ' ' && str[3] == ':') {
        CompileErr result = checkStr(str + 4, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      if (code->labels[arg] == 0) {
        Error("error: программа ссылается на несуществующую метку %s", code->filename, command, str, str);
        return INCORRECT_LABEL;
      }
      code->commands[command] = {.bname = 50, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JBE", 3)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[3] == ' ' && str[4] == ':') {
        CompileErr result = checkStr(str + 5, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 51, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JA", 2)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[2] == ' ' && str[3] == ':') {
        CompileErr result = checkStr(str + 4, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 52, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JAE", 3)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[3] == ' ' && str[4] == ':') {
        CompileErr result = checkStr(str + 5, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 53, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JE", 2)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[2] == ' ' && str[3] == ':') {
        CompileErr result = checkStr(str + 4, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 54, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JNE", 3)) {
      if (stack_count < 2) {
        Error("error: pop из пустого стека %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      stack_count -= 2;
      int arg = 0;
      if (str[3] == ' ' && str[4] == ':') {
        CompileErr result = checkStr(str + 5, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 55, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "JMP", 3)) {
      int arg = 0;
      if (str[3] == ' ' && str[4] == ':') {
        CompileErr result = checkStr(str + 5, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 56, .barg = code->labels[arg]};
    }
    else if (!strncmp(str, "CALL", 4)) {
      int arg = 0;
      if (str[4] == ' ' && str[5] == ':') {
        CompileErr result = checkStr(str + 6, &arg);
        if (result != SUCCESS) {
          Error("error: некорректный аргумент команды %s", code->filename, command, str, str);
          return result;
        }
      }
      code->commands[command] = {.bname = 57, .barg = code->labels[arg]};
      code->commands[code->labels[arg]].bname = -code->commands[code->labels[arg]].bname;
    }
    else if (!strncmp(str, "RET", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 49, .barg = 0};
    }
    else if (!strncmp(str, "HLT", 3)) {
      if (str[3] != '\0' && str[4] != ';') {
        Error("error: некорректный лишний аргумент команды %s", code->filename, command, str, str);
        return INCORRECT_ARG;
      }
      code->commands[command] = {.bname = 9, .barg = 0};
    }
    else if (str[0] == ':') {
      command++;
      continue;
    }
    else if (str[0] == ';') {
      code->commands[command] = {.bname = 0, .barg = 0};
    }
    else {
      Error("error: некорректная команда %s", code->filename, command, str, str);
      return UNKNOWN_COMMAND;
    }
    command++;
  }
  if (code->commands[code->total_command - 2].bname != 9) {
    Error("error: забыли HLT в конце программы %s", code->filename, command - 2, code->array[command - 2], code->array[command - 2]);
    return NO_HLT;
  }
  const int maxsize = 300;
  const char * out_dir = "../Processor/";
  char output_file[maxsize] = {};
  snprintf(output_file, maxsize, "%s%s", out_dir, code->filename);
  return commandsToFile(code, output_file);
}

CompileErr checkStr(char * str, int * arg) {
  int result = 0;
  while (*str != '\0' && *str != ';') {
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
  *arg = result;
  return SUCCESS;
}

CompileErr commandsToFile(Code * code, char * bfilename) {
  FILE * file = fopen(bfilename, "w");
  if (file == NULL) {
    return fileError();
  }
  fprintf(file, "%zu\n", code->real_command);
  for (size_t i = 0; i < code->total_command; i++) {
    if (code->commands[i].bname != 0) {
      if (code->commands[i].bname == 42 || code->commands[i].bname == 33 || code->commands[i].bname == 1 || code->commands[i].bname >= 50) {
        fprintf(file, "%d %d\n", code->commands[i].bname, code->commands[i].barg);
      }
      else {
        fprintf(file, "%d\n", code->commands[i].bname);
      }
    }
  }
  fclose(file);
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
