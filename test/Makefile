CC=cc
OBJS=test.o jim.o
CFLAGS=-W -Wall -Werror -Wextra -pedantic -O2
APP=jim-test

.PHONY=all clean

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $(APP) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) -f $(OBJS) $(APP)
