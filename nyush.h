void printPrompt();
int getUserInput(char* buffer, size_t bufsize);
char* getBuiltInCommand(char* command);
void splitStringBySpace(char *inputString, char ***outputArrayPtr,  int *arraySize);
void freeStringArray(char ***stringArrayPtr, int size); 
