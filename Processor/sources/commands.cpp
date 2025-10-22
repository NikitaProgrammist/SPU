#include <stdio.h>
#include <math.h>

#include "stack.h"
#include "read.h"
#include "commands.h"

Errors Pushr(Processor * processor, int value) {
  if (value < 0 || value > 15) {
    return INVALID_REGISTER;
  }
  StackErr result = stackPush(processor->value_stack, processor->regs[value]);
  return toProcessor(result);
}

Errors Popr(Processor * processor, int value) {
  if (value < 0 || value > 15) {
    return INVALID_REGISTER;
  }
  size_t size = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 0) {
    result = stackPop(processor->value_stack, &processor->regs[value]);
    return toProcessor(result);
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Push(Processor * processor, stack_t value) {
  StackErr result = stackPush(processor->value_stack, value);
  return toProcessor(result);
}

Errors Pop(Processor * processor) {
  size_t size = 0;
  stack_t _ = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 0) {
    result = stackPop(processor->value_stack, &_);
    return toProcessor(result);
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Add(Processor * processor) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPush(processor->value_stack, num_1 + num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    return SUCCESS_P;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Sub(Processor * processor) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPush(processor->value_stack, num_2 - num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    return SUCCESS_P;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Mul(Processor * processor) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPush(processor->value_stack, num_1 * num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    return SUCCESS_P;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Div(Processor * processor) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_1 == 0) {
      return DIVIDED_BY_ZERO;
    }
    result = stackPush(processor->value_stack, num_2 / num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    return SUCCESS_P;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Sqrt(Processor * processor) {
  size_t size = 0;
  stack_t num = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 0) {
    result = stackPop(processor->value_stack, &num);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num < 0) {
      return NO_SQRT_NUMBER;
    }
    result = stackPush(processor->value_stack, (stack_t) pow(num, 0.5));
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    return SUCCESS_P;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors In(Processor * processor) {
  stack_t number = 0;
  stack_t res = scanf("%d", &number);
  if (res != 1) {
    return SCANF_ERR;
  }
  StackErr result = stackPush(processor->value_stack, number);
  return toProcessor(result);
}

Errors Out(Processor * processor) {
  stack_t number = 0;
  size_t size = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 0) {
    result = stackPop(processor->value_stack, &number);
    error = toProcessor(result);
    if (error == SUCCESS_P) {
      printf("%d\n", number);
    }
    return error;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Ret(Processor * processor, size_t * command) {
  int number = 0;
  size_t size = 0;
  StackErr result = stackGetSize(processor->func_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 0) {
    result = stackPop(processor->func_stack, &number);
    error = toProcessor(result);
    if (error == SUCCESS_P) {
      *command = number;
    }
    return error;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Jb(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 < num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Jbe(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 <= num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Ja(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 > num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Jae(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 >= num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Je(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 == num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Jne(Processor * processor, int value, size_t * command) {
  size_t size = 0;
  stack_t num_1 = 0, num_2 = 0;
  StackErr result = stackGetSize(processor->value_stack, &size);
  Errors error = toProcessor(result);
  if (error == SUCCESS_P && size > 1) {
    result = stackPop(processor->value_stack, &num_1);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    result = stackPop(processor->value_stack, &num_2);
    if (result != SUCCESS_S) {
      return toProcessor(result);
    }
    if (num_2 != num_1 && value > 0) {
      *command = value;
    }
    return value > 0 ? SUCCESS_P : INCORRECT_VALUE;
  }
  return size <= 0 ? POP_FAILED_P : error;
}

Errors Jmp(int value, size_t * command) {
  if (value > 0) {
    *command = value;
    return SUCCESS_P;
  }
  return INCORRECT_VALUE;
}

Errors Call(Processor * processor, int value, size_t * command) {
  StackErr result = stackPush(processor->func_stack, *command);
  if (result != SUCCESS_S) {
    return toProcessor(result);
  }
  *command = value;
  return SUCCESS_P;
}

Errors toProcessor(StackErr result) {
  switch (result) {
    case SUCCESS_S:
      return SUCCESS_P;
    case ASSERTED_S:
      return ASSERTED_P;
    case CREATE_FAILED_S:
      return STACK_DIED;
    case NULL_POINTER_S:
      return NULL_POINTER_P;
    case POP_FAILED_S:
      return POP_FAILED_P;
    case ALLOC_FAILED_S:
      return ALLOC_FAILED_P;
    case CALLOC_FAILED_S:
      return CALLOC_FAILED_P;
    case REALLOC_FAILED_S:
      return REALLOC_FAILED_P;
    case INCORRECT_EXPRESSION_S:
      return BAD_EXPRESSION;
    case CHECK_FAILED_S:
      return CHECK_FAILED_P;
    default:
      return CHECK_FAILED_P;
  }
}
