CC = gcc
CFLAGS = -Wall -g -O2

SERVER = server
CLIENT = client
COMMON = common


SERVER_SRC = $(wildcard $(SERVER)/source/*.c)
CLIENT_SRC = $(wildcard $(CLIENT)/source/*.c)

SERVER_OBJ = $(patsubst $(SERVER)/source/%.c,$(SERVER)/object/%.o,$(SERVER_SRC))
CLIENT_OBJ = $(patsubst $(CLIENT)/source/%.c,$(CLIENT)/object/%.o,$(CLIENT_SRC))

SERVER_BIN = $(SERVER)/bin/server
CLIENT_BIN = $(CLIENT)/bin/client

SERVER_INC = $(wildcard $(SERVER)/include/*.h,$(COMMON)/include/*.h)
CLIENT_INC = $(wildcard $(CLIENT)/include/*.h,$(COMMON)/include/*.h)

all: client server

$(shell mkdir -p $(SERVER)/object)
$(shell mkdir -p $(SERVER)/bin)
$(shell mkdir -p $(CLIENT)/object)
$(shell mkdir -p $(CLIENT)/bin)

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_OBJ)

$(CLIENT)/object/%.o: $(CLIENT)/source/%.c $(CLIENT_INC)
	$(CC) $(CFLAGS) -c -o $@ $<

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJ)

$(SERVER)/object/%.o: $(SERVER)/source/%.c $(SERVER_INC)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_BIN) $(CLIENT_BIN)

