#ifndef TO_BINARY_H
#define BINARY_H

#define Error(comment, file, line, arg, text) {                                \
      colorPrintf(RED, BOLD, comment, arg);                                    \
      colorPrintf(RED, BOLD, "\n%s:%d\n\n %s", file, line + 1, text);          \
}

enum CompileErr {
  SUCCESS,
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
  CREATE_FAILED,
  NULL_POINTER,
  ALLOC_FAILED,
  CALLOC_FAILED,
  REALLOC_FAILED,
  UNKNOWN_COMMAND,
  INCORRECT_ARG,
  INCORRECT_LABEL,
  NEGATIVE_LABEL,
  TOO_BIG_LABEL,
  NO_HLT
};

enum Commands {
  PUSHR=42,
  POPR=33,
  PUSH=1,
  POP,
  ADD,
  SUB,
  MUL,
  SQRT,
  IN,
  OUT,
  HLT,
  DIV,
  RET=43,
  DRAW,
  PUSHM,
  POPM,
  JB,
  JBE,
  JA,
  JAE,
  JE,
  JNE,
  JMP,
  CALL
};

struct Command {
  int bname;
  int barg;
};

struct Code {
  char * filename;
  char * buffer;
  char ** array;
  struct Command * commands;
  int * labels;
  int last_ret;
  size_t len_labels;
  size_t len;
  size_t total_command;
  size_t real_command;
};

CompileErr getLabels(Code * code);
CompileErr textToCommands(Code * code);
CompileErr commandsToFile(Code * code, char * bfilename);

#endif // ASM_H
