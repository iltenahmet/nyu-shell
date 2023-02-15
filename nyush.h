void printPrompt();
enum inputStatus {INPUT_OK, INPUT_EOF, INPUT_EXIT, INPUT_ERROR};
enum inputStatus getUserInput(char* buffer, size_t bufsize);
bool isInvalid(char* input);
