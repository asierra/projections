CC = g++
CFLAGS = -c -Wall -g
PROG = projections

SRCS = projections.cpp glad.c
OBJECTS=$(SRCS:.cpp=.o)
HEADERS=$(SRCS:.cpp=.h)

ifeq ($(shell uname),Darwin)
	LIBS = -framework Cocoa -framework OpenGL -lglfw -framework IOKit
else
	LIBS = -lglfw -lGL -ldl
endif

all: $(PROG)

$(PROG):	$(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROG) *.o *~ shaders/*~

