BOOT = boot
SRC = src
BIN = bin

C_FILES := $(wildcard $(SRC)/**/*.c $(SRC)/*.c)
S_FILES := $(wildcard $(SRC)/**/*.s $(SRC)/*.s)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(C_FILES)) $(patsubst $(SRC)/%.s, $(BIN)/%.o, $(S_FILES))

all: $(BIN)/myos.bin

$(BIN)/%.o: $(SRC)/%.s
	mkdir -p $(dir $@) # Ensure the output directory exists
	i686-elf-as $< -o $@

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@) # Ensure the output directory exists
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/myos.bin: $(OBJ_FILES)
	i686-elf-gcc -T $(SRC)/boot/linker.ld -o $@ -ffreestanding -O2 -nostdlib $(OBJ_FILES) -lgcc

run: $(BIN)/myos.bin
	qemu-system-i386 -kernel $<

clean:
	rm -f $(BIN)/*.o $(BIN)/myos.bin
	find $(BIN) -type d -empty -delete
