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

int executeBuiltInCommand(char **inputArray, int size)
{
    char *command = inputArray[0];

    if (strcmp(command, "cd") == 0)
    {
        if (size != 2)
        {
            fprintf(stderr, "Error: invalid command\n");
            return EXIT_SUCCESS;
        }

        if (chdir(inputArray[1]) != 0)
        {
            fprintf(stderr, "Error: invalid directory\n");
        }

        return EXIT_SUCCESS;
    }

    if (strcmp(command, "exit") == 0)
    {
        // TO DO: check if there are suspended jobs, if so print "Error: there are suspended jobs" and return EXIT_SUCCESS
    
        if (size > 1)
        {
            fprintf(stderr, "Error: invalid command\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    // TO DO: Add fg, and jobs

    return EXIT_SUCCESS;
}
