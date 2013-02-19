CC = gcc
CFLAGS = -O3 -march=native -fstack-protector-all

SRCS = src/yeganesh_exec.c
OBJS = ${SRCS:.c=.o}
TARGET = bin/yeganesh_exec

$(TARGET): $(OBJS)
	mkdir -p bin
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf bin $(OBJS)
