// nyush.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include "nyush.h"
#include "builtInCommands.h"

int main()
{
	// ignore signals
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	while (1)
	{
		size_t inputSize = 100;
		char *input = (char *)malloc(inputSize * sizeof(char));

		int inputArraySize = 1; // We dynamically increase array size as we add elements
		char **inputArray = malloc(inputArraySize * sizeof(char *));

		printPrompt();
		if (getUserInput(input, inputSize) != EXIT_SUCCESS)
		{
			freeArgumentsAndExit(2, input, inputArray);
		}

		splitStringBySpace(input, &inputArray, &inputArraySize);

		char *builtInCommand = getBuiltInCommand(inputArray[0]);

		if (builtInCommand)
		{
			if (executeBuiltInCommand(inputArray, inputArraySize) == EXIT_FAILURE)
			{
				freeArgumentsAndExit(2, input, inputArray);
			}

			continue;
		}

		// command is not a built-in command
		char *executablePath = getExecutablePath(inputArray[0]);

		// create args
		char **args = malloc((inputArraySize + 1) * sizeof(char *));
		args[0] = inputArray[0];
		for (int i = 1; i < inputArraySize; i++)
		{
			args[i] = inputArray[i];
		}
		args[inputArraySize] = NULL;

		pid_t pid = fork();
		if (pid < 0) // fork failed
		{
			fprintf(stderr, "Fork failed");
		}
		else if (pid == 0) // child process
		{
			char *file;
			int operatorCount;
			for (int i = 0; i < inputArraySize; i++) 
			{ 
				// check for ">" or ">>" or "<"
				bool create = strcmp(inputArray[i], ">") == 0 ? true : false;
				bool append = strcmp(inputArray[i], ">>") == 0 ? true : false;
				bool inputRedirect = strcmp(inputArray[i], "<") == 0 ? true : false;

				if (create || append || inputRedirect) //handle i/o redirection
				{
					operatorCount++;
					file = inputArray[i + 1];
					if (file == NULL)
					{
						fprintf(stderr, "Error: invalid command\n");
						exit(-1);
					}

					// modify args to only include part before ">" ">>" or "<"
					args[i] = NULL;
					
					int fd;
					if (inputRedirect)
					{
						int fd = open(file, O_RDONLY);
						if (fd == -1)
						{
							fprintf(stderr, "Error: invalid file\n");
							exit(-1);	
						}

						dup2(fd, 0); //duplicate the file descriptor to standard input
						close(fd);
						
						continue;
					}

					// output redirect
					if (create) 
					{
						fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
					} 
					else //append
					{
						fd = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
						if (fd == -1) 
						{
							fprintf(stderr, "Error: invalid file\n");
							exit(-1);
						}
					}

					dup2(fd, 1); // duplicate the file descriptor to standard output
					close(fd);

					// After this point any calls to stdout will write to "file";
					continue;
				}


			}

			execv(executablePath, args);

			// TO DO: check if we have access to file with access()

			// execv error
			fprintf(stderr, "Error: invalid program\n");
			exit(-1);
		}
		else // parent process
		{
			int status;
			waitpid(-1, &status, 0);
		}

		free(args);
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
		if (!feof(stdin))
		{
			perror("Error reading input with getLine()");
		}

		// the user has pressed Ctrl+D
		return EXIT_FAILURE;
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

int outputRedirection(char ***inputArray, int size, char ***args)
{
	char *file;
	for (int i = 0; i < size; i++)
	{
		if (strcmp(*inputArray[i], ">") == 0)
		{
			file = *inputArray[i + 1];
			if (file == NULL)
			{
				fprintf(stderr, "Error: invalid command");
			}

			// modify args to only include part before >
			*args[i] = NULL;
			break;
		}

		if (i == size - 1) // there is no ">"
		{
			return EXIT_FAILURE;
		}
	}

	int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	dup2(fd, 1); // duplicate the file descriptor
	close(fd);	 // close the unused file descriptor

	// After this point any calls to stdout will write to "file"
	return EXIT_SUCCESS;
}
