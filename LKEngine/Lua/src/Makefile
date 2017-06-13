CC=gcc
AR= ar rcu
RANLIB= ranlib

SRC=lapi.c \
lauxlib.c \
lbaselib.c \
lbitlib.c \
lcode.c \
lcorolib.c \
lctype.c \
ldblib.c \
ldebug.c \
ldo.c \
ldump.c \
lfunc.c \
lgc.c \
linit.c \
liolib.c \
llex.c \
lmathlib.c \
lmem.c \
loadlib.c \
lobject.c \
lopcodes.c \
loslib.c \
lparser.c \
lstate.c \
lstring.c \
lstrlib.c \
ltable.c \
ltablib.c \
ltm.c \
lundump.c \
lvm.c \
lzio.c

INCLUDE= -I ../include

OBJ= $(SRC:.c=.o)
LIB= -lpthread -lm
LIBA= ../../../public/lib/liblua.a

$(LIBA): $(OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

#CFLAGS = -g

lapi.o:	lapi.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lauxlib.o:	lauxlib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lbaselib.o:	lbaselib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lbitlib.o:	lbitlib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lcode.o:	lcode.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lcorolib.o:	lcorolib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lctype.o:	lctype.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ldblib.o:	ldblib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ldebug.o:	ldebug.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ldo.o:	ldo.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ldump.o:	ldump.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lfunc.o:	lfunc.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lgc.o:	lgc.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
linit.o:	linit.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
liolib.o:	liolib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
llex.o:	llex.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lmathlib.o:	lmathlib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lmem.o:	lmem.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
loadlib.o:	loadlib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lobject.o:	lobject.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lopcodes.o:	lopcodes.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
loslib.o:	loslib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lparser.o:	lparser.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lstate.o:	lstate.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lstring.o:	lstring.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lstrlib.o:	lstrlib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ltable.o:	ltable.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ltablib.o:	ltablib.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
ltm.o:	ltm.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lundump.o:	lundump.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lvm.o:	lvm.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
lzio.o:	lzio.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@

all: $(LIBA)

clean:
	rm -f *.o
	rm -f $(LIBA)
