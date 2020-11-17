cc := clang
cflags := -pthread -Wall -lgmp -Wunused-command-line-argument

objs := $(shell find src -iname '*.c' | sed "s,src,obj,g; s,c$$,o,g")

.PHONY: all clean

all: $(objs) bin/volker

clean:
	rm -f $(objs) bin/volker


obj/%.o: src/%.c
	mkdir -p $(shell dirname $< | sed 's,src,obj,g')
	$(cc) $(cflags) -c -o $@ $<

bin/volker: $(objs)
	mkdir -p bin
	$(cc) $(cflags) -o $@ $^
