mallocTestVersion2: duMalloc.o mallocTestVersion2.o
	gcc -o mallocTestVersion2 duMalloc.o mallocTestVersion2.o

duMalloc.o: duMalloc.c dumalloc.h
	gcc -c duMalloc.c

mallocTestVersion2.o: mallocTestVersion2.c dumalloc.h
	gcc -c mallocTestVersion2.c 
	
.PHONY: clean
clean:
	rm -f *.o mallocTestVersion2