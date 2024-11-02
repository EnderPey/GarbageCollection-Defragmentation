mallocTest: duMalloc.o mallocTestVersion1.o
	gcc -o mallocTestVersion1 duMalloc.o mallocTestVersion1.o

duMalloc.o: duMalloc.c dumalloc.h
	gcc -c duMalloc.c

mallocTestVersion1.o: mallocTestVersion1.c dumalloc.h
	gcc -c mallocTestVersion1.c 
 clean:
	rm -f *.o mallocTestVersion1