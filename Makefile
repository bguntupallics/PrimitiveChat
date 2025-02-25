CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wpedantic
SERVERSRC = src/server/server.c src/server/server_network.c
CLIENTSRC = src/client/client.c src/client/interface.c src/client/client_outputs.c src/client/client_network.c
client = bin/client
server = bin/server

all: server client

client: $(CLIENTSRC)
	$(CC) $(CFLAGS) $(CLIENTSRC) -o $(client)

server: $(SERVERSRC)
	$(CC) $(CFLAGS) $(SERVERSRC) -o $(server)

rc: $(client)
	./bin/client

rs: $(server)
	./bin/server

clean:
	rm -rf bin/*