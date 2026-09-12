CC ?= cc

CFLAGS ?= -Wall -Wextra -Wpedantic
CFLAGS += -std=c11

PROG = clout
SRCS = clout.c color.c
OBJS = $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
rm -f $(PROG) $(OBJS)

.PHONY: all clean

