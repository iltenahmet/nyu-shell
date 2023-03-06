// nyuc  -- for valgrind: valgrind --leak-check=full --track-origins=yes ./nyush
// docker command for windows: docker run -i --name cs202 --privileged --rm -t -v C:\Users\ahmet\cs202\labs:/cs202 -w /cs202 ytang/os bash
// docker command for mac: docker run -i --name cs202 --privileged --rm -t -v /Users/ahmetilten/cs202/labs:/cs202 -w /cs202 ytang/os bash
// zip: zip nyush.zip Makefile *.h *.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdarg.h>
#include <sys/wait.h>
#include "nyush.h"
#include "builtInCommands.h"

int main()
{

	size_t inputSize = 100;
	char *input = (char *)malloc(inputSize * sizeof(char));

	int inputArraySize = 1; //We dynamically increase array size as we add elements
	char **inputArray = malloc(inputArraySize * sizeof(char *));

	while (1)
	{

		printPrompt();
		if (getUserInput(input, inputSize) != EXIT_SUCCESS)
		{
			freeArgumentsAndExit(2, input, inputArray);
		}

		splitStringBySpace(input, &inputArray, &inputArraySize);

		char *builtInCommand = getBuiltInCommand(inputArray[0]);

		if (builtInCommand)
		{
			if (executeBuiltInCommand(inputArray) == EXIT_FAILURE)
			{
				freeArgumentsAndExit(2, input, inputArray);
			}
		}

		// not a built-in command
		char *executablePath = getExecutablePath(inputArray[0]);
		char *programName = getProgramName(inputArray[0]);

		char *args[inputArraySize+1];
		args[0] = programName;
		for (int i = 1; i < inputArraySize; i++)
		{
			args[i] = inputArray[i];
		}
		args[inputArraySize] = NULL;

		pid_t pid = fork();

		if (pid < 0)
		{
			// fork failed (this shouldn't happen)
		}
		else if (pid == 0) // child process
		{
			execv(executablePath, args);

			//execv error
			fprintf(stderr, "Error: invalid program");
			exit(0);
		}
		else // parent
		{
			int status;
			waitpid(-1, &status, 0);
		}

		free(programName);
		free(executablePath);
		
	}
}

void printPrompt()
{
	fflush(stdout);
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
			(*arraySize)++;
			outputArray = realloc(outputArray, sizeof(char *) * *arraySize);
			*outputArrayPtr = outputArray;
		}

		outputArray[tokenCount] = token;
		token = strtok(NULL, " ");
		tokenCount++;
	}
}

void freeArgumentsAndExit(int argumentCount, ...)
{
	va_list args;
	va_start(args, argumentCount);

	for (int i = 0; i < argumentCount; i++)
	{
		void *ptr = va_arg(args, void *);
		free(ptr);
	}

	va_end(args);
	exit(0);
}

char *getExecutablePath(char *command)
{
	if (strchr(command, '/') == 0) // Just the base name
	{
		char *path = "/bin";

		char *result = (char *)malloc(strlen(path) + strlen(command) + 2);
		sprintf(result, "%s/%s", path, command);

		return result;
	}

	if (command[0] == '/') // abosolute path
	{
		char *result = (char *)malloc(strlen(command) + 1); // return a new string to avoid double free() errors
		strcpy(result, command);

		return result;
	}

	// relative path
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
	}

	char *result = (char *)malloc(strlen(cwd) + strlen(command) + 2);
	sprintf(result, "%s/%s", cwd, command);

	return result;
}

char *getProgramName(char *command)
{
	char *delimiter = strrchr(command, '/');

	if (delimiter == NULL) // only base name
	{
		char *result = (char *)malloc(strlen(command) + 1); // return a new string to avoid double free() errors
		strcpy(result, command);

		return result;
	}

	int length = delimiter - command;
	char *result = malloc(length + 1);

	strncpy(result, command, length);
	result[length] = '\0';

	return result;
}
