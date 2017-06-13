CC=g++
SRC = \
netcode.cpp \
dserver.cpp \
global_net.cpp \
netserver.cpp \
client.cpp \
gameproc.cpp \
main.cpp \

INCLUDE = -I ../include  -I  ../../LKUtils/include
LIBRARY = -L ../../../public/lib
OBJ = $(SRC:.cpp=.o)
LIB = -llknet -llkutils -lpthread -ldl -lm
EXE = ../../../exec/globalServer
CFLAGS = -g
$(EXE) : $(OBJ)
	$(CC)  $(CFLAGS) $(INCLUDE) $(LIBRARY) -o $(EXE) $(CF) $(OBJ) $(LIB)

netcode.o:	netcode.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
dserver.o:	dserver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
global_net.o:	global_net.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
netserver.o:	netserver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
client.o: client.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gameproc.o: gameproc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
main.o: main.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

all: $(EXE)
clean :
	rm -f *.o
	rm -f $(EXE)
