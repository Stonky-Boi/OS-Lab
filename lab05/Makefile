CC = gcc
CFLAGS = -Wall -Wextra -std=c11
PROGRAMS = q1_shared_memory q2_execve q1_pipe q2_pipe q1_message_queue q2_message_queue

all: $(PROGRAMS)

q1_shared_memory: q1_shared_memory.c utils.c utils.h
	$(CC) $(CFLAGS) q1_shared_memory.c utils.c -o q1_shared_memory

q2_execve: q2_execve.c utils.c utils.h
	$(CC) $(CFLAGS) q2_execve.c utils.c -o q2_execve

q1_pipe: q1_pipe.c utils.c utils.h
	$(CC) $(CFLAGS) q1_pipe.c utils.c -o q1_pipe

q2_pipe: q2_pipe.c utils.c utils.h
	$(CC) $(CFLAGS) q2_pipe.c utils.c -o q2_pipe

q1_message_queue: q1_message_queue.c utils.c utils.h
	$(CC) $(CFLAGS) q1_message_queue.c utils.c -o q1_message_queue

q2_message_queue: q2_message_queue.c utils.c utils.h
	$(CC) $(CFLAGS) q2_message_queue.c utils.c -o q2_message_queue

clean:
	rm -f $(PROGRAMS)