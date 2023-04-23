PROJ=/mnt/d/HSC/Training-Project/Refurbished
BIN=$(PROJ)/bin
SERVER=$(PROJ)/server/
CLIENT=$(PROJ)/client/
COMMON=$(PROJ)/common/
CC=g++
CPPFLAGS=-g -Wall -std=c++17 -I$(COMMON)/include
LFLAGS=-lpthread

COMMON_OBJS=$(COMMON)/src/cmdExec.o
SERVER_OBJS=$(SERVER)/src/server_main.o
CLIENT_OBJS=$(CLIENT)/src/client_main.o

all: $(BIN)/server $(BIN)/client $(COMMON_OBJS)

$(BIN)/server: $(SERVER_OBJS) $(COMMON_OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

$(BIN)/client : $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o : %.c
	$(CC) -c $(CPPFLAGS) $< -o $@

clean:
	\rm -f $(BIN)/server $(BIN)/client
	\rm `find $(PROJ) -name '*.o'`
