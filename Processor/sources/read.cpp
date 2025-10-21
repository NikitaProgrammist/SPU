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
  for (size_t i = 0; i < processor->commands_count; i++) {
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

void errorParser(Errors result) {
  switch (result) {
    case SUCCESS_P:
      break;
    case LONG_NAME:
      printf("Длинное имя файла\n");
      break;
    case NO_FILE:
      printf("Файл не существует\n");
      break;
    case NO_ROOT:
      printf("Нет прав на чтение файла\n");
      break;
    case FILE_DIR:
      printf("Попытка открыть директорию как файл\n");
      break;
    case MAX_FILE_PROCESS:
      printf("Максимальное количество файлов в процессе\n");
      break;
    case MAX_FILE_SYSTEM:
      printf("Максимальное количество файлов в системе\n");
      break;
    case WRITE_ERROR:
      printf("Ошибка при записи в файл\n");
      break;
    case FILES_CONFLICT:
      printf("Конфликт файлов в системе\n");
      break;
    case MEMORY_ERROR:
      printf("Ошибка памяти\n");
      break;
    case DIR_ERROR:
      printf("Ошибка в директории\n");
      break;
    case FILE_ERROR:
      printf("Программа не смогла открыть файл\n");
      break;
    case CREATE_FAILED_P:
      printf("Ошибка при создании\n");
      break;
    case NULL_POINTER_P:
      printf("Нулевой указатель\n");
      break;
    case ALLOC_FAILED_P:
      printf("Ошибка при аллокации\n");
      break;
    case CALLOC_FAILED_P:
      printf("Ошибка при аллокации\n");
      break;
    case REALLOC_FAILED_P:
      printf("Ошибка при реаллокации\n");
      break;
    case STACK_DIED:
      printf("Стек поврежден\n");
      break;
    case POP_FAILED_P:
      printf("Ошибка при удалении элементов из стека\n");
      break;
    case ASSERTED_P:
      break;
    case BAD_EXPRESSION:
      printf("Неправильное выражение\n");
      break;
    case DIVIDED_BY_ZERO:
      printf("Деление на 0\n");
      break;
    case NO_SQRT_NUMBER:
      printf("Отрицательное число под корнем\n");
      break;
    case CHECK_FAILED_P:
      printf("Ошибка при проверке\n");
      break;
    case SCANF_ERR:
      printf("Ошибка при считывании данных\n");
      break;
    case INCORRECT_COMMAND:
      printf("Неправильная команда\n");
      break;
    case INVALID_REGISTER:
      printf("Некорректный регистр\n");
      break;
    case INCORRECT_VALUE:
      printf("Некорректное значение\n");
      break;
    default:
      printf("Ошибка во время выполнения\n");
      break;
  }
}
