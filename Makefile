CFLAGS=-std=c11 -g -fno-common -Wall -Wno-switch

INCDIR := compiler/include
SRCDIR := compiler/src
OBJDIR := compiler/obj

SRCS=$(shell find $(SRCDIR) -name '*.c')
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.c.o,$(SRCS))

TEST_SRCS=$(wildcard test/*.wc)
TESTS=$(TEST_SRCS:.wc=.exe)

# Stage 1

wincc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.c.o: $(SRCDIR)/%.c $(INCDIR)/wincc.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

test/%.exe: wincc test/%.wc
	./wincc --project test -I$(INCDIR) -Itest -xc -c -o test/$*.o test/$*.wc
	$(CC) -pthread -o $@ test/$*.o

test: $(TESTS)
	for i in $^; do echo $$i; ./$$i || exit 1; echo; done

test-all: test test-stage2

# Stage 2

stage2/wincc: $(OBJS:%=stage2/%)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

stage2/$(OBJDIR)/%.c.o: wincc $(SRCDIR)/%.c $(INCDIR)/wincc.h
	mkdir -p $(@D)
	./wincc -I$(INCDIR) -c -o $@ $(SRCDIR)/$*.c

stage2/test/%.exe: stage2/wincc test/%.wc
	mkdir -p stage2/test
	./stage2/wincc -I$(INCDIR) -Itest -xc -c -o stage2/test/$*.o test/$*.wc
	$(CC) -pthread -o $@ stage2/test/$*.o

test-stage2: $(TESTS:test/%=stage2/test/%)
	for i in $^; do echo $$i; ./$$i || exit 1; echo; done

# Misc.

clean:
	rm -rf wincc tmp* $(TESTS) test/*.o test/*.s test/*.exe stage2
	rm -rf $(OBJDIR)
	find * -type f '(' -name '*~' -o -name '*.o' ')' -exec rm {} ';'

install:
	sudo cp -v ./wincc /usr/bin/wincc

.PHONY: test test-all clean test-stage2 install