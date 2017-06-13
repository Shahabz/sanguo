CC=g++
AR= ar rcu
RANLIB= ranlib
SRC=thread.cpp \
utils.cpp \
mytimer.cpp \
http.cpp \
share_memory.cpp

INCLUDE= -I ../include

OBJ= $(SRC:.cpp=.o)
LIB= -lpthread -lm
LIBA= ../../../public/lib/liblkutils.a

$(LIBA): $(OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

CFLAGS = -g

thread.o:	thread.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
utils.o:	utils.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
mytimer.o:	mytimer.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
http.o:	http.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
share_memory.o:	share_memory.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
	
all: $(LIBA)

clean:
	rm -f *.o
	rm -f $(LIBA)
