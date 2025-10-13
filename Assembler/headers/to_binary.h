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
  NO_HLT
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
  size_t len_labels;
  size_t len;
  size_t total_command;
  size_t real_command;
};

CompileErr convert(char * filename);
void Destroy(Code * code);
CompileErr getLabels(Code * code);
CompileErr textToCommands(Code * code);
CompileErr commandsToFile(Code * code, char * bfilename);
CompileErr checkStr(char * str, int * arg);
size_t getFileSize(FILE * file);
void getBuffer(FILE * file, Code * code);
void getArray(Code * code);
CompileErr fileError();

#endif // ASM_H
