CC = gcc
CFLAGS = -Wall -Wextra -std=c11
PTHREAD_FLAGS = -pthread

PROGRAMS = fib_multi fib_single fac_multi fac_single sort
all: $(PROGRAMS)

fib_multi: fib_multi.c utils.c utils.h
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) fib_multi.c utils.c -o fib_multi

fib_single: fib_single.c utils.c utils.h
	$(CC) $(CFLAGS) fib_single.c utils.c -o fib_single

fac_multi: fac_multi.c utils.c utils.h
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) fac_multi.c utils.c -o fac_multi

fac_single: fac_single.c utils.c utils.h
	$(CC) $(CFLAGS) fac_single.c utils.c -o fac_single

sort: sort.c utils.c utils.h
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) sort.c utils.c -o sort

clean:
	rm -f $(PROGRAMS)