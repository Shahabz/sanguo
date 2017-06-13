CC=g++
AR= ar rcu
RANLIB= ranlib

SRC=netcode.cpp \
dserver.cpp \
global_net.cpp \
netserver.cpp

INCLUDE= -I ../include -I ../../LKUtils/include

OBJ= $(SRC:.cpp=.o)
LIB= -lpthread -lm
LIBA= ../../../public/lib/liblknet.a

$(LIBA): $(OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

CFLAGS = -g

netcode.o:	netcode.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
dserver.o:	dserver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
global_net.o:	global_net.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
netserver.o:	netserver.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@

all: $(LIBA)

clean:
	rm -f *.o
	rm -f $(LIBA)
