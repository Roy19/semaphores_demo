CFLAGS=-o
LDFLAGS=-lrt -pthread

all:
	mkdir bin
	gcc src/producer.c $(CFLAGS) bin/producer $(LDFLAGS)
	gcc src/consumer.c $(CFLAGS) bin/consumer $(LDFLAGS)