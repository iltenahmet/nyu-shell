#nyush

To use valgrind to check for memory leaks and track origins:

valgrind --leak-check=full --track-origins=yes ./nyush

To run a Docker container on Windows:

docker run -i --name cs202 --privileged --rm -t -v C:\Users\ailte\OS\labs:/cs202 -w /cs202 ytang/os bash

To run a Docker container on Mac:

docker run -i --name cs202 --privileged --rm -t -v /Users/ahmetilten/OS/labs:/cs202 -w /cs202 ytang/os bash

To zip specific files:

zip nyush.zip Makefile *.h *.c
