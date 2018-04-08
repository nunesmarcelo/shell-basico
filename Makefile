
# A simple hack to check if we are on Windows or not (i.e. are we using mingw32-make?)
ifeq ($(findstring mingw32, $(MAKE)), mingw32)
WINDOWS=1
endif

# Compiler settings
CC = gcc
ifeq ($(findstring gcc, $(CC)), gcc)
CFLAGS = -W  -std=c11 -O3 -lncurses
else
CFLAGS = -W -O3 -c
endif
LFLAGS =
LIBS =

# Non-windows systems need pthread and rt
ifndef WINDOWS
ifeq ($(shell uname), Darwin)
LIBS += -pthread -lncurses
else
LIBS += -pthread -lrt -lncurses
endif
endif

# MinGW32 GCC 4.5 link problem fix
ifdef WINDOWS
ifeq ($(findstring 4.5.,$(shell g++ -dumpversion)), 4.5.)
LFLAGS += -static-libgcc
endif
endif

# Misc. system commands
ifdef WINDOWS
RM = del /Q
else
RM = rm -f
endif

# File endings
ifdef WINDOWS
EXE = .exe
else
EXE =
endif

# Object files for the test program
TEST_OBJS = meutop.o

# TinyCThread object files
TINYCTHREAD_OBJS = tinycthread.o

all:  meutop$(EXE)

clean: $(RM) meutop$(EXE) $(TEST_OBJS) $(TINYCTHREAD_OBJS)

check: all
	./meutop$(EXE)

meutop$(EXE): $(TEST_OBJS) $(TINYCTHREAD_OBJS)
	$(CC) $(LFLAGS) -o $@ $(TEST_OBJS) $(TINYCTHREAD_OBJS) $(LIBS)




%.o: %.cpp
	$(CC) $(CFLAGS) $<

%.o: ../source/%.c
	$(CC) $(CFLAGS) $<

meutop.o: meutop.c tinycthread.h
tinycthread.o: tinycthread.c tinycthread.h
