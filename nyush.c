// nyuc  -- for valgrind: valgrind --leak-check=full --track-origins=yes ./nyush
// docker command for windows: docker run -i --name cs202 --privileged --rm -t -v C:\Users\ahmet\cs202\labs:/cs202 -w /cs202 ytang/os bash

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "unistd.h"
#include "libgen.h"
#include "nyush.h"

int main()
{

	size_t inputSize = 100;
	char *input = (char *)malloc(inputSize * sizeof(char));

	int inputArraySize = 7;
	char **inputArray = malloc(inputArraySize * sizeof(char *));

	while (1)
	{

		printPrompt();
		if (getUserInput(input, inputSize) != EXIT_SUCCESS)
		{
			free(input);
			free(inputArray);	
			exit(0);
		}

		splitStringBySpace(input, &inputArray, &inputArraySize);

		char *builtInCommand = getBuiltInCommand(inputArray[0]);
		if (builtInCommand)
		{
			printf("Built in: %s", builtInCommand);
			if (strcmp(builtInCommand, "exit") == 0)
			{
				free(input);
				free(inputArray);	
				exit(0);
			}
		}
		else
		{
			printf("Not built-in");
		}
	}


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
int getUserInput(char *buffer, size_t bufsize)
{
	if (getline(&buffer, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			// the user has pressed Ctrl+D
			return EXIT_FAILURE;
		}
		else
		{
			perror("Error reading input with getLine()");
			return EXIT_FAILURE;
		}
	}

	// change \n at the end of the buffer to \0 so it's a null terminated string
	buffer[strcspn(buffer, "\n")] = '\0';

	return EXIT_SUCCESS;
}

void splitStringBySpace(char *inputString, char ***outputArrayPtr, int *arraySize)
{
	char **outputArray = *outputArrayPtr;
	int tokenCount = 0;
	char *token = strtok(inputString, " ");

	while (token != NULL)
	{
		if (tokenCount == *arraySize)
		{
			*arraySize *= 2;
			outputArray = realloc(outputArray, sizeof(char *) * *arraySize);
			*outputArrayPtr = outputArray;
		}

		outputArray[tokenCount] = token;
		token = strtok(NULL, " ");
		tokenCount++;
	}
}

char *getBuiltInCommand(char *command)
{
	char *builtInCommands[] = {"cd", "jobs", "fg", "exit"};
	int count = sizeof(builtInCommands) / sizeof(char *);

	for (int i = 0; i < count; i++)
	{
		if (strcmp(command, builtInCommands[i]) == 0)
		{
			return builtInCommands[i];
		}
	}

	return NULL;
}
