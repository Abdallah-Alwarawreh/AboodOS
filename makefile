BOOT = boot
SRC = src
BIN = bin

all: $(BIN)/myos.bin

$(BIN)/boot.o: $(SRC)/boot/boot.s
	i686-elf-as $< -o $@

$(BIN)/kernel.o: $(SRC)/boot/kernel.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/vga.o: $(SRC)/vga.c $(SRC)/vga.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/keyboard.o: $(SRC)/keyboard.c $(SRC)/keyboard.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/string_utils.o: $(SRC)/string_utils.c $(SRC)/string_utils.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/idt.o: $(SRC)/utils/idt.c $(SRC)/utils/idt.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)/utils

$(BIN)/irq.o: $(SRC)/utils/irq.c $(SRC)/utils/irq.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)/utils

$(BIN)/isr.o: $(SRC)/utils/isr.c $(SRC)/utils/isr.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)/utils

$(BIN)/util.o: $(SRC)/utils/util.c $(SRC)/utils/util.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)/utils

$(BIN)/timer.o: $(SRC)/utils/timer.c $(SRC)/utils/timer.h
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)/utils

$(BIN)/myos.bin: $(BIN)/boot.o $(BIN)/kernel.o $(BIN)/vga.o $(BIN)/keyboard.o $(BIN)/string_utils.o $(BIN)/idt.o $(BIN)/isr.o $(BIN)/irq.o $(BIN)/util.o $(BIN)/timer.o
	i686-elf-gcc -T $(SRC)/boot/linker.ld -o $@ -ffreestanding -O2 -nostdlib $(BIN)/boot.o $(BIN)/kernel.o $(BIN)/vga.o $(BIN)/keyboard.o $(BIN)/string_utils.o $(BIN)/idt.o $(BIN)/isr.o $(BIN)/irq.o $(BIN)/util.o $(BIN)/timer.o -lgcc

run: $(BIN)/myos.bin
	qemu-system-i386 -kernel $<

clean:
	rm -f $(BIN)/*.o $(BIN)/myos.bin
