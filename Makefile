cc := clang
coflags :=  -pthread -Isrc/include $(shell if [ $${RELEASE:-0} -eq 1 ]; then echo -O3; else echo -g -O0; fi )
cbflags := -pthread -lgmp -lssh -lcrypto

objs := $(shell find src -iname '*.c' | sed "s,src,obj,g; s,c$$,o,g" | grep -v "obj/main.o")

testsbin := $(shell find tests -iname '*.c'  | sed "s,tests/,bin/test_,g; s,\.c$$,,g")
benchbin := $(shell find benchmarks -iname '*.c' | sed "s,benchmarks/,bin/bm_,g; s,\.c$$,,g")

.PHONY: all objects clean

all: compile_flags.txt $(objs) $(testsbin) $(benchbin) obj/main.o bin/volker

objects: $(objs)

clean:
	rm -f $(objs) $(testbin) bin/volker
	rm -rf bin/test.pem
	rm -f compile_flags.txt
	rm -f test.pem

obj/%.o: src/%.c
	mkdir -p $(shell dirname $< | sed 's,src,obj,g')
	$(cc) $(coflags) -c -o $@ $<

bin/volker: $(objs) obj/main.o
	mkdir -p bin
	$(cc) $(cbflags) -o $@ $^

bin/test_%: tests/%.c $(objs)
	mkdir -p bin
	$(cc) $(coflags) $(cbflags) -o $@ $^

bin/bm_%: benchmarks/%.c $(objs)
	mkdir -p bin
	$(cc) $(coflags) $(cbflags) -o $@ $^

tests: $(testsbin)
benchmarks: $(benchbin)

bm: benchmarks
	if [ -f dat/bm.dat ]; then mv dat/bm.dat dat/bm.old.dat; fi
	for bm in bin/bm_*; do $$bm; done > dat/bm.dat
	python3 dat/bm.py

test: tests
	for test in bin/test*; do echo -e "\033[32m[+]" running $$test "\033[00m"; $$test || echo -e "\033[31m[-] returned error: $$? \033[00m"; done

compile_flags.txt:
	for flag in $(coflags) $(cbflags); do echo $$flag; done > compile_flags.txt
