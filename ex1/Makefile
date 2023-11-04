CFLAGS=-Wall -Werror -g  -fdiagnostics-color=always

TARGETS = readFirewall checkPacket

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(TARGETS) *.o
