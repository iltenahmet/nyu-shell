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
	enum inputStatus status;

	do 
	{
		printPrompt();
		status = getUserInput(input, inputSize);
	}
	while (status != INPUT_EXIT && status != INPUT_EOF);

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
enum inputStatus getUserInput(char* buffer, size_t bufsize)
{
	if (getline(&buffer, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			// the user has pressed Ctrl+D
			return INPUT_EOF; 
		} 
		else
		{
			perror("Error reading input with getLine()");
			return INPUT_ERROR;
		} 
	}

	//change \n at the end of the buffer to \0 so it's a null terminated string
	buffer[strcspn(buffer, "\n")] = '\0';

	if (strcmp(buffer, "exit") == 0)
	{
		return INPUT_EXIT;
	}

	return INPUT_OK;
}

bool isInvalid(char* input)
{
	//for now the only command I've implemented is exit, I'll change this later
	if (strcmp(input, "exit") == 0)
	{
		return false;		
	}
	return true;
}
