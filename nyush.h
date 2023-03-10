#pragma once

void printPrompt();

int getUserInput(char *buffer, size_t bufsize);

void splitStringBySpace(char *inputString, char ***outputArrayPtr, int *arraySize);

void freeArgumentsAndExit(int argumentCount, ...);

char *getExecutablePath(char *command);

char * getProgramName(char * command);

int outputRedirection(char ***inputArray, int size, char ***args);
