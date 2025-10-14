#include <stdio.h>

#include "stack.h"
#include "read.h"
#include "run.h"
#include "commands.h"

Errors run(Processor * processor) {
  size_t command = 0;
  while (command < processor->commands_count) {
    if (processor->commands[command].number < 0) {
      while (processor->commands[command].number != RET && command < processor->commands_count) {
        command++;
      }
      continue;
    }
    switch (processor->commands[command].number) {
      case PUSHR:
        check(Pushr(processor, processor->commands[command].value));
        break;
      case POPR:
        check(Popr(processor, processor->commands[command].value));
        break;
      case PUSH:
        check(Push(processor, processor->commands[command].value));
        break;
      case POP:
        check(Pop(processor));
        break;
      case ADD:
        check(Add(processor));
        break;
      case SUB:
        check(Sub(processor));
        break;
      case MUL:
        check(Mul(processor));
        break;
      case DIV:
        check(Div(processor));
        break;
      case SQRT:
        check(Sqrt(processor));
        break;
      case IN:
        check(In(processor));
        break;
      case OUT:
        check(Out(processor));
        break;
      case HLT:
        return SUCCESS_P;
        break;
      case RET:
        check(Ret(processor, &command));
        break;
      case JB:
        check(Jb(processor, processor->commands[command].value, &command));
        break;
      case JBE:
        check(Jbe(processor, processor->commands[command].value, &command));
        break;
      case JA:
        check(Ja(processor, processor->commands[command].value, &command));
        break;
      case JAE:
        check(Jae(processor, processor->commands[command].value, &command));
        break;
      case JE:
        check(Je(processor, processor->commands[command].value, &command));
        break;
      case JNE:
        check(Jne(processor, processor->commands[command].value, &command));
        break;
      case JMP:
        check(Jmp(processor->commands[command].value, &command));
        break;
      case CALL:
        processor->commands[processor->commands[command].value].number = -processor->commands[processor->commands[command].value].number;
        check(Call(processor, processor->commands[command].value, &command));
        processor->commands[processor->commands[command].value].number = -processor->commands[processor->commands[command].value].number;
        break;
      default:
        return INCORRECT_COMMAND;
        break;
    }
  }
  return SUCCESS_P;
}
