#include "stdio.h"
#include "unistd.h"
#include "libgen.h"

int main()
{
   char cwd[256];
   if (getcwd(cwd, sizeof(cwd)) == NULL)
   {
      perror("getcwd() error");
      return 1;
   }
   char *cwdBase = basename(cwd);
   printf("[nyush %s]$", cwdBase);
   fflush(stdout);
}
