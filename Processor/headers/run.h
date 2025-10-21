#ifndef RUN_H
#define RUN_H

#define check(arg) {                                          \
  Errors result_ = arg;                                       \
  if (result_ != SUCCESS_P) {                                 \
    printf("Ошибка в строке: %zu %d\n", command, result_);    \
    return result_;                                           \
  }                                                           \
}

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
  RET=49,
  JB,
  JBE,
  JA,
  JAE,
  JE,
  JNE,
  JMP,
  CALL
};

Errors run(Processor * processor);

#endif // RUN_H
