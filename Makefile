CC       = gcc
CFLAGS   = -Iinclude -Wall -Wextra -Wpedantic
LDFLAGS  = -lpthread

BINDIR   = bin

SERVER_SRC = src/server/server.c src/server/server_network.c src/server/server_outputs.c
CLIENT_SRC = src/client/client.c src/client/interface.c src/client/client_outputs.c src/client/client_network.c

SERVER = $(BINDIR)/server
CLIENT = $(BINDIR)/client

.PHONY: all server client run-server run-client clean

all: server client

$(BINDIR):
	mkdir -p $(BINDIR)

server: $(BINDIR) $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER)

client: $(BINDIR) $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) $(LDFLAGS) -o $(CLIENT)

run-server: server
	./$(SERVER)

run-client: client
	./$(CLIENT)

clean:
	rm -rf $(BINDIR)
