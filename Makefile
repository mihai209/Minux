CC=clang
CFLAGS=-ffreestanding -fno-builtin -fno-stack-protector -O2 -Wall -Wextra -mno-red-zone
LDFLAGS=-nostdlib -T linker.ld

BUILD=build

all: $(BUILD)/kernel.elf

# creează folder build dacă nu există
$(BUILD):
	mkdir -p $(BUILD)

# compilează boot.asm
$(BUILD)/boot.o: boot/boot.asm | $(BUILD)
	nasm -f elf64 $< -o $@

# compilează load.c
$(BUILD)/load.o: bootloader/load.c bootloader/load.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# compilează syscheck.c
$(BUILD)/syscheck.o: bootloader/syscheck.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# compilează kernel.c
$(BUILD)/kernel.o: kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# link kernel.elf
$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/load.o $(BUILD)/syscheck.o $(BUILD)/kernel.o
	ld $(LDFLAGS) $^ -o $@

# rulează direct în QEMU
run: $(BUILD)/kernel.elf
	qemu-system-x86_64 -kernel $(BUILD)/kernel.elf -m 512M -serial stdio

# clean
clean:
	rm -rf $(BUILD)
