cc := clang
coflags :=  -Isrc/include -Isrc/include -Isrc/include $(shell if [ $${RELEASE:-0} -eq 1 ]; then echo -O3; else echo -g -O0; fi )
cbflags := -pthread -lgmp -lssh 

objs := $(shell find src -iname '*.c' | sed "s,src,obj,g; s,c$$,o,g")

.PHONY: all objects clean

objects: $(objs)
all: $(objs) bin/volker

clean:
	rm -f $(objs) $(testbin) bin/volker


obj/%.o: src/%.c
	mkdir -p $(shell dirname $< | sed 's,src,obj,g')
	$(cc) $(coflags) -c -o $@ $<

bin/volker: $(objs)
	mkdir -p bin
	$(cc) $(cbflags) -o $@ $^
