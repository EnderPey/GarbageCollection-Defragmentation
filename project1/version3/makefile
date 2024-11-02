mallocTestVersion3: duMalloc.o mallocTestVersion3.o
	gcc -o mallocTestVersion3 duMalloc.o mallocTestVersion3.o

duMalloc.o: duMalloc.c dumalloc.h
	gcc -c duMalloc.c

mallocTestVersion3.o: mallocTestVersion3.c dumalloc.h
	gcc -c mallocTestVersion3.c 
	
.PHONY: clean
clean:
	rm -f *.o mallocTestVersion3