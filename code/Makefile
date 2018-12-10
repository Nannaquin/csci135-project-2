
SHELL   = /bin/sh
CC      = g++
EXEC    = solve 

# Sources
SRCS_0 = puzzle.cpp solve.cpp 
SRCS = $(SRCS_0) main.cpp
HDRS = $(SRCS_0:.cpp=.h) 
OBJS = $(SRCS:.cpp=.o)

EXECS = $(EXEC)

CFLAGS += -Wall 
LDLIBS += -lm 

.PHONY: all clean

# Build
all: $(EXECS)

clean:
	-rm -f $(OBJS) $(EXECS)

%.o: %.cpp $(HDRS)  
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(EXEC): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(EXEC)

