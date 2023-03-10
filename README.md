#nyush
for valgrind: valgrind --leak-check=full --track-origins=yes ./nyush
docker command for windows: docker run -i --name cs202 --privileged --rm -t -v C:\Users\ailte\OS\labs:/cs202 -w /cs202 ytang/os bash      
docker command for mac: docker run -i --name cs202 --privileged --rm -t -v /Users/ahmetilten/cs202/labs:/cs202 -w /cs202 ytang/os bash
zip: zip nyush.zip Makefile *.h *.c
