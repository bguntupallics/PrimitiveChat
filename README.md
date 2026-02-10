# PrimitiveChat

A lightweight TCP chat server and client written in C. Supports multiple simultaneous clients, direct messaging, nicknames, and automatic keep-alive heartbeats.

## Features

- **Multi-client server** using `select()` for multiplexed I/O (up to 64 concurrent clients)
- **Direct messaging** between connected users
- **Nickname system** with automatic assignment (`user1`, `user2`, ...) and rename support
- **Heartbeat keep-alive** — clients send a pulse every 5 seconds; server removes stale connections after 10 seconds
- **Graceful shutdown** — handles `SIGINT` (Ctrl+C) and `SIGPIPE` cleanly

## Building

```bash
make            # Build both server and client
make server     # Build server only
make client     # Build client only
```

## Running

Start the server first, then connect one or more clients:

```bash
# Terminal 1 — start the server
make run-server

# Terminal 2+ — start clients
make run-client
```

The server listens on port **45556** and clients connect to `127.0.0.1`.

## Client Commands

| Command | Description |
|---|---|
| `help` | Show available commands |
| `connect` | Connect to the server |
| `list` | List all connected users |
| `message <name> <text>` | Send a direct message to a user |
| `echo <name> <text>` | Send a message and receive a copy back |
| `nickname <name>` | Change your nickname (max 20 characters) |
| `leave` | Disconnect from the server |
| `exit` | Disconnect (if connected) and quit |

## Protocol

Communication uses a simple binary protocol over TCP:

| Command | Value | Description |
|---|---|---|
| `ACK` | `0x01` | Acknowledgment |
| `CONNECT` | `0x02` | Client connection request |
| `DISCONNECT` | `0x03` | Client disconnect |
| `HEARTBEAT` | `0x04` | Keep-alive ping |
| `LIST_USERS` | `0x05` | Request user list |
| `NICKNAME` | `0x06` | Change nickname |
| `MESSAGE` | `0x07` | Send a message |
| `INVALID` | `0x08` | Error / invalid request |

## Project Structure

```
├── include/            Header files
│   ├── global.h            Shared enums, structs, and constants
│   ├── server_network.h    Server network function declarations
│   ├── server_outputs.h    Server logging declarations
│   ├── client_network.h    Client network function declarations
│   ├── client_outputs.h    Client UI output declarations
│   └── interface.h         Client command parser declarations
├── src/
│   ├── server/
│   │   ├── server.c            Server entry point
│   │   ├── server_network.c    Connection handling, message routing, heartbeats
│   │   └── server_outputs.c    Server-side logging
│   └── client/
│       ├── client.c            Client entry point
│       ├── client_network.c    Connection, messaging, heartbeat thread
│       ├── client_outputs.c    Client-side UI output
│       └── interface.c         Command parsing and main event loop
├── Makefile
└── README.md
```
