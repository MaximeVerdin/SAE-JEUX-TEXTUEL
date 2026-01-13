#ifndef UTILS_H
#define UTILS_H

char *readParam(const char *filePath, const char *paramName);
void freeParams(char **params, int count);

#endif
