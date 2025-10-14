#ifndef COMMANDS_H
#define COMMANDS_H

Errors Pushr(Processor * processor, int value);
Errors Popr(Processor * processor, int value);
Errors Push(Processor * processor, stack_t value);
Errors Pop(Processor * processor);
Errors Add(Processor * processor);
Errors Sub(Processor * processor);
Errors Mul(Processor * processor);
Errors Div(Processor * processor);
Errors Sqrt(Processor * processor);
Errors In(Processor * processor);
Errors Out(Processor * processor);
Errors Ret(Processor * processor, size_t * command);
Errors Jb(Processor * processor, int value, size_t * command);
Errors Jbe(Processor * processor, int value, size_t * command);
Errors Ja(Processor * processor, int value, size_t * command);
Errors Jae(Processor * processor, int value, size_t * command);
Errors Je(Processor * processor, int value, size_t * command);
Errors Jne(Processor * processor, int value, size_t * command);
Errors Jmp(int value, size_t * command);
Errors Call(Processor * processor, int value, size_t * command);
Errors toProcessor(StackErr result);

#endif // COMMANDS_H
