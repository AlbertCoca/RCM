CC=gcc
CFLAGS=-std=c99 -Wall -g -O3

objects = csr_graph.o linkedQueue.o rma.o

TARGET=rma


all:$(TARGET)

rma: $(objects)
	$(CC) $(CFLAGS) $(CINCS) $+ -o rma

rma.o: rma.c
	$(CC) $(CFLAGS) $(CINCS) $+ -c

csr_graph.o:csr_graph.c
	$(CC) $(CFLAGS) $(CINCS) $+ -c

cqueue.o: cqueue.c
	$(CC) $(CFLAGS) $(CINCS) $+ -c

linkedQueue.o: linkedQueue.c
	$(CC) $(CFLAGS) $(CINCS) $+ -c

test.o: test.c
	$(CC) $(CFLAGS) $(CINCS) $+ -c

clean:
	rm -rf *.o *.bin $(TARGET)

test: linkedQueue.o test.o
	$(CC) $(CFLAGS) $(CINCS) $+ -o testQueue

run:
	./main.bin

.PHONY:all clean run
