CC           = gcc
LD           = gcc
AR           = ar
ARFLAGS      = rcs
CFLAGS       = -Wall -Wno-unused-function -Os -c
LDFLAGS      = -Wall -Os -Wl,-Map,test.map

.PHONY: all clean

all: regfile.elf


aes.o : src/aes-c/aes.c src/aes-c/aes.h
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

regfile.o : src/regfile.c src/aes-c/aes.h src/aes-c/aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

regfile.elf : regfile.o src/aes-c/aes.o
	echo [LD] $@ $(LDFLAGS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o regfile.elf test.map
