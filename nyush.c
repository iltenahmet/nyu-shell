#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "unistd.h"
#include "libgen.h"
#include "nyush.h"

int main()
{
	// size_t inputSize = 32;
	// char* input = (char *)malloc(inputSize * sizeof(char));

	// while (true)
	// {
	// 	printPrompt();
	// 	getUserInput(input, inputSize);	

	// 	if (strcmp(input, "exit") == 0)
	// 	{
	// 		break;
	// 	}
	// }

	// free(input);

	printPrompt();
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
void getUserInput(char* buffer, size_t bufsize)
{
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

	getline(&buffer, &bufsize, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
}
