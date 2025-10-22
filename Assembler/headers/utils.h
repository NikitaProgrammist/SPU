#ifndef UTILS_H
#define UTILS_H

CompileErr convert(char * filename);
void Destroy(Code * code);
CompileErr checkStr(char * str, int * arg);
size_t getFileSize(FILE * file);
void getBuffer(FILE * file, Code * code);
void getArray(Code * code);
CompileErr fileError();

#endif // UTILS_H
