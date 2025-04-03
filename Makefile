# Directories
# Flags
CFLAGS = -g -Wall -pthread -Wpedantic -Wextra -Wsign-compare -Werror # -Wconversion
CC = cc $(CFLAGS)


# Targets
TARGETS = \
    azi 

# Outputs
OUTPUTS = \
	azi

LIBS = \
	libmyazi.a 

all: $(LIBS) $(TARGETS)

libs: $(LIBS)

install: $(TARGETS)
	mv $(TARGETS) ~/bin/

clean:
	rm -r $(TARGETS) $(OUTPUTS) $(LIBS)

%.o: %.c %.h
	$(CC) -c -o $@ $<

libmyazi.a: libmyazi.o libmyazi.h
	ar -rcs $@ $<

%: %.c $(LIBS)
	$(CC) -L. -o $@ $< -lmyazi -lm  
