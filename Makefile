default: test-malloc malloc.so malloc-debug.so

test-malloc: test-malloc.c
	clang 	-Wall \
			-Werror \
			-o test-malloc \
			test-malloc.c

malloc.so: malloc.c
	clang 	-Wall \
			-shared \
			-fPIC \
			-o malloc.so \
			malloc.c

malloc-debug.so: malloc.c
	clang 	-Wall \
			-shared \
			-fPIC \
			-o malloc-debug.so \
			-DDEBUG=1 \
			malloc.c

valgrind: test-malloc
	valgrind -v ./test-malloc

test: malloc-debug.so test-malloc
	LD_PRELOAD=./malloc-debug.so ./test-malloc

clean:
	rm malloc.so malloc-debug.so test-malloc
