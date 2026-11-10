STD := std
STD_OBJ_DIR := obj_std

STD_SRC := $(shell find $(STD) -name '*.asm')
STD_OBJ := $(patsubst $(STD)/%.asm,$(STD_OBJ_DIR)/%.asm.o,$(STD_SRC))

NASM := nasm
AR := ar

NASMFLAGS := -f elf64
ARFLAGS := rcs

TARGET := libwcstd.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(STD_OBJ)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) -o $@ $^

$(STD_OBJ_DIR)/%.asm.o: $(STD)/%.asm
	mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) -o $@ $^

clean:
	rm -rf $(STD_OBJ_DIR)
	rm $(TARGET)