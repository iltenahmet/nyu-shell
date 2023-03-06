#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "builtInCommands.h"

char *builtInCommands[] = {"cd", "jobs", "fg", "exit"};

char *getBuiltInCommand(char *command)
{
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

int executeBuiltInCommand(char **inputArray)
{
    char *command = inputArray[0];

    if (strcmp(command, "cd") == 0)
    {
        if (chdir(inputArray[1]) != 0)
        {
            perror("Error changing directory");
        }
        return EXIT_SUCCESS;
    }

    if (strcmp(command, "exit") == 0)
    {
        return EXIT_FAILURE;
    }

    // TO DO: Add fg, and jobs

    return EXIT_SUCCESS;
}
