#ifndef READ_H
#define READ_H

struct Command {
  int number;
  int value;
};

struct Processor {
  struct Stack * value_stack;
  struct Stack * func_stack;
  struct Command * commands;
  stack_t regs[16];
  size_t commands_count;
};

enum Errors {
  SUCCESS_P,
  LONG_NAME,
  NO_FILE,
  NO_ROOT,
  FILE_DIR,
  MAX_FILE_PROCESS,
  MAX_FILE_SYSTEM,
  WRITE_ERROR,
  FILES_CONFLICT,
  MEMORY_ERROR,
  DIR_ERROR,
  FILE_ERROR,
  CREATE_FAILED_P,
  NULL_POINTER_P,
  ALLOC_FAILED_P,
  CALLOC_FAILED_P,
  REALLOC_FAILED_P,
  STACK_DIED,
  POP_FAILED_P,
  ASSERTED_P,
  BAD_EXPRESSION,
  DIVIDED_BY_ZERO,
  NO_SQRT_NUMBER,
  CHECK_FAILED_P,
  SCANF_ERR,
  INCORRECT_COMMAND,
  INVALID_REGISTER,
  INCORRECT_VALUE
};

Errors readFile(Processor * processor, char * filename);
Errors getCommands(Processor * processor, char * filename);
Errors fileError();

#endif // READ_H
