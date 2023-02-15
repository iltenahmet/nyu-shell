//nyuc  -- for valgrind: valgrind --leak-check=full --track-origins=yes ./nyush

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "unistd.h"
#include "libgen.h"
#include "nyush.h"

int main()
{
	size_t inputSize = 32;
	char* input = (char *)malloc(inputSize * sizeof(char));
	bool inputError = false;

	do 
	{
		printPrompt();
		getUserInput(input, inputSize, &inputError);	
	}
	while (strcmp(input, "exit") != 0 && !inputError);

	free(input);
}

void printPrompt()
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
	}

	printf("[nyush %s]$ ", basename(cwd));
	fflush(stdout);
}

// https://c-for-dummies.com/blog/?p=1112
void getUserInput(char* buffer, size_t bufsize, bool* inputError)
{
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	if (getline(&buffer, &bufsize, stdin) == -1)
	{
		*inputError = true;
	}

	buffer[strcspn(buffer, "\n")] = '\0';
}
