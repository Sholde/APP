CC=mpicc
CFLAGS=-Wall

.PHONY: clean

all: ping pong ping_pong

ping: ping.c
	$(CC) $(CFLAGS) $< -o $@

pong: pong.c
	$(CC) $(CFLAGS) $< -o $@

ping_pong: ping_pong.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *~ ping pong ping_pong
