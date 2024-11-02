mallocTestVersion4: duMalloc.o mallocTestVersion4.o
	gcc -o mallocTestVersion4 duMalloc.o mallocTestVersion4.o

duMalloc.o: duMalloc.c dumalloc.h
	gcc -c duMalloc.c

mallocTestVersion4.o: mallocTestVersion4.c dumalloc.h
	gcc -c mallocTestVersion4.c 
	
.PHONY: clean
clean:
	rm -f *.o mallocTestVersion4