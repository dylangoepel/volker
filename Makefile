cc := clang
coflags :=  -Isrc/include -Isrc/include -Isrc/include $(shell if [ $${RELEASE:-0} -eq 1 ]; then echo -O3; else echo -g -O0; fi )
cbflags := -pthread -lgmp -lssh 

objs := $(shell find src -iname '*.c' | sed "s,src,obj,g; s,c$$,o,g")

.PHONY: all objects clean

objects: $(objs)
all: $(objs) bin/volker

testsbin := $(shell find tests -iname '*.c'  | sed "s,tests/,bin/test_,g; s,\.c$$,,g")

clean:
	rm -f $(objs) $(testbin) bin/volker


obj/%.o: src/%.c
	mkdir -p $(shell dirname $< | sed 's,src,obj,g')
	$(cc) $(coflags) -c -o $@ $<

bin/volker: $(objs)
	mkdir -p bin
	$(cc) $(cbflags) -o $@ $^

bin/test_%: tests/%.c $(objs)
	mkdir -p bin
	$(cc) $(coflags) $(cbflags) -o $@ $^

tests: $(testsbin)

test: tests
	for test in bin/test*; do echo -e "\033[32m[+]" running $$test "\033[00m"; $$test || echo -e "\033[31m[-] returned error: $$? \033[00m"; done
