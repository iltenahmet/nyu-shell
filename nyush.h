#pragma once

void printPrompt();

int getUserInput(char *buffer, size_t bufsize);

void splitStringBySpace(char *inputString, char ***outputArrayPtr, int *arraySize);

void freeStringArray(char ***stringArrayPtr, int size);

void freeArgumentsAndExit(int argumentCount, ...);

char *getExecutablePath(char *command);

char * getProgramName(char * command);
