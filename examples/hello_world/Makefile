WINCC := wincc
WINCFLAGS := --project .
LDFLAGS := -L"$(HOME)/.winc" -lwcstd

SOURCES := $(shell find . -name '*.wc')
TARGET := hello_world

all: $(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p $(dir $@)
	$(WINCC) $(WINCFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm $(TARGET)

run:
	./$(TARGET)

.PHONY: all clean run