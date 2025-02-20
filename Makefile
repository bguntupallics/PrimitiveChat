CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wpedantic
SERVERSRC = src/server/server.c src/server/parser.c
CLIENTSRC = src/client/client.c src/client/interface.c src/client/client_outputs.c
client = bin/client
server = bin/server

all: server client

client: $(CLIENTSRC)
	$(CC) $(CFLAGS) $(CLIENTSRC) -o $(client)

server: $(SERVERSRC)
	$(CC) $(CFLAGS) $(SERVERSRC) -o $(server)

clean:
	rm -rf bin/*